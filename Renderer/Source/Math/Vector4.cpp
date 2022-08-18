//
// Created by james on 02/06/2022.
//

#include "Math/Mathf.h"
#include "Math/Vector4.h"

const Vector4 Vector4::Forward	= Vector4(0, 0, 1);
const Vector4 Vector4::Backward = Vector4(0, 0, -1);
const Vector4 Vector4::Up		= Vector4(0, 1, 0);
const Vector4 Vector4::Down		= Vector4(0, -1, 0);
const Vector4 Vector4::Left		= Vector4(-1, 0, 0);
const Vector4 Vector4::Right	= Vector4(1, 0, 0);
const Vector4 Vector4::One		= Vector4(1, 1, 1, 1);
const Vector4 Vector4::Zero		= Vector4(0, 0, 0, 0);

Vector4::Vector4(float X, float Y, float Z, float W) : x(X), y(Y), z(Z), w(W)
{
}

Vector4::Vector4(const Vector3& other) : x(other.x), y(other.y), z(other.z), w(0.0f)
{
}

Vector4::Vector4(const Vector4& other) : x(other.x), y(other.y), z(other.z), w(0.0f)
{
}


float& Vector4::operator[](int i)
{
    return (&x)[i];
}

const float& Vector4::operator[](int i)const
{
    return (&x)[i];
}

bool Vector4::operator==(const Vector4& other)const
{
    return Mathf::IsEqual(x, other.x) && Mathf::IsEqual(y, other.y) && Mathf::IsEqual(z, other.z) && Mathf::IsEqual(w, other.w);
}

bool Vector4::operator!=(const Vector4& other)const
{
    return !Mathf::IsEqual(x, other.x) && !Mathf::IsEqual(y, other.y) && !Mathf::IsEqual(z, other.z) && !Mathf::IsEqual(w, other.w);
}

Vector4& Vector4::operator=(float s)
{
    x = s; y = s; z = s; w = s;
    return *this;
}

Vector4& Vector4::operator=(const Vector4& other)
{
    x = other.x; y = other.y; z = other.z; w = other.w;
    return *this;
}

Vector4& Vector4::operator+=(const Vector4& other)
{
    x += other.x; y += other.y; z += other.z; w += other.w;
    return *this;
}

Vector4& Vector4::operator-=(const Vector4& other)
{
    x -= other.x; y -= other.y; z -= other.z; w -= other.w;
    return *this;
}

Vector4& Vector4::operator*=(float s)
{
    x *= s; y *= s; z *= s; w *= s;
    return *this;
}

Vector4& Vector4::operator/=(float s)
{
    x /= s; y /= s; z /= s; w /= s;
    return *this;
}

Vector4 Vector4::operator+(const Vector4& other)const
{
    return {x + other.x, y + other.y, z + other.z, w + other.w};
}

Vector4 Vector4::operator-(const Vector4& other)const
{
    return {x - other.x, y - other.y, z - other.z, w - other.w};
}

Vector4 Vector4::operator*(float s)const
{
    return {x * s, y * s, z * s, w * s};
}

Vector4 Vector4::operator/(float s)const
{
    float recip = 1.0f / s;
    return {x * recip, y * recip, z * s, w * recip};
}

Vector4 Vector4::operator-()const
{
    return {-x, -y, -z, -w};
}

float Vector4::Magnitude()const
{
    return Mathf::Sqrt(x * x + y * y + z * z + w * w);
}

float Vector4::SqrMagnitude()const
{
    return x * x + y * y + z * z + w * w;
}

Vector4& Vector4::Normalized()
{
    float sqr = x * x + y * y + z * z + w * w;
    if (Mathf::IsZero(sqr))
    {
        x = 0.0f; y = 0.0f; z = 0.0f; w = 0.0f;
        return *this;
    }

    sqr = Mathf::RecipSqrt(sqr);
    x *= sqr; y *= sqr; z *= sqr; w *= sqr;
    return *this;
}

float Vector4::Angle(const Vector4& a, const Vector4& b)
{
    return Mathf::Acos((a.x * b.x + a.y * b.y + a.z * b.z + a.w * b.w) / Mathf::Sqrt(a.SqrMagnitude() * b.SqrMagnitude()));
}

float Vector4::Dot(const Vector4& a, const Vector4& b)
{
    return a.x * b.x + a.y * b.y + a.z * b.z + a.w * b.w;
}

float Vector4::Distance(const Vector4& a, const Vector4& b)
{
    return (b - a).Magnitude();
}

