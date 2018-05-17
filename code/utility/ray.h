#ifndef MINOTAUR_CPP_RAY_H
#define MINOTAUR_CPP_RAY_H

#include "vector.h"

namespace nrg {

    /**
     * A directed line segment, represented by two points.
     * Segment starts from a and goes to b.
     * 
     * @tparam val_t
     */
    template<typename val_t>
    class ray {
    public:
        ray() :
            m_a(),
            m_b() {}

        ray(val_t a_x, val_t a_y, val_t b_x, val_t b_y) :
            m_a(a_x, a_y),
            m_b(b_x, b_y) {}

        ray(const vector<val_t> &a, const vector<val_t> &b) :
            m_a(a),
            m_b(b) {}

        template<typename v = vector<val_t>>
        ray(std::initializer_list<v> l) :
            m_a(l.begin()[0]),
            m_b(l.begin()[1]) {}

        vector<val_t> &a() {
            return m_a;
        }

        vector<val_t> &b() {
            return m_b;
        }

        const vector<val_t> &a() const {
            return m_a;
        }

        const vector<val_t> &b() const {
            return m_b;
        }

        vector<val_t> n_inv() const {
            return (m_b - m_a).n_inv();
        }

    private:
        vector<val_t> m_a;
        vector<val_t> m_b;
    };

}

typedef nrg::ray<int> ray2i;
typedef nrg::ray<float> ray2f;
typedef nrg::ray<double> ray2d;

#endif //MINOTAUR_CPP_RAY_H
