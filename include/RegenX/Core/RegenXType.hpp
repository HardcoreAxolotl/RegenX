#pragma once
#ifndef REGENXAPP_REGENXTYPE_HPP
#define REGENXAPP_REGENXTYPE_HPP
#include "RegenXMath.hpp"
using regenx::math::matrix3;
namespace regenx::type
{
    template<typename T>
struct vector3
    {
        T x{};
        T y{};
        T z{};

        vector3 operator+(const vector3& other) const {
            return vector3{this->x + other.x, this->y + other.y, this->z + other.z};
        }
        vector3 operator-(const vector3& other) const {
            return vector3{this->x - other.x, this->y - other.y, this->z - other.z};
        }
        vector3 operator*(T scalar) const {
            return vector3{this->x * scalar, this->y * scalar, this->z * scalar};
        }
        vector3 operator/(T scalar) const {
            return vector3{this->x / scalar, this->y / scalar, this->z / scalar};
        }

        vector3& operator+=(const vector3& other) {
            return *this = *this + other;
        }
        vector3& operator-=(const vector3& other) {
            return *this = *this - other;
        }
        vector3& operator*=(T scalar) {
            return *this = *this * scalar;
        }
        vector3<T>& operator*=(const matrix3<T>& m) {
            vector3<T> t = m * (*this);  // use matrix3 × vector3 multiplication
            x = t.x;
            y = t.y;
            z = t.z;
            return *this;
        }
        vector3& operator/=(T scalar) {
            return *this = *this / scalar;
        }
    };

    template<typename T>
    struct vector2
    {
        T x{};
        T y{};

        vector2 operator+(const vector2& other) const {
            return vector2{this->x + other.x, this->y + other.y};
        }
        vector2 operator-(const vector2& other) const {
            return vector2{this->x - other.x, this->y - other.y};
        }
        vector2 operator*(T scalar) const {
            return vector2{this->x * scalar, this->y * scalar};
        }
        vector2 operator/(T scalar) const {
            return vector2{this->x / scalar, this->y / scalar};
        }

        vector2& operator+=(const vector2& other) {
            return *this = *this + other;
        }
        vector2& operator-=(const vector2& other) {
            return *this = *this - other;
        }
        vector2& operator*=(T scalar) {
            return *this = *this * scalar;
        }
        vector2& operator*=(const matrix3<T>& m) {
            vector3<T> hv{x, y, 1};
            vector3<T> t = m * hv;
            x = t.x;
            y = t.y;
            return *this;
        }
        vector2& operator/=(T scalar) {
            return *this = *this / scalar;
        }
    };
}
#endif //REGENXAPP_REGENXTYPE_HPP