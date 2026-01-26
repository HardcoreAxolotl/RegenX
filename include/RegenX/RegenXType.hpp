#include <cmath>

namespace regenx::type
{
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
        vector2& operator/=(T scalar) {
            return *this = *this / scalar;
        }
    };

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
        vector3& operator/=(T scalar) {
            return *this = *this / scalar;
        }
    };
}