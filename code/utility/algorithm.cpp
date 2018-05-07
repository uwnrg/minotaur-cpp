#include "algorithm.h"

nrg::dir invert_dir(nrg::dir dir)  {
    // Direction is cyclic modulus 4 and
    // TOP + 2 = BOTTOM
    return static_cast<nrg::dir>((dir + 2) % 4);
}

nrg::dir rotate_cw(nrg::dir dir) {
    return static_cast<nrg::dir>((dir + 1) % 4);
}

bool is_dir_hor(nrg::dir dir)  {
    // Odd values are horizontal
    return static_cast<bool>(dir % 2);
}

bool is_dir_ver(nrg::dir dir) {
    return !is_dir_hor(dir);
}
