//
// Created by james on 02/06/2022.
//

#include "Math/Vector3.h"
#include "Math/Mathf.h"

const Vector3 Vector3::Forward	= Vector3(0, 0, 1);
const Vector3 Vector3::Backward = Vector3(0, 0, -1);
const Vector3 Vector3::Up		= Vector3(0, 1, 0);
const Vector3 Vector3::Down		= Vector3(0, -1, 0);
const Vector3 Vector3::Left		= Vector3(-1, 0, 0);
const Vector3 Vector3::Right	= Vector3(1, 0, 0);
const Vector3 Vector3::One		= Vector3(1, 1, 1);
const Vector3 Vector3::Zero		= Vector3(0, 0, 0);

Vector3::Vector3(float X, float Y, float Z) : x(X), y(Y), z(Z)
{
}

Vector3::Vector3(const Vector2 &other) : x(other.x), y(other.y), z(0.0f)
{
}

Vector3::Vector3(const Vector3 &other)  : x(other.x), y(other.y), z(other.z)
{
}

float& Vector3::operator[](int i)
{
    return (&x)[i];
}

const float& Vector3::operator[](int i)const
{
    return (&x)[i];
}

bool Vector3::operator==(const Vector3& other)const
{
    return Mathf::IsEqual(x, other.x) && Mathf::IsEqual(y, other.y) && Mathf::IsEqual(z, other.z);
}

bool Vector3::operator!=(const Vector3& other)const
{
    return !Mathf::IsEqual(x, other.x) && !Mathf::IsEqual(y, other.y) && !Mathf::IsEqual(z, other.z);
}

Vector3& Vector3::operator=(const float s)
{
    x = s; y = s; z = s;
    return *this;
}

Vector3& Vector3::operator=(const Vector3& other)
{
    x = other.x; y = other.y; z = other.z;
    return *this;
}

Vector3& Vector3::operator+=(const Vector3& other)
{
    x += other.x; y += other.y; z += other.z;
    return *this;
}

Vector3& Vector3::operator-=(const Vector3& other)
{
    x -= other.x; y -= other.y; z -= other.z;
    return *this;
}

Vector3& Vector3::operator*=(float s)
{
    x *= s; y *= s; z *= s;
    return *this;
}

Vector3& Vector3::operator/=(float s)
{
    x /= s; y /= s; z /= s;
    return *this;
}

Vector3  Vector3::operator+(const Vector3& other)const
{
    return {x + other.x, y + other.y, z + other.z};
}

Vector3  Vector3::operator-(const Vector3& other)const
{
    return {x - other.x, y - other.y, z - other.z};
}

Vector3  Vector3::operator*(float s)const
{
    return {x * s, y * s, z * s};
}

Vector3  Vector3::operator/(float s)const
{
    float recip = 1.0f / s;
    return {x * recip, y * recip, z * recip};
}

Vector3  Vector3::operator-()const
{
    return {-x, -y, -z};
}

float Vector3::Magnitude()const
{
    float sqrMag = x*x + y*y + z*z;
    if(Mathf::IsZero(sqrMag))
    {
        return 0.0f;
    }

    return Mathf::Sqrt(sqrMag);
}

float Vector3::SqrMagnitude()const
{
    return x*x + y*y + z*z;
}

Vector3& Vector3::Normalized()
{
    float sqrMag = x*x + y*y + z*z;
    if(Mathf::IsZero(sqrMag))
    {
        x = 0.0f; y = 0.0f; z = 0.0f;
        return *this;
    }

    sqrMag = Mathf::RecipSqrt(sqrMag);
    x *= sqrMag; y *= sqrMag; z *= sqrMag;

    return *this;
}

float Vector3::Angle(const Vector3& a, const Vector3& b)
{
    float d = Mathf::Sqrt(a.SqrMagnitude() * b.SqrMagnitude());
    if (Mathf::IsZero(d)) { return 0; }

    return Mathf::Acos(Mathf::Clamp((a.x * b.x + a.y * b.y + a.z * b.z) / d, -1.0f, 1.0f));
}

float Vector3::Dot(const Vector3& a, const Vector3& b)
{
    return a.x * b.x + a.y * b.y + a.z * b.z;
}

Vector3 Vector3::Cross(const Vector3& a, const Vector3& b)
{
    return {a.y * b.z - a.z * b.y,
            a.z * b.x - a.x * b.z,
            a.x * b.y - a.y * b.x};
}

