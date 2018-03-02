#include "vector2i.h"

Vector2i::Vector2i(int xp, int yp) : m_y(xp), m_x(yp) {
}

Vector2i::Vector2i() : Vector2i(0, 0) {
}

std::ostream &Vector2i::operator<<(std::ostream &os) {
    return os << '(' << m_y << ", " << m_x << ')';
}

Vector2i Vector2i::operator+(const Vector2i &other) {
    return Vector2i(m_y + other.m_y, m_x + other.m_x);
}

Vector2i Vector2i::operator-(const Vector2i &other) {
    return Vector2i(m_y - other.m_y, m_x - other.m_x);
}

Vector2i &Vector2i::operator+=(const Vector2i &other) {
    m_y += other.m_y;
    m_x += other.m_x;
    return *this;
}

Vector2i &Vector2i::operator-=(const Vector2i &other) {
    m_y -= other.m_y;
    m_x -= other.m_x;
    return *this;
}

Vector2i Vector2i::operator*(const int a) {
    return Vector2i(m_y * a, m_x * a);
}

const std::string Vector2i::toString() {
    return "(" + std::to_string(m_y) + ", " + std::to_string(m_x) + ")";
}