#include "compstate.h"

#include <cassert>

#ifndef NDEBUG
#include <QDebug>
#endif

bool CompetitionState::s_tracking_robot = false;
bool CompetitionState::s_tracking_object = false;
bool CompetitionState::s_acquire_walls = false;

bool **CompetitionState::s_walls = nullptr;
int CompetitionState::s_walls_x = 0;
int CompetitionState::s_walls_y = 0;

int CompetitionState::s_object_type = ObjectType::UNACQUIRED;

bool CompetitionState::reinitialize_walls(int x, int y) {
#ifndef NDEBUG
    assert(x > 0);
    assert(y > 0);
#endif
    bool was_initialized = s_walls != nullptr;
    // s_walls is column-major
    if (was_initialized) {
       delete_walls();
    }
    s_walls = new wall_t*[x];
    for (int tx = 0; tx < x; ++tx) {
        s_walls[tx] = new wall_t[y];
    }
    s_walls_x = x;
    s_walls_y = y;
    return was_initialized;
}

void CompetitionState::delete_walls() {
    if (s_walls != nullptr) {
#ifndef NDEBUG
        assert(s_walls_x > 0);
        assert(s_walls_y > 0);
#endif
        for (int tx = 0; tx < s_walls_x; ++tx) {
            delete[] s_walls[tx];
        }
        delete[] s_walls;
        s_walls = nullptr;
    }
}

wall_t CompetitionState::wall(int x, int y) {
#ifndef NDEBUG
    assert(x >= 0);
    assert(y >= 0);
    assert(x < s_walls_x);
    assert(y < s_walls_y);
#endif
    return s_walls[x][y];
}

wall_t **CompetitionState::wall_ptr() {
    return s_walls;
}

int CompetitionState::object_type() {
    return s_object_type;
}

bool CompetitionState::is_walls_initialized() {
    return s_walls != nullptr;
}
