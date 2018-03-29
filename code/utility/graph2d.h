#ifndef MINOTAUR_CPP_GRAPH2D_H
#define MINOTAUR_CPP_GRAPH2D_H

#include <unordered_map>
#include <unordered_set>

#include "vector.h"

namespace nrg {

    template<typename val_t>
    class node2d {
    public:
        typedef std::size_t id_t;

        node2d(id_t id, const vector<val_t> &pos) :
            m_id(id),
            m_pos(pos) {}

        const id_t &id() const {
            return m_id;
        }

        const vector<val_t> &pos() const {
            return m_pos;
        }

    private:
        id_t m_id;
        vector<val_t> m_pos;
    };

    template<typename val_t>
    class graph2d {
    public:
        typedef typename node2d<val_t>::id_t id_t;

        static nrg::path<val_t> to_path(const std::vector<node2d<val_t>> &node_path) {
            nrg::path<val_t> path;
            for (const node2d<val_t> &node : node_path) {
                path.push_back(node.pos());
            }
            return path;
        }

    private:
        typedef std::unordered_set<id_t> id_set;
        typedef std::vector<node2d<val_t>> node_list;
        typedef std::unordered_map<id_t, id_set> connection_map;
        typedef std::unordered_map<id_t, id_t> id_map;
        typedef std::unordered_map<id_t, val_t> score_map;

        static id_t node_min_fscore(
            const id_set &open_set,
            const score_map &f_scores
        ) {
            // Linear to find the best node but graph2d is meant for small
            // applications (less than 20 nodes), can make the open_set an
            // ordered map on f_score if really needed
            id_t min_id = *open_set.begin();
            for (const id_t &id : open_set) {
                if (score_of(id, f_scores) < score_of(min_id, f_scores)) { min_id = id; }
            }
            return min_id;
        }

        static val_t score_of(
            const id_t &id,
            const score_map &scores
        ) {
            auto it = scores.find(id);
            return it == scores.end() ? std::numeric_limits<val_t>::max() : it->second;
        }

        static val_t dist(
            const node2d<val_t> &n0,
            const node2d<val_t> &n1
        ) {
            return (n1.pos() - n0.pos()).norm();
        }

    public:
        const node2d<val_t> &add_node(const vector<val_t> &coord) {
            id_t id = m_nodes.size();
            m_nodes.emplace_back(id, coord);
            return m_nodes[id];
        }

        void connect(const node2d<val_t> &n0, const node2d<val_t> &n1) {
            m_connections[n0.id()].insert(n1.id());
            m_connections[n1.id()].insert(n0.id());
        }

        std::vector<node2d<val_t>> astar(const node2d<val_t> &start, const node2d<val_t> &end) const {
            id_set closed_set;
            id_set open_set;
            id_map came_from;
            score_map g_scores;
            score_map f_scores;
            open_set.insert(start.id());
            g_scores[start.id()] = 0;
            f_scores[start.id()] = dist(start, end);
            while (!open_set.empty()) {
                id_t current = node_min_fscore(open_set, f_scores);
                if (current == end.id()) {
                    return reconstruct_path(came_from, current);
                }
                open_set.erase(current);
                closed_set.insert(current);
                for (const id_t &nb : m_connections.at(current)) {
                    if (closed_set.find(nb) != closed_set.end()) { continue; }
                    if (open_set.find(nb) == open_set.end()) { open_set.insert(nb); }
                    val_t tentative = score_of(current, g_scores) + dist(m_nodes[current], m_nodes[nb]);
                    if (tentative >= score_of(nb, g_scores)) { continue; }
                    came_from[nb] = current;
                    g_scores[nb] = tentative;
                    f_scores[nb] = tentative + dist(m_nodes[nb], end);
                }
            }
            return {};
        }

    private:
        std::vector<node2d<val_t>> reconstruct_path(
            const id_map &came_from,
            id_t current
        ) const {
            std::vector<node2d<val_t>> path = {m_nodes[current]};
            while (came_from.find(current) != came_from.end()) {
                current = came_from.at(current);
                path.push_back(m_nodes[current]);
            }
            std::reverse(path.begin(), path.end());
            return path;
        }

        node_list m_nodes;
        connection_map m_connections;
    };

}

#endif //MINOTAUR_CPP_GRAPH2D_H
