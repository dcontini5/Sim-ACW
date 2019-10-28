#include "Vector2f.h"
#define _USE_MATH_DEFINES
#include <math.h>

Vector2f::Vector2f(void)
{
}

Vector2f::Vector2f(float x, float y)
{
	m_x = x;
	m_y = y;
}

Vector2f::~Vector2f(void)
{
}

void Vector2f::Set(float x, float y)
{
	m_x = x;
	m_y = y;
}

float Vector2f::GetX() const
{
	return m_x;
}

float Vector2f::GetY() const
{
	return m_y;
}

Vector2f Vector2f::add(const Vector2f &vec) const
{
	return Vector2f(m_x + vec.GetX(), m_y + vec.GetY());
}

Vector2f Vector2f::subtract(const Vector2f &vec) const
{
	return Vector2f(m_x - vec.GetX(), m_y - vec.GetY());
}

Vector2f Vector2f::mult(float n) const
{
	return Vector2f(m_x*n, m_y*n);
}

Vector2f Vector2f::divide(float n) const
{
	return Vector2f(m_x/n, m_y/n);
}

float Vector2f::dot(const Vector2f &vec) const
{
	return m_x*vec.GetX() + m_y*vec.GetY();
}

float Vector2f::length() const
{
	return sqrt(m_x*m_x + m_y*m_y);
}

float Vector2f::distance(const Vector2f &vec) const
{
	return subtract(vec).length();
}

Vector2f Vector2f::normalise()
{
	float len = length();
	Set(m_x/len, m_y/len);
	return *this;
}


Vector2f operator+ (const Vector2f &lhs, const Vector2f &rhs)
{
	return lhs.add(rhs);
}

Vector2f operator- (const Vector2f &lhs, const Vector2f &rhs)
{
	return lhs.subtract(rhs);
}

Vector2f operator* (const Vector2f &lhs, float n)
{
	return lhs.mult(n);
}

Vector2f operator* (float n, const Vector2f &rhs)
{
	return rhs.mult(n);
}

Vector2f operator/ (const Vector2f &lhs, float n)
{
	return lhs.divide(n);
}