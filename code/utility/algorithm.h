#ifndef MINOTAUR_CPP_ALGORITHM_H
#define MINOTAUR_CPP_ALGORITHM_H

#include "ray.h"
#include "rect.h"

namespace algo {

    /**
     * Method for checking the intersection of two line segments
     * defined by two points each.
     *
     * @tparam val_t
     * @param p0_x
     * @param p0_y
     * @param p1_x
     * @param p1_y
     * @param p2_x
     * @param p2_y
     * @param p3_x
     * @param p3_y
     * @param i_x
     * @param i_y
     * @return
     */
    template<typename val_t>
    bool __line_t_intersect(
        val_t p0_x, val_t p0_y,
        val_t p1_x, val_t p1_y,
        val_t p2_x, val_t p2_y,
        val_t p3_x, val_t p3_y,
        val_t *i_x, val_t *i_y
    ) {
        val_t s1_x, s1_y, s2_x, s2_y;
        s1_x = static_cast<val_t>(p1_x - p0_x);
        s1_y = static_cast<val_t>(p1_y - p0_y);
        s2_x = static_cast<val_t>(p3_x - p2_x);
        s2_y = static_cast<val_t>(p3_y - p2_y);
        val_t s, t;
        s = static_cast<val_t>(
            (-s1_y * (p0_x - p2_x) + s1_x * (p0_y - p2_y)) /
            (-s2_x * s1_y + s1_x * s2_y)
        );
        t = static_cast<val_t>(
            (s2_x * (p0_y - p2_y) - s2_y * (p0_x - p2_x)) /
            (-s2_x * s1_y + s1_x * s2_y)
        );
        if (s >= 0 && s <= 1 && t >= 0 && t <= 1) {
            // Collision detected
            *i_x = static_cast<val_t>(p0_x + (t * s1_x));
            *i_y = static_cast<val_t>(p0_y + (t * s1_y));
            return true;
        }
        return false; // No collision
    }

    /**
     * Determines whether two line segments intersect.
     *
     * @tparam val_t
     * @param r0
     * @param r1
     * @return
     */
    template<typename val_t>
    std::tuple<bool, nrg::vector<val_t>>
    ray_intersect(const nrg::ray<val_t> &r0, const nrg::ray<val_t> &r1) {
        val_t i_x = 0;
        val_t i_y = 0;
        bool i = __line_t_intersect(
            r0.a().x(),
            r0.a().y(),
            r0.b().x(),
            r0.b().y(),
            r1.a().x(),
            r1.a().y(),
            r1.b().x(),
            r1.b().y(),
            &i_x,
            &i_y
        );
        return std::make_tuple(i, nrg::vector<val_t>(i_x, i_y));
    }

    /**
     * Naive function to determine whether a line segment
     * intersects with an AABB.
     *
     * @tparam val_t
     * @param v
     * @param r
     * @return
     */
    template<typename val_t>
    bool ray_rect_intersect(const nrg::ray<val_t> &v, const nrg::rect<val_t> &r) {
        // Check intersection with each rectangle side
        return
            std::get<0>(ray_intersect(v, r.tltr())) ||
            std::get<0>(ray_intersect(v, r.trbr())) ||
            std::get<0>(ray_intersect(v, r.brbl())) ||
            std::get<0>(ray_intersect(v, r.bltl()));
    }

    /**
     * Determines whether a line segment intersects with an AABB.
     *
     * @tparam val_t
     * @param v
     * @param aabb
     * @return
     */
    template<typename val_t>
    bool ray_aabb_intersect(const nrg::ray<val_t> &v, const nrg::rect<val_t> &aabb) {
        nrg::vector<val_t> n_inv = v.n_inv();
        val_t tx1 = (aabb.tl().x() - v.a().x()) * n_inv.x();
        val_t tx2 = (aabb.br().x() - v.a().x()) * n_inv.x();
        val_t tmin = std::min(tx1, tx2);
        val_t tmax = std::max(tx1, tx2);
        val_t ty1 = (aabb.tl().y() - v.a().y()) * n_inv.y();
        val_t ty2 = (aabb.br().y() - v.a().y()) * n_inv.y();
        tmin = std::max(tmin, std::min(ty1, ty2));
        tmax = std::min(tmax, std::max(ty1, ty2));
        return tmax >= tmin;
    }

