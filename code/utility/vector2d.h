#ifndef VECTOR2I_H
#define VECTOR2I_H

#include <sstream>
#include <iostream>

#include <opencv2/core/types.hpp>

#include "utility.h"

template<typename val_t>
class vector2d;

typedef vector2d<int> Vector2i;

template<typename val_t>
class vector2d {
public:

    vector2d() :
        m_x(0),
        m_y(0) {}

    vector2d(val_t x, val_t y) :
        m_x(x),
        m_y(y) {}

    vector2d(std::initializer_list<val_t> l) :
        m_x(l.begin()[0]),
        m_y(l.begin()[1]) {}

    template<typename u_val_t>
    vector2d(const cv::Point_<u_val_t> &p) :
        m_x(p.x),
        m_y(p.y) {}

    val_t &x() {
        return m_x;
    }

    val_t &y() {
        return m_y;
    }

    val_t norm_sq() const {
        return m_x * m_x + m_y * m_y;
    }

    const val_t &x() const {
        return m_x;
    }

    const val_t &y() const {
        return m_y;
    }

    vector2d<val_t> operator+(const vector2d<val_t> &o) const {
        return {m_x + o.m_x, m_y + o.m_y};
    }

    vector2d<val_t> operator-(const vector2d<val_t> &o) const {
        return {m_x - o.m_x, m_y - o.m_y};
    }

    vector2d<val_t> &operator+=(const vector2d<val_t> &o) {
        m_x += o.m_x;
        m_y += o.m_y;
        return *this;
    }

    vector2d<val_t> &operator-=(const vector2d<val_t> &o) {
        m_x -= o.m_x;
        m_y -= o.m_y;
        return *this;
    }

    template<
        typename scalar_t,
        typename = enable_if_arithmetic_t<scalar_t>
    >
    vector2d<val_t> operator*(scalar_t b) {
        return {
            static_cast<val_t>(m_x * b),
            static_cast<val_t>(m_y * b)
        };
    };

    template<
        typename scalar_t,
        typename = enable_if_arithmetic_t<scalar_t>
    >
    vector2d<val_t> operator/(scalar_t b) {
        return {
            static_cast<val_t>(m_x / b),
            static_cast<val_t>(m_y / b)
        };
    };

    template<typename u_val_t>
    friend std::ostream &operator<<(std::ostream &os, const vector2d<u_val_t> &v);

private:
    val_t m_x;
    val_t m_y;
};

template<typename u_val_t>
std::ostream &operator<<(std::ostream &os, const vector2d<u_val_t> &v) {
    return os << '(' << v.m_x << ',' << ' ' << v.m_y << ')';
}

template<typename val_t>
using path2d = std::vector<vector2d<val_t>>;

namespace std {

    template<typename val_t>
    inline string to_string(const vector2d<val_t> &v) {
        ostringstream os;
        os << v;
        return os.str();
    }

}

#endif // VECTOR2I_H
