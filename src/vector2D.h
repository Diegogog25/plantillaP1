#ifndef VECTOR2D_H
#define VECTOR2D_H

#include <iostream>
#include <cmath>

/**
 * Vector bidimensional genérico.
 */
template<typename T = float>
class Vector2D
{
    T x, y;

public:
    Vector2D(T x, T y) : x(x), y(y) {}
    Vector2D() : Vector2D(0, 0) {}

    // Coordenadas del vector
    const T& getX() const { return x; }
    const T& getY() const { return y; }
    void setX(T nx) { x = nx; }
    void setY(T ny) { y = ny; }

    // Operadores básicos
    Vector2D operator+(const Vector2D& o) const { return { x + o.x, y + o.y }; }
    Vector2D operator-(const Vector2D& o) const { return { x - o.x, y - o.y }; }
    Vector2D& operator+=(const Vector2D& o) { x += o.x; y += o.y; return *this; }
    Vector2D& operator-=(const Vector2D& o) { x -= o.x; y -= o.y; return *this; }

    // Producto por escalar y división
    Vector2D operator*(T k) const { return { x * k, y * k }; }
    Vector2D& operator*=(T k) { x *= k; y *= k; return *this; }
    Vector2D operator/(T k) const { return { x / k, y / k }; }
    Vector2D& operator/=(T k) { x /= k; y /= k; return *this; }

    // Producto escalar
    T dot(const Vector2D& o) const { return x * o.x + y * o.y; }

    // Utilidades
    T sqrMagnitude() const { return x * x + y * y; }
    auto magnitude() const { return std::sqrt(static_cast<long double>(sqrMagnitude())); }

    // E/S
    friend std::ostream& operator<<(std::ostream& out, const Vector2D& v) {
        return out << '{' << v.x << ", " << v.y << '}';
    }
};

template<typename T>
inline Vector2D<T> operator*(T k, const Vector2D<T>& v) { return v * k; }

// Alias punto
using Point2D = Vector2D<>;

#endif // VECTOR2D_H
