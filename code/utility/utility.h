#ifndef MINOTAUR_UTILITY_H
#define MINOTAUR_UTILITY_H

#include <memory>
#include <type_traits>
#include <climits>

namespace std {

    template<typename T, typename... Args>
    std::unique_ptr<T> make_unique(Args &&... args) {
        return std::unique_ptr<T>(new T(std::forward<Args>(args)...));
    };

}

template<typename T>
struct enable_if_arithmetic {
    typedef typename std::enable_if<
        std::is_arithmetic<T>::value
    >::type type;
};

template<typename T>
using enable_if_arithmetic_t = typename enable_if_arithmetic<T>::type;

template<
    typename number_t,
    typename = enable_if_arithmetic_t<number_t>
>
struct shrink_into {
    template<
        typename u_number_t,
        typename = enable_if_arithmetic_t<u_number_t>
    >
    number_t operator()(u_number_t u) {
        if (u > std::numeric_limits<number_t>::max()) {
            return std::numeric_limits<number_t>::max();
        } else if (u < std::numeric_limits<number_t>::min()) {
            return std::numeric_limits<number_t>::min();
        }
        return static_cast<number_t>(u);
    }
};

#endif
