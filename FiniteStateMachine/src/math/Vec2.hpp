#pragma once
#include <cmath>

struct Vec2
{
    float x = 0.f;
    float y = 0.f;

    Vec2() = default;
    Vec2(float x_, float y_) : x(x_), y(y_) {}

    Vec2 operator+(const Vec2& other) const
    {
        return Vec2{ x + other.x, y + other.y };
    }

    Vec2 operator-(const Vec2& other) const
    {
        return Vec2{ x - other.x, y - other.y };
    }

    Vec2 operator*(float s) const
    {
        return Vec2{ x * s, y * s };
    }

    float length() const
    {
        return std::sqrt(x * x + y * y);
    }

    Vec2 normalized() const
    {
        const float len = length();
        if (len <= 0.00001f)
            return Vec2{ 0.f, 0.f };
        return Vec2{ x / len, y / len };
    }

    static float distance(const Vec2& a, const Vec2& b)
    {
        return (b - a).length();
    }
};
