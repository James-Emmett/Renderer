#pragma once
#include "Vector2.h"

class Vector3
{
public:
    static const Vector3 Forward;
    static const Vector3 Backward;
    static const Vector3 Up;
    static const Vector3 Down;
    static const Vector3 Left;
    static const Vector3 Right;
    static const Vector3 One;
    static const Vector3 Zero;

public:
    float x,y,z = 0.0f;

public:
    Vector3(float X = 0.0f, float Y = 0.0f, float Z = 0.0f);
    Vector3(const Vector2& other);
    Vector3(const Vector3& other);

public:
    //--Access Operations--
    float& operator[](int i);
    const float& operator[](int i)const;

    //--Comparison Operations--
    bool operator==(const Vector3& other)const;
    bool operator!=(const Vector3& other)const;

    //--Assignment Operations--
    Vector3& operator=(const float s);
    Vector3& operator=(const Vector3& other);
    Vector3& operator+=(const Vector3& other);
    Vector3& operator-=(const Vector3& other);
    Vector3& operator*=(float s);
    Vector3& operator/=(float s);

    //--Arthritic Operations--
    Vector3 operator+(const Vector3& other)const;
    Vector3 operator-(const Vector3& other)const;
    Vector3 operator*(float s)const;
    Vector3 operator/(float s)const;
    Vector3 operator-()const;

public:
    float Magnitude()const;
    float SqrMagnitude()const;
    Vector3& Normalized();

public:
    static float    Angle(const Vector3& a, const Vector3& b);
    static float    Dot(const Vector3& a, const Vector3& b);
    static Vector3	Cross(const Vector3& a, const Vector3& b);
    static float    Distance(const Vector3& a, const Vector3& b);
    static float    DistanceSquared(const Vector3& a, const Vector3& b);
    static Vector3  Clamp(const Vector3& other, const Vector3& min, const Vector3& max);
    static Vector3  ClampLength(const Vector3& v, float s);
    static Vector3  Max(const Vector3& a, const Vector3& b);
    static Vector3  Min(const Vector3& a, const Vector3& b);
    static void     OrthoNormalize(Vector3& n, Vector3& tangent);
    static Vector3  Normalize(const Vector3& v);
    static Vector3  Project(const Vector3& a, const Vector3& b);
    static Vector3  Reflect(const Vector3& dir, const Vector3& n);
    static Vector3  Lerp(const Vector3& a, const Vector3& b, float t);
    static Vector3	UnclampedLerp(const Vector3& a, const Vector3& b, float t);
    static void		Barycentric(const Vector3& a, const Vector3& b, const Vector3& c, const Vector3& p, float& u, float& v, float& w);
    static Vector3  CatmullRom(const Vector3& a, const Vector3& b, const Vector3& c, const Vector3& d, float t);
    static Vector3  Bezier(const Vector3& a, const Vector3& b, const Vector3& c, float t);
    static Vector3  BezierCubic(const Vector3& a, const Vector3& b, const Vector3& c, const Vector3& d, float t);
    static Vector3  RoundToInt(const Vector3& a);
};

inline Vector3 operator*(float s, const Vector3& other)
{
    return Vector3(s * other.x, s * other.y, s * other.z);
}