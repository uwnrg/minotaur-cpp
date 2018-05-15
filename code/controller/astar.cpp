#include "astar.h"

#include "../gui/griddisplay.h"
#include "../compstate/parammanager.h"
#include "../utility/algorithm.h"

#include <unordered_set>

#include <set>
#include <map>

#ifndef NDEBUG

#include <cassert>

#endif

#define TERRAIN_WALL -1

struct node {

    node() :
        x(0),
        y(0),
        g(0),
        h(0),
        f(0),
        terrain(0),
        parent(nullptr) {}

    bool operator==(const node &o) {
        return x == o.x && y == o.y;
    }

    bool operator!=(const node &o) {
        return x != o.x || y != o.y;
    }

    int x;
    int y;

    int g;
    int h;
    int f;

    int terrain;

    node *parent;
};

static int get_h(node *a, node *b) {
    int dx = a->x - b->x;
    int dy = a->y - b->y;
    return abs(dx) + abs(dy);
}

static void backtrack(
    node *start,
    node *dest,
    std::list<node *> &path
) {
    node *cur = dest;
    while (cur != start) {
        path.push_front(cur);
        cur = cur->parent;
    }
}

static void get_neighbors(
    node *cur,
    array2d<node *, int> &world,
    std::list<node *> &neighbors
) {
    int cx = cur->x;
    int cy = cur->y;
    int xs[] = {cx - 1, cx, cx, cx + 1};
    int ys[] = {cy, cy - 1, cy + 1, cy};
    for (int i = 0; i < 4; ++i) {
        cx = xs[i];
        cy = ys[i];
        if (cx < 0 ||
            cy < 0 ||
            cx >= world.x() ||
            cy >= world.y()) {
            continue;
        }
        if (world[cx][cy]->terrain != TERRAIN_WALL) {
            neighbors.push_back(world[cx][cy]);
        }
    }
}

static void astar_search_path(
    node *start,
    node *dest,
    array2d<node *, int> &world,
    std::list<node *> &path
) {
    std::unordered_set<node *> open_set;
    std::unordered_set<node *> closed_set;
    node *cur;

    start->g = 0;
    start->h = get_h(start, dest);
    start->f = start->h;

    open_set.insert(start);
    while (!open_set.empty()) {
        cur = nullptr;
        for (node *node : open_set) {
            if (cur == nullptr ||
                node->f < cur->f ||
                (node->f == cur->f && node->h < cur->h)) {
                cur = node;
            }
        }
        if (cur == nullptr) {
            break;
        }
        open_set.erase(cur);
        closed_set.insert(cur);

        std::list<node *> neighbors;
        get_neighbors(cur, world, neighbors);

        for (node *neigh : neighbors) {
            if (closed_set.find(neigh) != closed_set.end()) {
                continue;
            }
            int cur_g = cur->g + get_h(cur, neigh) + world[neigh->x][neigh->y]->terrain;
            if (cur_g <= neigh->g || neigh->g == 0) {
                neigh->g = cur_g;
                neigh->h = get_h(neigh, dest);
                neigh->f = neigh->h + neigh->g;
                neigh->parent = cur;
            }
            open_set.insert(neigh);
        }
        if (cur->x == dest->x && cur->y == dest->y) {
            backtrack(start, dest, path);
            return;
        }
    }
}

typedef std::pair<double, vector2i> associated_cost;

static double manhattan_dist(
    const vector2i &cur,
    const vector2i &dest
) {
    return abs(dest.x() - cur.x()) + abs(dest.y() - cur.y());
}

static bool is_valid(
    int x, int y,
    int max_row, int max_col
) {
    return (x >= 0) && (x < max_row) && (y >= 0) && (y < max_col);
}

static void get_neighbors(
    const vector2i &c,
    int max_row, int max_col,
    std::vector<vector2i> &neighbors
) {
    int cur_x = c.x();
    int cur_y = c.y();
    if (is_valid(cur_x - 1, cur_y, max_row, max_col)) {
        neighbors.emplace_back(cur_x - 1, cur_y);
    }
    if (is_valid(cur_x + 1, cur_y, max_row, max_col)) {
        neighbors.emplace_back(cur_x + 1, cur_y);
    }
    if (is_valid(cur_x, cur_y - 1, max_row, max_col)) {
        neighbors.emplace_back(cur_x, cur_y - 1);
    }
    if (is_valid(cur_x, cur_y + 1, max_row, max_col)) {
        neighbors.emplace_back(cur_x, cur_y + 1);
    }
}

static void backtrack(
    const vector2i &start,
    const vector2i &dest,
    const std::map<vector2i, vector2i> &parent,
    std::vector<vector2i> &path
) {
    vector2i cur = dest;
    while (cur != start) {
        path.push_back(cur);
        cur = parent.at(cur);
    }
    path.push_back(start);
    std::reverse(std::begin(path), std::end(path));
}

