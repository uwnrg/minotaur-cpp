#ifndef MINOTAUR_CPP_2DARRAY_H
#define MINOTAUR_CPP_2DARRAY_H

#include <cstddef>

template<typename val_t, typename size_t>
class __array2d_access {
public:

    __array2d_access(val_t *sub_arr, size_t len)
        : m_sub_arr(sub_arr),
          m_len(len) {}

    val_t &operator[](size_t t) {
        return m_sub_arr[t];
    }

private:
    val_t *m_sub_arr;
    size_t m_len;
};

template<typename val_t, typename size_t = std::size_t>
class array2d {
public:

    array2d(size_t x, size_t y)
        : m_x(x),
          m_y(y) {
        make_array(x, y);
    }

    array2d(std::initializer_list< std::initializer_list<val_t> > l) {
        m_x = l.size();
        m_y = l.begin()->size();
        make_array(m_x, m_y);

        int i = 0;
        for (auto &array : l) {
            int j = 0;
            for (auto &e : array) {
                m_arr[i][j] = std::move(e);
                j++;
            }
            i++;
        }
    }

    array2d(array2d<val_t, size_t> &&arr) noexcept
        : m_x(arr.m_x),
          m_y(arr.m_y),
          m_arr(arr.m_arr) {
        arr.m_x = 0;
        arr.m_y = 0;
        arr.m_arr = nullptr;
    }

    ~array2d() {
        if (m_arr) {
            delete_array();
        }
    }

    size_t x() const {
        return m_x;
    }

    size_t y() const {
        return m_y;
    }

    size_t xy() const {
        return m_x*m_y;
    }

    array2d<val_t, size_t> &operator=(array2d<val_t, size_t> &&arr) noexcept {
        m_x = arr.m_x;
        m_y = arr.m_y;
        m_arr = arr.m_arr;
        arr.m_x = 0;
        arr.m_y = 0;
        arr.m_arr = nullptr;
    };

    __array2d_access<val_t, size_t> operator[](size_t x) {
        return __array2d_access<val_t, size_t>(m_arr[x], m_y);
    };

    // Disable copy constructor and assignment
    array2d(const array2d<val_t, size_t> &) = delete;

    array2d<val_t, size_t> &operator=(const array2d<val_t, size_t> &) = delete;

private:
    void make_array(int x, int y) {
        if (m_arr) {
            delete_array();
        }
        m_arr = new val_t *[x];
        for (int tx = 0; tx < x; ++tx) {
            m_arr[tx] = new val_t[y];
        }
    }

    void delete_array() noexcept {
        for (size_t x = 0; x < m_x; ++x) {
            delete[] m_arr[x];
        }
        delete[] m_arr;
    }

    val_t **m_arr = nullptr;
    size_t m_x = 0;
    size_t m_y = 0;
};

#endif //MINOTAUR_CPP_2DARRAY_H