float Vector3::Distance(const Vector3& a, const Vector3& b)
{
    return (b - a).Magnitude();
}

float Vector3::DistanceSquared(const Vector3& a, const Vector3& b)
{
    return (b - a).SqrMagnitude();
}

Vector3 Vector3::Clamp(const Vector3& other, const Vector3& min, const Vector3& max)
{
    return	{Mathf::Clamp(other.x, min.x, max.x),
             Mathf::Clamp(other.y, min.y, max.y),
             Mathf::Clamp(other.z, min.z, max.z)};
}

Vector3 Vector3::ClampLength(const Vector3& v, float s)
{
    float sqr = v.x * v.x + v.y * v.y + v.z * v.z;
    if (sqr < s * s)
    {
        return v;
    }

    // Normalise and adjust to s;
    return (v / Mathf::Sqrt(sqr)) * s;
}

Vector3 Vector3::Max(const Vector3& a, const Vector3& b)
{
    return {Mathf::Max(a.x, b.x), Mathf::Max(a.y, b.y), Mathf::Max(a.z, b.z)};
}

Vector3 Vector3::Min(const Vector3& a, const Vector3& b)
{
    return {Mathf::Min(a.x, b.x), Mathf::Min(a.y, b.y), Mathf::Min(a.z, b.z)};
}

void Vector3::OrthoNormalize(Vector3& n, Vector3& tangent)
{
    n.Normalized();
    tangent = (tangent - Project(tangent, n)).Normalized();
}

Vector3 Vector3::Normalize(const Vector3& v)
{
    float sqr = (v.x * v.x + v.y * v.y + v.z * v.z);
    if (Mathf::IsZero(sqr))
    {
        return Vector3::Zero;
    }

    sqr = Mathf::RecipSqrt(sqr);
    return {v.x * sqr, v.y * sqr, v.z * sqr};
}

Vector3 Vector3::Project(const Vector3& a, const Vector3& b)
{
    return Dot(a, b) * b;
}

Vector3 Vector3::Reflect(const Vector3& dir, const Vector3& n)
{
    return dir - (2.0f * (Dot(dir, n) * n));
}

Vector3 Vector3::Lerp(const Vector3& a, const Vector3& b, float t)
{
    return{ Mathf::Lerp(a.x, b.x, t),
            Mathf::Lerp(a.y, b.y, t),
            Mathf::Lerp(a.z, b.z, t)};
}

Vector3 Vector3::UnclampedLerp(const Vector3& a, const Vector3& b, float t)
{
    return{ Mathf::UnclampedLerp(a.x, b.y, t),
            Mathf::UnclampedLerp(a.y, b.y, t),
            Mathf::UnclampedLerp(a.z, b.z, t)};
}

void Vector3::Barycentric(const Vector3& a, const Vector3& b, const Vector3& c, const Vector3& p, float& u, float& v, float& w)
{
    // Get the 2 triangle edges and vector to the point
    Vector3 v0 = b - a, v1 = c - a, v2 = p - a;
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

Vector3 Vector3::CatmullRom(const Vector3& a, const Vector3& b, const Vector3& c, const Vector3& d, float t)
{
    float tSqr = t * t;
    float tCubed = tSqr * t;
    return	(0.5f * (2.0f * b + (c - a) * t +
                       (2.0f * a - 5.0f * b + 4.0f * c - d) * tSqr +
                       (3.0f * b - a - 3.0f * c + b) * tCubed));
}

Vector3 Vector3::Bezier(const Vector3& a, const Vector3& b, const Vector3& c, float t)
{
    t = Mathf::Clamp01(t);
    float	oneMinusT = 1.0f - t;
    return	oneMinusT * oneMinusT * a +
              2.0f * oneMinusT * t * b +
              t * t * c;
}

Vector3 Vector3::BezierCubic(const Vector3& a, const Vector3& b, const Vector3& c, const Vector3& d, float t)
{
    t = Mathf::Clamp01(t);
    float	oneMinusT = 1.0f - t;
    return	oneMinusT * oneMinusT * oneMinusT * a +
              3.0f * oneMinusT * oneMinusT * t * b +
              3.0f * oneMinusT * t * t * c +
              t * t * t * d;
}

Vector3 Vector3::RoundToInt(const Vector3& a)
{
    return Vector3((float)(int)(a.x), (float)(int)(a.y), (float)(int)(a.z));
}