    /**
     * Determines whether two AABBs are in collision.
     *
     * @tparam val_t
     * @param r0
     * @param r1
     * @return
     */
    template<typename val_t>
    bool aabb_collide(
        const nrg::rect<val_t> &r0,
        const nrg::rect<val_t> &r1
    ) {
        nrg::vector<val_t> c0 = r0.center();
        nrg::vector<val_t> c1 = r1.center();
        val_t half_dx = r0.width() + r1.width();
        val_t half_dy = r0.height() + r1.height();
        half_dx *= 0.5;
        half_dy *= 0.5;
        bool x_overlap = abs(c0.x() - c1.x()) <= half_dx;
        bool y_overlap = abs(c0.y() - c1.y()) <= half_dy;
        return x_overlap && y_overlap;
    }

    /**
     * Resolves an AABB collision by returning a vector representing
     * the distance the collided movable AABB should be moved. This
     * function assumes that the objects are colliding, and resolves
     * by finding the smallest distance to move.
     *
     * @tparam val_t value type
     * @param mv movable AABB
     * @param ob static obstacle AABB
     * @return vector representing resolution movement
     */
    template<typename val_t>
    nrg::vector<val_t> resolve_aabb_collide(
        const nrg::rect<val_t> &mv,
        const nrg::rect<val_t> &ob
    ) {
        // Move rectangle mv to resolve collision on rectangle ob
        // on an axis direction
        val_t d[4]{
            static_cast<val_t>(ob.x() + ob.width() - mv.x()),  // right
            static_cast<val_t>(mv.x() + mv.width() - ob.x()),  // left
            static_cast<val_t>(ob.y() + ob.height() - mv.y()), // down
            static_cast<val_t>(mv.y() + mv.height() - ob.y())  // up
        };
        int i = 0;
        for (int j = 1; j < 4; ++j) {
            if (d[j] < d[i]) { i = j; }
        }
        switch (i) {
            case 0:
                return {d[0], 0};
            case 1:
                return {-d[1], 0};
            case 2:
                return {0, d[2]};
            case 3:
                return {0, -d[3]};
            default:
                return {};
        }
    }

    /**
     * Determines whether moving an AABB from a start to final
     * position will result in a collision with a static obstacle.
     *
     * Used to determine whether the robot will collide with the
     * object for a certain movement.
     *
     * @tparam val_t value type
     * @param r robot initial position
     * @param c robot target position center
     * @param s object bounding box
     * @return true if a collision occurs
     */
    template<typename val_t>
    bool swept_collide(
        const nrg::rect<val_t> &r,
        const nrg::rect<val_t> &s,
        const nrg::vector<val_t> &c
    ) {
        val_t x_entry_inv, y_entry_inv;
        val_t x_exit_inv, y_exit_inv;
        nrg::vector<val_t> delta = c - r.center();
        if (delta.x() > 0) {
            x_entry_inv = s.x() - (r.x() + r.width());
            x_exit_inv = (s.x() + s.width()) - r.x();
        } else {
            x_entry_inv = (s.x() + s.width()) - r.x();
            x_exit_inv = s.x() - (r.x() + r.width());
        }
        if (delta.y() > 0) {
            y_entry_inv = s.y() - (r.y() + r.height());
            y_exit_inv = (s.y() + s.height()) - r.y();
        } else {
            y_entry_inv = (s.y() + s.height()) - r.y();
            y_exit_inv = s.y() - (r.y() + r.height());
        }
        val_t x_entry, y_entry;
        val_t x_exit, y_exit;
        if (delta.x() == 0) {
            x_entry = -std::numeric_limits<val_t>::infinity();
            x_exit = std::numeric_limits<val_t>::infinity();
        } else {
            x_entry = x_entry_inv / delta.x();
            x_exit = x_exit_inv / delta.x();
        }
        if (delta.y() == 0) {
            y_entry = -std::numeric_limits<val_t>::infinity();
            y_exit = std::numeric_limits<val_t>::infinity();
        } else {
            y_entry = y_entry_inv / delta.y();
            y_exit = y_exit_inv / delta.y();
        }
        val_t entry_time = std::max(x_entry, y_entry);
        val_t exit_time = std::min(x_exit, y_exit);
        return !(
            entry_time > exit_time ||
            x_entry < 0 && y_entry < 0 ||
            x_entry >= 1 ||
            y_entry >= 1
        );
    }

    template<typename val_t>
    bool swept_collide(
        const nrg::rect<val_t> &r0,
        const nrg::rect<val_t> &r1,
        const nrg::rect<val_t> &ob
    ) { return swept_collide(r0, ob, r1.center()); }

}

nrg::dir invert_dir(nrg::dir dir);

nrg::dir rotate_cw(nrg::dir dir);

bool is_dir_hor(nrg::dir dir);

bool is_dir_ver(nrg::dir dir);

#endif //MINOTAUR_CPP_ALGORITHM_H
