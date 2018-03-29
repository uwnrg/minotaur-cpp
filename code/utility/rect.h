#ifndef MINOTAUR_CPP_RECT_H
#define MINOTAUR_CPP_RECT_H

#include "vector.h"

#include <opencv2/core/types.hpp>

namespace nrg {

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

    private:
        val_t m_x;
        val_t m_y;
        val_t m_w;
        val_t m_h;
    };

}

#endif //MINOTAUR_CPP_RECT_H
