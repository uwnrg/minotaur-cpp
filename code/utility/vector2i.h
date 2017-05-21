#ifndef VECTOR2I_H
#define VECTOR2I_H

#include <iostream>

class Vector2i
{
public:
    Vector2i();
    Vector2i(int xp, int yp);
    int x_comp;
    int y_comp;

	Vector2i operator+(const Vector2i& other);
	Vector2i operator-(const Vector2i& other);
	Vector2i& operator+=(const Vector2i& other);
	Vector2i& operator-=(const Vector2i& other);
	Vector2i operator*(const int a);

    std::ostream& operator<<(std::ostream& os);
};

#endif // VECTOR2I_H
