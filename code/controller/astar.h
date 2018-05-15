#ifndef ASTAR_H
#define ASTAR_H

#include "../utility/array2d.h"
#include "../utility/vector.h"
#include "../utility/weak_ref.h"

class GridDisplay;

class param_manager;

namespace nrg {
    void search_path(
        array2d<int> &terrain,
        const vector2i &start,
        const vector2i &dest,
        std::vector<vector2i> &path
    );

    void search_path_del(
        array2d<int> &terrain,
        const vector2i &start,
        const vector2i &dest,
        std::vector<vector2i> &path
    );

    array2d<int> grid_kernelize(
        weak_ref<GridDisplay> grid,
        weak_ref<param_manager> pm
    );

    std::vector<vector2i> grid_path(
        weak_ref<GridDisplay> grid,
        weak_ref<param_manager> pm
    );

    void scale_path_pixels(
        weak_ref<GridDisplay> grid,
        std::vector<vector2i> &path
    );

    void connect_path(
        weak_ref<GridDisplay> grid,
        weak_ref<param_manager> pm
    );

    void smooth_path(std::vector<vector2i> &path);

    void optimize_path(
        std::vector<vector2i> &path,
        array2d<int> &walls
    );
}

#endif
