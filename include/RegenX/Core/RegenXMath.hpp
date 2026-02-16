//
// Created by liad on 04/02/2026.
//
#pragma once

#ifndef REGENXAPP_REGENXMATH_HPP
#define REGENXAPP_REGENXMATH_HPP
#include "RegenXType.hpp"  // vector2/3
#include <cmath>
#include <type_traits>

namespace regenx::math
{
    template<typename T>
    struct matrix3
    {
        static_assert(std::is_floating_point<T>::value,
                 "matrix3 only supports float or double types!");
        T m[3][3]{};

        static matrix3 identity() {
            matrix3 result{};
            for (int i = 0; i < 3; ++i) {
                result.m[i][i] = static_cast<T>(1);
            }
            return result;
        }

        static matrix3 translation(const type::vector2<T>& t)
        {
            matrix3 result = identity();
            result.m[0][2] = t.x;
            result.m[1][2] = t.y;
            return result;
        }

        static matrix3 rotation(T angle) {
            matrix3 result = identity();
            T radians = angle * static_cast<T>(M_PI) / static_cast<T>(180);
            T cosA = std::cos(radians);
            T sinA = std::sin(radians);
            result.m[0][0] = cosA;
            result.m[0][1] = -sinA;
            result.m[1][0] = sinA;
            result.m[1][1] = cosA;
            return result;
        }

        static matrix3 scaling(const type::vector2<T>& scale)
        {
            matrix3 result = identity();
            result.m[0][0] = scale.x;
            result.m[1][1] = scale.y;
            return result;
        }

        matrix3 operator+(const matrix3& other) const
        {
            matrix3 result{};
            for (int i = 0; i < 3; ++i) {
                for (int j = 0; j < 3; ++j) {
                    result.m[i][j] = m[i][j] + other.m[i][j];
                }
            }
            return result;
        }
        matrix3 operator-(const matrix3& other) const
        {
            matrix3 result{};
            for (int i = 0; i < 3; ++i) {
                for (int j = 0; j < 3; ++j) {
                    result.m[i][j] = m[i][j] - other.m[i][j];
                }
            }
            return result;
        }
        matrix3 operator*(const matrix3& other) const {
            matrix3 result{};
            for (int i = 0; i < 3; ++i) {
                for (int j = 0; j < 3; ++j) {
                    result.m[i][j] = m[i][0] * other.m[0][j] + m[i][1] * other.m[1][j] + m[i][2] * other.m[2][j];
                }
            }
            return result;
        }

        matrix3& operator+=(const matrix3& other) {
            return *this = *this + other;
        }
        matrix3& operator-=(const matrix3& other) {
            return *this = *this - other;
        }
        matrix3& operator*=(const matrix3& other) {
            return *this = *this * other;
        }

        T* operator[](std::size_t row) {
            return m[row];
        }
    };

    namespace vector
    {
        template<typename T>
        T dot(const type::vector2<T>& a, const type::vector2<T>& b) {
            return a.x * b.x + a.y * b.y;
        }

        template<typename T>
        T dot(const type::vector3<T>& a, const type::vector3<T>& b) {
            return a.x * b.x + a.y * b.y + a.z * b.z;
        }

        template<typename T>
            type::vector3<T> transform(const type::vector3<T>& v, const matrix3<T>& m) {
            return {
                m.m[0][0]*v.x + m.m[0][1]*v.y + m.m[0][2]*v.z,
                m.m[1][0]*v.x + m.m[1][1]*v.y + m.m[1][2]*v.z,
                m.m[2][0]*v.x + m.m[2][1]*v.y + m.m[2][2]*v.z
            };
        }

        template<typename T>
        type::vector2<T> transform(const type::vector2<T>& v, const matrix3<T>& m) {
            type::vector3<T> hv{v.x, v.y, 1};          // homogeneous vector
            type::vector3<T> t = transform(hv, m);     // use vector3 version
            return { t.x / t.z, t.y / t.z };                   // handle perspective if needed
        }
    }
}
#endif //REGENXAPP_REGENXMATH_HPP