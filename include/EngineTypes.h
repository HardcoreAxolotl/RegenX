//
// Created by liad on 10/25/25.
//

#ifndef GAMEENGINE_ENGINETYPES_H
#define GAMEENGINE_ENGINETYPES_H
#pragma once

namespace RGNX2D {
    struct Vector2 {
        float x{};
        float y{};

        // --- Constructors ---
        Vector2() : x(0), y(0) {}
        Vector2(float x_, float y_) : x(x_), y(y_) {}

        // --- Comparison ---
        bool operator==(const Vector2& rhs) const { return x == rhs.x && y == rhs.y; }
        bool operator!=(const Vector2& rhs) const { return !(*this == rhs); }

        // --- Compound assignment operators ---
        Vector2& operator+=(const Vector2& rhs) { x += rhs.x; y += rhs.y; return *this; }
        Vector2& operator-=(const Vector2& rhs) { x -= rhs.x; y -= rhs.y; return *this; }
        Vector2& operator*=(float scalar) { x *= scalar; y *= scalar; return *this; }
        Vector2& operator/=(float scalar) { x /= scalar; y /= scalar; return *this; }

        // --- Unary operators ---
        Vector2 operator-() const { return {-x, -y}; }

        // --- Binary arithmetic operators ---
        Vector2 operator+(const Vector2& rhs) const { return {x + rhs.x, y + rhs.y}; }
        Vector2 operator-(const Vector2& rhs) const { return {x - rhs.x, y - rhs.y}; }
        Vector2 operator*(float scalar) const { return {x * scalar, y * scalar}; }
        Vector2 operator/(float scalar) const { return {x / scalar, y / scalar}; }
    };
}

#endif //GAMEENGINE_ENGINETYPES_H