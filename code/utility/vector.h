#ifndef VECTOR2I_H
#define VECTOR2I_H

#include <sstream>
#include <iostream>

#include <QPoint>

#include <opencv2/core/types.hpp>

#include "utility.h"

namespace nrg {

    /**
     * A coordinate point or vector quantity.
     *
     * @tparam val_t
     */
    template<typename val_t>
    class vector {
    public:
        vector() :
            m_x(0),
            m_y(0) {}

        vector(val_t x, val_t y) :
            m_x(x),
            m_y(y) {}

        vector(std::initializer_list<val_t> l) :
            m_x(l.begin()[0]),
            m_y(l.begin()[1]) {}

        template<typename u_val_t>
        vector(std::initializer_list<u_val_t> l) :
            m_x(static_cast<val_t>(l.begin()[0])),
            m_y(static_cast<val_t>(l.begin()[1])) {}

        vector(const QPoint &p) :
            m_x(static_cast<val_t>(p.x())),
            m_y(static_cast<val_t>(p.y())) {}

        template<typename u_val_t>
        vector(const vector<u_val_t> &p) :
            m_x(static_cast<val_t>(p.x())),
            m_y(static_cast<val_t>(p.y())) {}

        template<typename u_val_t>
        vector(const cv::Point_<u_val_t> &p) :
            m_x(static_cast<val_t>(p.x)),
            m_y(static_cast<val_t>(p.y)) {}

        val_t &x() {
            return m_x;
        }

        val_t &y() {
            return m_y;
        }

        val_t norm() const {
            return sqrt(norm_sq());
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

        vector<val_t> n() const {
            return *this / norm();
        }

        vector<val_t> n_inv() const {
            val_t n = norm();
            return {n / m_x, n / m_y};
        }

        vector<val_t> &operator=(std::initializer_list<val_t> l) {
            m_x = l.begin()[0];
            m_y = l.begin()[1];
            return *this;
        }

        template<typename u_val_t>
        vector<val_t> &operator=(std::initializer_list<u_val_t> l) {
            m_x = static_cast<val_t>(l.begin()[0]);
            m_y = static_cast<val_t>(l.begin()[1]);
            return *this;
        }

        template<typename u_val_t>
        vector<val_t> &operator=(const cv::Point_<u_val_t> &p) {
            m_x = static_cast<val_t>(p.x);
            m_y = static_cast<val_t>(p.y);
            return *this;
        }

        vector<val_t> operator+(const vector<val_t> &o) const {
            return {m_x + o.m_x, m_y + o.m_y};
        }

        vector<val_t> operator-(const vector<val_t> &o) const {
            return {m_x - o.m_x, m_y - o.m_y};
        }

        vector<val_t> &operator+=(const vector<val_t> &o) {
            m_x += o.m_x;
            m_y += o.m_y;
            return *this;
        }

        vector<val_t> &operator-=(const vector<val_t> &o) {
            m_x -= o.m_x;
            m_y -= o.m_y;
            return *this;
        }

        bool operator==(const vector<val_t> &o) const {
            return (m_x == o.m_x) && (m_y == o.m_y);
        }

        bool operator!=(const vector<val_t> &o) const {
            return (m_x != o.m_x) || (m_y != o.m_y);
        }

        template<
            typename scalar_t,
            typename = enable_if_arithmetic_t<scalar_t>
        >
        vector<val_t> operator*(scalar_t b) const {
            return {
                static_cast<val_t>(m_x * b),
                static_cast<val_t>(m_y * b)
            };
        };

        template<
            typename scalar_t,
            typename = enable_if_arithmetic_t<scalar_t>
        >
        vector<val_t> operator/(scalar_t b) const {
            return {
                static_cast<val_t>(m_x / b),
                static_cast<val_t>(m_y / b)
            };
        };

        val_t dot(const vector<val_t> &v) const {
            return m_x * v.m_x + m_y * v.m_y;
        }

        val_t cross(const vector<val_t> &w) const {
            return m_x * w.m_y - m_y * w.m_x;
        }

        template<typename u_val_t>
        friend std::ostream &operator<<(std::ostream &os, const vector<u_val_t> &v);

    private:
        val_t m_x;
        val_t m_y;
    };

    template<typename val_t>
    using path = std::vector<nrg::vector<val_t>>;

    template<typename u_val_t>
    std::ostream &operator<<(std::ostream &os, const nrg::vector<u_val_t> &v) {
        return os << '(' << v.m_x << ',' << ' ' << v.m_y << ')';
    }
}

typedef nrg::vector<int> vector2i;
typedef nrg::vector<float> vector2f;
typedef nrg::vector<double> vector2d;

typedef nrg::path<int> path2i;
typedef nrg::path<float> path2f;
typedef nrg::path<double> path2d;

#endif // VECTOR2I_H
