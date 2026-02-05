#include "Vec2.h"

//Constructeur
Vec2::Vec2() = default;

Vec2::Vec2(float x_, float y_)
	: x(x_), y(y_)
{

}

//Opération
Vec2 Vec2::operator+(const Vec2& other) const
{
	return Vec2(x + other.x, y + other.y);
}

Vec2 Vec2::operator-(const Vec2& other) const
{
	return Vec2(x - other.x, y - other.y);
}

Vec2 Vec2::operator*(float scalar) const
{
	return Vec2(x * scalar, y * scalar);
}

Vec2 Vec2::operator/(float scalar) const
{
	return Vec2(x / scalar, y / scalar);
}

Vec2& Vec2::operator+=(const Vec2& other)
{
	x += other.x;
	y += other.y;
	return *this;
}

Vec2& Vec2::operator-=(const Vec2& other)
{
	x -= other.x;
	y -= other.y;
	return *this;
}

Vec2& Vec2::operator*=(float scalar)
{
	x *= scalar;
	y *= scalar;
	return *this;
}


//Maths
//Connaître la distance exacte
float Vec2::lenght() const
{
	return std::sqrt(x * x + y * y);
}

//Optimisation pour comparer des distances
float Vec2::lenghtSquared() const
{
	return x * x + y * y;
}


Vec2 Vec2::normalized() const
{
	float len = lenght();
	if (len <= 0.0001f)
		return Vec2(0.f, 0.f);

	return Vec2(x / len, y / len);
}

//Transformer le vecteur en une direction
void Vec2::normalize()
{
	float len = lenght();
	if (len <= 0.0001f)
		return;

	x /= len;
	y /= len;
}

float Vec2::distance(const Vec2& a, const Vec2& b)
{
	return (a - b).lenght();
}

float Vec2::distanceSquared(const Vec2& a, const Vec2& b)
{
	return (a - b).lenghtSquared();
}


//Vérifier si un objet est à l'arrê
bool Vec2::isZero(float epsilon) const
{
	return std::abs(x) <= epsilon && std::abs(y) <= epsilon;
}