void nrg::search_path(
    array2d<int> &terrain,
    const vector2i &start,
    const vector2i &dest,
    std::vector<vector2i> &path
) {
    int mx = static_cast<int>(terrain.x());
    int my = static_cast<int>(terrain.y());
    std::list<node *> node_path;
    array2d<node, int> nodes(mx, my);
    array2d<node *, int> world(mx, my);
    for (unsigned int x = 0; x < terrain.x(); ++x) {
        for (unsigned int y = 0; y < terrain.y(); ++y) {
            nodes[x][y].x = x;
            nodes[x][y].y = y;
            nodes[x][y].terrain = terrain[x][y];
            world[x][y] = &nodes[x][y];
        }
    }
    node *node_start = world[start.x()][start.y()];
    node *node_dest = world[dest.x()][dest.y()];
    astar_search_path(node_start, node_dest, world, node_path);
    for (node *node : node_path) {
        path.emplace_back(node->x, node->y);
    }
}

void nrg::search_path_del(
    array2d<int> &terrain,
    const vector2i &start,
    const vector2i &dest,
    std::vector<vector2i> &path
) {
    std::map<vector2i, vector2i> parent;
    std::map<vector2i, double> cost;
    std::set<associated_cost> open_set;

    open_set.emplace(0, start);
    parent[start] = start;
    cost[start] = 0;

    int max_row = static_cast<int>(terrain.x());
    int max_col = static_cast<int>(terrain.y());

    while (!open_set.empty()) {
        vector2i cur = open_set.begin()->second;
        open_set.erase(open_set.begin());
        if (cur == dest) { break; }

        std::vector<vector2i> neighbors;
        get_neighbors(cur, max_row, max_col, neighbors);
        for (const vector2i &next : neighbors) {
            if (terrain[next.x()][next.y()] != TERRAIN_WALL) {
                double new_cost = cost[cur] + terrain[next.x()][next.y()];
                if (!cost.count(next) || new_cost < cost[next]) {
                    cost[next] = new_cost;
                    open_set.emplace(new_cost + manhattan_dist(next, dest), next);
                    parent[next] = cur;
                }
            }
        }
    }
    backtrack(start, dest, parent, path);
}

static void apply_kernel(
    array2d<int> &source,
    array2d<int> &target,
    unsigned int x, unsigned int y,
    int wall, int wp0, int wp1, int wp2
) {
    using std::max;
    using std::min;
    using std::abs;
#ifndef NDEBUG
    assert(x < source.x());
    assert(y < source.y());
    assert(source[x][y] == wall);
#endif
    constexpr unsigned int offset = 2;
    int wp[] = {0, wp0, wp1, wp2};
    int x_min = -min(offset, x);
    int y_min = -min(offset, y);
    int x_max = min(offset, static_cast<int>(source.x()) - x - 1);
    int y_max = min(offset, static_cast<int>(source.y()) - y - 1);
    for (int dx = x_min; dx <= x_max; ++dx) {
        for (int dy = y_min; dy <= y_max; ++dy) {
            int tx = x + dx;
            int ty = y + dy;
            if (source[tx][ty] != wall) {
                target[tx][ty] += wp[max(abs(dx), abs(dy))];
            }
        }
    }
}

static void kernelize(
    array2d<int> &source,
    array2d<int> &target,
    int wall, int wp0, int wp1, int wp2
) {
#ifndef NDEBUG
    assert(source.x() == target.x());
    assert(source.y() == target.y());
#endif
    for (unsigned int x = 0; x < source.x(); ++x) {
        for (unsigned int y = 0; y < source.y(); ++y) {
            if (source[x][y] == wall) {
                target[x][y] = TERRAIN_WALL;
                apply_kernel(
                    source, target,
                    x, y, wall,
                    wp0, wp1, wp2
                );
            }
        }
    }
}

array2d<int> nrg::grid_kernelize(
    weak_ref<GridDisplay> grid,
    weak_ref<param_manager> pm
) {
    constexpr int wall = GridDisplay::DEFAULT_WEIGHT;
    int wp0 = pm->wall_penalty_0;
    int wp1 = pm->wall_penalty_1;
    int wp2 = pm->wall_penalty_2;
    auto mx = static_cast<std::size_t>(grid->get_num_cols());
    auto my = static_cast<std::size_t>(grid->get_num_rows());
    array2d<int> source(mx, my);
    array2d<int> terrain(mx, my);
    array2d<int> &selected = grid->selected();
    for (unsigned int x = 0; x < mx; ++x) {
        memcpy(source[x].get(), selected[x].get(), my * sizeof(int));
    }
    kernelize(source, terrain, wall, wp0, wp1, wp2);
    return terrain; // move constructor
}

