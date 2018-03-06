#ifndef VECTOR2I_H
#define VECTOR2I_H

#include <sstream>
#include <iostream>

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

    val_t &x() {
        return m_x;
    }

    val_t &y() {
        return m_y;
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
        typename = typename std::enable_if<
            std::is_arithmetic<scalar_t>::value
        >::type
    >
    vector2d<val_t> operator*(scalar_t b) {
        return {
            static_cast<val_t>(m_x * b),
            static_cast<val_t>(m_y * b)
        };
    };

    template<
        typename scalar_t,
        typename = typename std::enable_if<
            std::is_arithmetic<scalar_t>::value
        >::type
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

namespace std {

    template<typename val_t>
    inline string to_string(const vector2d<val_t> &v) {
        ostringstream os;
        os << v;
        return os.str();
    }

}

#endif // VECTOR2I_H
