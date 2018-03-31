#ifndef MINOTAUR_CPP_RECT_H
#define MINOTAUR_CPP_RECT_H

#include "vector.h"
#include "ray.h"

#include <opencv2/core/types.hpp>

namespace nrg {

    // Sides of a box ordered clockwise starting from the top
    enum dir {
        TOP = 0,
        RIGHT = 1,
        BOTTOM = 2,
        LEFT = 3,

        UP = 0,
        DOWN = 2
    };

    /**
     * Axis-aligned rectangle represented by its top-left corner,
     * with lowest (x, y) values, width, and height.
     *
     * @tparam val_t
     */
    template<typename val_t>
    class rect {
    public:

        rect() :
            m_x(0),
            m_y(0),
            m_w(0),
            m_h(0) {}

        rect(val_t w, val_t h) :
            m_x(0),
            m_y(0),
            m_w(w),
            m_h(h) {}

        rect(val_t x, val_t y, val_t w, val_t h) :
            m_x(x),
            m_y(y),
            m_w(w),
            m_h(h) {}

        template<typename u_val_t>
        rect(const rect<u_val_t> &r) :
            m_x(static_cast<val_t>(r.m_x)),
            m_y(static_cast<val_t>(r.m_y)),
            m_w(static_cast<val_t>(r.m_w)),
            m_h(static_cast<val_t>(r.m_h)) {}

        template<typename u_val_t>
        rect(const cv::Rect_<u_val_t> &r) :
            m_x(static_cast<val_t>(r.x)),
            m_y(static_cast<val_t>(r.y)),
            m_w(static_cast<val_t>(r.width)),
            m_h(static_cast<val_t>(r.height)) {}

        val_t &x() {
            return m_x;
        }

        val_t &y() {
            return m_y;
        }

        val_t &width() {
            return m_w;
        }

        val_t &height() {
            return m_h;
        }

        const val_t &x() const {
            return m_x;
        }

        const val_t &y() const {
            return m_y;
        }

        const val_t &width() const {
            return m_w;
        }

        const val_t &height() const {
            return m_h;
        }

        rect<val_t> operator+(const vector<val_t> &v) const {
            return {m_x + v.x(), m_y + v.y(), m_w, m_h};
        }

        rect<val_t> operator-(const vector<val_t> &v) const {
            return {m_x - v.x(), m_y - v.y(), m_w, m_h};
        }

        rect<val_t> &operator+=(const vector<val_t> &v) {
            m_x += v.x();
            m_y += v.y();
            return *this;
        }

        rect<val_t> &operator-=(const vector<val_t> &v) {
            m_x -= v.x();
            m_y -= v.y();
            return *this;
        }

        vector<val_t> tl() const {
            return {m_x, m_y};
        }

        vector<val_t> tr() const {
            return {m_x + m_w, m_y};
        }

        vector<val_t> bl() const {
            return {m_x, m_y + m_h};
        }

        vector<val_t> br() const {
            return {m_x + m_w, m_y + m_h};
        }

        vector<val_t> mt() const {
            return {m_x + m_w / 2, m_y};
        }

        vector<val_t> mb() const {
            return {m_x + m_w / 2, m_y + m_h};
        }

        vector<val_t> ml() const {
            return {m_x, m_y + m_h / 2};
        }

        vector<val_t> mr() const {
            return {m_x + m_w, m_y + m_h / 2};
        }

        vector<val_t> center() const {
            return {m_x + m_w / 2, m_y + m_h / 2};
        }

        ray<val_t> tltr() const {
            return {tl(), tr()};
        }

        ray<val_t> trbr() const {
            return {tr(), br()};
        }

        ray<val_t> brbl() const {
            return {br(), bl()};
        }

        ray<val_t> bltl() const {
            return {bl(), tl()};
        }

    private:
        val_t m_x;
        val_t m_y;
        val_t m_w;
        val_t m_h;
    };

}

typedef nrg::rect<int> rect2i;
typedef nrg::rect<float> rect2f;
typedef nrg::rect<double> rect2d;

#endif //MINOTAUR_CPP_RECT_H