std::vector<vector2i> nrg::grid_path(
    weak_ref<GridDisplay> grid,
    weak_ref<param_manager> pm
) {
    std::vector<vector2i> path;
    array2d<int> terrain = nrg::grid_kernelize(grid, pm);
    const vector2i &start = grid->get_pos_start();
    const vector2i &dest = grid->get_pos_end();
    search_path(terrain, start, dest, path);
    smooth_path(path);
    //optimize_path(path, grid->selected());
    return path; // move constructor
}

void nrg::scale_path_pixels(
    weak_ref<GridDisplay> grid,
    std::vector<vector2i> &path
) {
    constexpr int b = GridDisplay::GRID_SIZE;
    constexpr int b2 = b / 2;
    int gx = grid->x();
    int gy = grid->y();
    for (vector2i &v : path) {
        v.x() = gx + b2 + v.x() * b;
        v.y() = gy + b2 + v.y() * b;
    }
}

void nrg::connect_path(
    weak_ref<GridDisplay> grid,
    weak_ref<param_manager> pm
) {
#ifndef NDEBUG
    assert(dynamic_cast<ImageViewer *>(grid->parent()) != nullptr);
#endif
    std::vector<vector2i> path = nrg::grid_path(grid, pm);
    nrg::scale_path_pixels(grid, path);
    weak_ref<ImageViewer> viewer = dynamic_cast<ImageViewer *>(grid->parent());
    viewer->set_path(path);
}

void nrg::smooth_path(std::vector<vector2i> &path) {
    std::vector<vector2i> smooth;
    smooth.push_back(path.front());
    for (std::size_t i = 1; i < path.size(); ++i) {
        const vector2i &v = path.at(i);
        const vector2i &p = smooth.back();
        if (p.x() != v.x() &&
            p.y() != v.y()) {
            smooth.push_back(path[i - 1]);
        }
    }
    smooth.push_back(path.back());
    path.swap(smooth);
}

static void wallify(
    array2d<int> &walls,
    std::vector<rect2i> &rects
) {
    constexpr int wall = GridDisplay::DEFAULT_WEIGHT;
    constexpr int b = GridDisplay::GRID_SIZE;
    constexpr int b2 = b / 2;
    for (std::size_t x = 0; x < walls.x(); ++x) {
        for (std::size_t y = 0; y < walls.y(); ++y) {
            if (walls[x][y] == wall) {
                rects.emplace_back(
                    static_cast<int>(x) - b2,
                    static_cast<int>(y) - b2,
                    b, b
                );
            }
        }
    }
}

static std::pair<ray2i, ray2i> l_upper(
    const vector2i &c,
    const vector2i &p
) {
    vector2i m(c.x(), p.y());
    return {{c, m},
            {m, p}};
};

static std::pair<ray2i, ray2i> l_lower(
    const vector2i &c,
    const vector2i &p
) {
    vector2i m(p.x(), c.y());
    return {{c, m},
            {m, p}};
};

static bool collides_with_any(
    const ray2i &ray,
    const std::vector<rect2i> &rects
) {
    for (const rect2i &rect : rects) {
        if (algo::ray_aabb_intersect(ray, rect)) {
            return true;
        }
    }
    return false;
}

void nrg::optimize_path(
    std::vector<vector2i> &path,
    array2d<int> &walls
) {
    typedef std::pair<ray2i, ray2i> l_path;

    std::vector<vector2i> opt;
    std::vector<rect2i> rects;
    wallify(walls, rects);

    vector2i c = path.front();
    std::size_t j = 0;
    std::size_t m = path.size();
    opt.push_back(c);

    int upper_pass = 0;
    int lower_pass = 0;
    for (std::size_t i = 1; i < m; ++i) {
        if (j + 3 >= m) {
            opt.push_back(path[i]);
        }
        if (i <= j + 2) {
            continue;
        }
        const vector2i &p = path.at(i);
        l_path upper = l_upper(c, p);
        l_path lower = l_lower(c, p);
        int expected = static_cast<int>(i - j) - 3;
        if (!collides_with_any(upper.first, rects) &&
            !collides_with_any(upper.second, rects) &&
            expected == upper_pass) {
            ++upper_pass;
        }
        if (!collides_with_any(lower.first, rects) &&
            !collides_with_any(lower.second, rects) &&
            expected == lower_pass) {
            ++lower_pass;
        }
        ++expected;
        if ((
                expected != upper_pass &&
                expected != lower_pass) ||
            i + 1 == m) {
            const vector2i &prev = path[i - 1];
            l_path ref =
                upper_pass >= lower_pass
                ? l_upper(c, prev)
                : l_lower(c, prev);
            opt.push_back(ref.second.a());
            opt.push_back(ref.second.b());
            upper_pass = 0;
            lower_pass = 0;
            j = i;
            c = p;
        }
    }
}
