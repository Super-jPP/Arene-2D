#pragma once
#include <cmath>

struct Vec2
{
	float x = 0.f;
	float y = 0.f;

	Vec2() = default;
	Vec2(float x_, float y_);

	Vec2 operator+(const Vec2& other) const;
	Vec2 operator-(const Vec2& other) const;
	Vec2 operator*(float scalar) const;
	Vec2 operator/(float scalar) const;

	Vec2& operator+=(const Vec2& other);
	Vec2& operator-=(const Vec2& other);
	Vec2& operator*=(float scalar);

	float lenght() const;
	float lenghtSquared() const;

	Vec2 normalized() const;
	void normalize();

	static float distance(const Vec2& a, const Vec2& b);
	static float distanceSquared(const Vec2& a, const Vec2& b);

	bool isZero(float epsilon = 0.0001f) const;
};