#include "vector2i.h"

Vector2i::Vector2i(int xp, int yp) : x_comp(xp), y_comp(yp) {
}

Vector2i::Vector2i() : Vector2i(0, 0) {
}

std::ostream &Vector2i::operator<<(std::ostream &os) {
    return os << to_string();
}

Vector2i Vector2i::operator+(const Vector2i &other) {
    return Vector2i(x_comp + other.x_comp, y_comp + other.y_comp);
}

Vector2i Vector2i::operator-(const Vector2i &other) {
    return Vector2i(x_comp - other.x_comp, y_comp - other.y_comp);
}

Vector2i &Vector2i::operator+=(const Vector2i &other) {
    x_comp += other.x_comp;
    y_comp += other.y_comp;
    return *this;
}

Vector2i &Vector2i::operator-=(const Vector2i &other) {
    x_comp -= other.x_comp;
    y_comp -= other.y_comp;
    return *this;
}

Vector2i Vector2i::operator*(const int a) {
    return Vector2i(x_comp * a, y_comp * a);
}

std::string Vector2i::to_string() {
    return "(" + std::to_string(x_comp) + ", " + std::to_string(y_comp) + ")";
}