float Vector4::DistanceSquared(const Vector4& a, const Vector4& b)
{
    return (b - a).SqrMagnitude();
}

Vector4 Vector4::Clamp(const Vector4& v, const Vector4& min, const Vector4& max)
{
    return {Mathf::Clamp(v.x, min.x, max.x),
                   Mathf::Clamp(v.y, min.y, max.y),
                   Mathf::Clamp(v.z, min.z, max.z),
                   Mathf::Clamp(v.w, min.w, max.w)};
}

Vector4 Vector4::ClampLength(const Vector4& v, float s)
{
    float sqr = (v.x * v.x + v.y * v.y, v.z * v.z, v.w * v.w);
    if (sqr > s * s)
    {
        return (v / Mathf::Sqrt(sqr)) * s;
    }

    // This should force return a copy i think.
    return v;
}

Vector4 Vector4::Max(const Vector4& a, const Vector4& b)
{
    return {Mathf::Max(a.x, b.x), Mathf::Max(a.y, b.y), Mathf::Max(a.z, b.z), Mathf::Max(a.w, b.w)};
}

Vector4 Vector4::Min(const Vector4& a, const Vector4& b)
{
    return {Mathf::Min(a.x, b.x), Mathf::Min(a.y, b.y), Mathf::Min(a.z, b.z), Mathf::Min(a.w, b.w)};
}

void Vector4::OrthoNormalize(Vector4& n, Vector4& tangent)
{
    n.Normalized();
    tangent = (tangent - Dot(tangent, n) * n).Normalized();
}

Vector4 Vector4::Normalize(const Vector4& v)
{
    float sqr = v.x * v.x + v.y * v.y + v.z * v.z + v.w * v.w;

    if (Mathf::IsZero(sqr))
    {
        return Vector4::Zero;
    }
    sqr = Mathf::RecipSqrt(sqr);
    return v * sqr;
}

Vector4 Vector4::Project(const Vector4& a, const Vector4& b)
{
    return Dot(a, b) * b;
}

Vector4 Vector4::Reflect(const Vector4& dir, const Vector4& n)
{
    return dir - 2.0f * (Dot(dir, n) * n);
}

Vector4 Vector4::Lerp(const Vector4& a, const Vector4& b, float t)
{
    return {Mathf::Lerp(a.x, b.x, t), Mathf::Lerp(a.y, b.y, t), Mathf::Lerp(a.z, b.z, t), Mathf::Lerp(a.w, b.w, t)};

}

Vector4 Vector4::UnclampedLerp(const Vector4& a, const Vector4& b, float t)
{
    return {Mathf::UnclampedLerp(a.x, b.x, t), Mathf::UnclampedLerp(a.y, b.y, t), Mathf::UnclampedLerp(a.z, b.z, t), Mathf::UnclampedLerp(a.w, b.w, t)};
}

Vector4 Vector4::SmoothStep(const Vector4& min, const Vector4& max, float t)
{
    return { Mathf::SmoothStepKP(min.x, max.x, t),
                    Mathf::SmoothStepKP(min.y, max.y, t),
                    Mathf::SmoothStepKP(min.z, max.z, t),
                    Mathf::SmoothStepKP(min.w, max.w, t)};
}

Vector4 Vector4::CatmullRom(const Vector4& a, const Vector4& b, const Vector4& c, const Vector4& d, float t)
{
    float tSqr = t * t;
    float tCubed = tSqr * t;
    return	(0.5f * (2.0f * b + (c - a) * t +
                       (2.0f * a - 5.0f * b + 4.0f * c - d) * tSqr +
                       (3.0f * b - a - 3.0f * c + b) * tCubed));
}

Vector4 Vector4::Bezier(const Vector4& a, const Vector4& b, const Vector4& c, float t)
{
    t = Mathf::Clamp01(t);
    float	oneMinusT = 1.0f - t;
    return	oneMinusT * oneMinusT * a +
              2.0f * oneMinusT * t * b +
              t * t * c;
}

Vector4 Vector4::BezierCubic(const Vector4& a, const Vector4& b, const Vector4& c, const Vector4& d, float t)
{
    t = Mathf::Clamp01(t);
    float	oneMinusT = 1.0f - t;
    return	oneMinusT * oneMinusT * oneMinusT * a +
              3.0f * oneMinusT * oneMinusT * t * b +
              3.0f * oneMinusT * t * t * c +
              t * t * t * d;
}

Vector4 Vector4::RoundToInt(const Vector4& a)
{
    return {(float)(int)(a.x), (float)(int)(a.y), (float)(int)(a.z), (float)(int)(a.x)};
}
