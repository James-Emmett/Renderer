//
// Created by james on 02/06/2022.
//

#include "Math/Vector2.h"
#include "Math/Mathf.h"

const Vector2 Vector2::Up		= Vector2(0,1);
const Vector2 Vector2::Down		= Vector2(0,-1);
const Vector2 Vector2::Left		= Vector2(-1, 0);
const Vector2 Vector2::Right	= Vector2(1,0);
const Vector2 Vector2::One		= Vector2(1,1);
const Vector2 Vector2::Zero		= Vector2(0,0);

Vector2::Vector2(float X, float Y) : x(X), y(Y)
{
}

Vector2::Vector2(const Vector2& other) : x(other.x), y(other.y)
{
}

float& Vector2::operator[](int i)
{
    return (&x)[i];
}

const float& Vector2::operator[](int i)const
{
    return (&x)[i];
}

bool Vector2::operator==(const Vector2& other)const
{
    return x == other.x && y == other.y;
}

bool Vector2::operator!=(const Vector2& other)const
{
    return x != other.x && y != other.y;
}

Vector2& Vector2::operator=(const float s)
{
    x = s; y = s;
    return *this;
}

Vector2& Vector2::operator=(const Vector2& other)
{
    x = other.x; y = other.y;
    return *this;
}

Vector2& Vector2::operator+=(const Vector2& other)
{
    x += other.x; y += other.y;
    return *this;
}

Vector2& Vector2::operator-=(const Vector2& other)
{
    x -= other.x; y -= other.y;
    return *this;
}

Vector2& Vector2::operator*=(float s)
{
    x *= s; y *= s;
    return *this;
}

Vector2& Vector2::operator/=(float s)
{
    x /= s; y /= s;
    return *this;
}

Vector2  Vector2::operator+(const Vector2& other)const
{
    return {x + other.x, y + other.y};
}

Vector2  Vector2::operator-(const Vector2& other)const
{
    return {x - other.x, y - other.y};
}

Vector2  Vector2::operator*(float s)const
{
    return {x * s, y * s};
}

Vector2  Vector2::operator/(float s)const
{
    float recip = 1 / s;
    return {x * recip, y * recip};
}

Vector2  Vector2::operator-()const
{
    return {-x, -y};
}

float Vector2::Magnitude() const
{
    return Mathf::Sqrt(x * x + y * y);
}

float Vector2::SqrMagnitude() const
{
    return x * x + y * y;
}

Vector2& Vector2::Normalized()
{
    float sqr = x * x + y * y;
    if (Mathf::IsZero(sqr))
    {
        x = 0.0f; y = 0.0f;
        return *this;
    }

    sqr = Mathf::RecipSqrt(sqr);
    x *= sqr; y *= sqr;
    return *this;
}

float Vector2::Angle(const Vector2& a, const Vector2& b)
{
    float d = Mathf::Sqrt(a.SqrMagnitude() * b.SqrMagnitude());
    if (Mathf::IsZero(d)) { return 0; }

    return Mathf::Acos(Mathf::Clamp((a.x * b.x + a.y * b.y) / d, -1.0f, 1.0f));
}

float Vector2::AngleDifference(const Vector2& a, const Vector2& b)
{
    return Mathf::Atan2(b.y - a.y, b.x - a.x);
}

float Vector2::Dot(const Vector2& a, const Vector2& b)
{
    return a.x * b.x + a.y * b.y;
}

Vector2 Vector2::Perpendicular(const Vector2& v)
{
    return {-v.y, v.x};
}

float Vector2::Distance(const Vector2& a, const Vector2& b)
{
    return (b - a).Magnitude();
}

float Vector2::DistanceSquared(const Vector2& a, const Vector2& b)
{
    return (b - a).SqrMagnitude();
}

Vector2 Vector2::Clamp(const Vector2& v, Vector2 min, Vector2 max)
{
    return	{ Mathf::Clamp(v.x, min.x, max.x),
              Mathf::Clamp(v.y, min.y, max.y)};
}

