#ifndef VECTOR2I_H
#define VECTOR2I_H

#include <iostream>

class Vector2i {
public:
    Vector2i();
    Vector2i(int xp, int yp);

    int x_comp;
    int y_comp;

    /**
     * Component-wise vector addition.
     *
     * @param other
     * @return
     */
    Vector2i operator+(const Vector2i &other);

    /**
     * Component-wise vector subtraction.
     *
     * @param other
     * @return
     */
    Vector2i operator-(const Vector2i &other);

    Vector2i &operator+=(const Vector2i &other);
    Vector2i &operator-=(const Vector2i &other);

    /**
     * Calculates the cartesian product with
     * this vector and a scalar.
     *
     * @param a
     * @return
     */
    Vector2i operator*(const int a);

    std::ostream& operator<<(std::ostream& os);
	const std::string toString();
};

#endif // VECTOR2I_H