Vector2 Vector2::ClampLength(const Vector2& v, float s)
{
    float sqr = v.x * v.x + v.y * v.y;
    if (sqr < s * s)
    {
        return v;
    }

    // Normalise and adjust to s;
    return (v / Mathf::Sqrt(sqr)) * s;
}

Vector2 Vector2::Max(const Vector2& a, const Vector2& b)
{
    return {Mathf::Max(a.x, b.x), Mathf::Max(a.y, b.y)};
}

Vector2 Vector2::Min(const Vector2& a, const Vector2& b)
{
    return {Mathf::Min(a.x, b.x), Mathf::Min(a.y, b.y)};
}

void Vector2::OrthoNormalize(Vector2& n, Vector2& tangent)
{
    n.Normalized();
    tangent = (tangent - Project(tangent, n)).Normalized();
}

Vector2 Vector2::Normalize(const Vector2& v)
{
    float sqr = (v.x * v.x + v.y * v.y);
    if (Mathf::IsZero(sqr))
    {
        return Vector2::Zero;
    }

    sqr = Mathf::RecipSqrt(sqr);
    return {v.x * sqr, v.y * sqr};
}

Vector2 Vector2::Project(const Vector2& a, const Vector2& b)
{
    return Dot(a, b) * b;
}

Vector2 Vector2::Reflect(const Vector2& dir, const Vector2& n)
{
    return dir - (2.0f * (Dot(dir, n) * n));
}

Vector2 Vector2::Lerp(const Vector2& a, const Vector2& b, float t)
{
    return {Mathf::Lerp(a.x, b.x, t), Mathf::Lerp(a.y, b.y, t)};
}

Vector2 Vector2::UnclampedLerp(const Vector2& a, const Vector2& b, float t)
{
    return {Mathf::UnclampedLerp(a.x, b.y, t), Mathf::UnclampedLerp(a.y, b.y, t)};
}

void Vector2::Barycentric(const Vector2& a, const Vector2& b, const Vector2& c, const Vector2& p, float& u, float& v, float& w)
{
    // Get the 2 triangle edges and vector to the point
    Vector2 v0 = b - a, v1 = c - a, v2 = p - a;
    if ((v0 == v1) && (v1 == v2) && (v0 == v2)) { return; } //div 0

    float d00 = Dot(v0, v0);
    float d01 = Dot(v0, v1);
    float d11 = Dot(v1, v1);
    float d20 = Dot(v2, v0);
    float d21 = Dot(v2, v1);
    float d = 1.0f / (d00 * d11 - d01 * d01);

    u = (d11 * d20 - d01 * d21) * d;
    v = (d00 * d21 - d01 * d20) * d;
    w = 1.0f - u - v;
}

Vector2 Vector2::CatmullRom(const Vector2& a, const Vector2& b, const Vector2& c, const Vector2& d, float t)
{
    float tSqr = t * t;
    float tCubed = tSqr * t;
    return	(0.5f * (2.0f * b + (c - a) * t +
                       (2.0f * a - 5.0f * b + 4.0f * c - d) * tSqr +
                       (3.0f * b - a - 3.0f * c + b) * tCubed));
}

Vector2 Vector2::Bezier(const Vector2& a, const Vector2& b, const Vector2& c, float t)
{
    t = Mathf::Clamp01(t);
    float	oneMinusT = 1.0f - t;
    return	oneMinusT * oneMinusT * a +
              2.0f * oneMinusT * t * b +
              t * t * c;
}

Vector2 Vector2::BezierCubic(const Vector2& a, const Vector2& b, const Vector2& c, const Vector2& d, float t)
{
    t = Mathf::Clamp01(t);
    float	oneMinusT = 1.0f - t;
    return	oneMinusT * oneMinusT * oneMinusT * a +
              3.0f * oneMinusT * oneMinusT * t * b +
              3.0f * oneMinusT * t * t * c +
              t * t * t * d;
}

Vector2 Vector2::RoundToInt(const Vector2& a)
{
    return {(float)(int)a.x, (float)(int)a.y};
}