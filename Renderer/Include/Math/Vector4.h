#pragma once
#include "Vector3.h"

class Vector4
{
public:
    static const Vector4 Forward;
    static const Vector4 Backward;
    static const Vector4 Up;
    static const Vector4 Down;
    static const Vector4 Left;
    static const Vector4 Right;
    static const Vector4 One;
    static const Vector4 Zero;

public:
    float x, y, z, w;

public:
    Vector4(float X = 0, float Y = 0, float Z = 0, float W = 0);
    Vector4(const Vector3& other);
    Vector4(const Vector4& other);

public:
    //--Access Operations--
    float& operator[](int i);
    const float& operator[](int i)const;

    //--Comparison Operations--
    bool operator==(const Vector4& other)const;
    bool operator!=(const Vector4& other)const;

    //--Assignment Operations--
    Vector4& operator=(float s);
    Vector4& operator=(const Vector4& other);
    Vector4& operator+=(const Vector4& other);
    Vector4& operator-=(const Vector4& other);
    Vector4& operator*=(float s);
    Vector4& operator/=(float s);

    //--Arthritic Operations--
    Vector4 operator+(const Vector4& other)const;
    Vector4 operator-(const Vector4& other)const;
    Vector4 operator*(float s)const;
    Vector4 operator/(float s)const;
    Vector4 operator-()const;

public:
    float Magnitude()const;
    float SqrMagnitude()const;
    Vector4& Normalized();

public:
    static float    Angle(const Vector4& a, const Vector4& b);
    static float    Dot(const Vector4& a, const Vector4& b);
    static float    Distance(const Vector4& a, const Vector4& b);
    static float    DistanceSquared(const Vector4& a, const Vector4& b);
    static Vector4  Clamp(const Vector4& v, const Vector4& min, const Vector4& max);
    static Vector4  ClampLength(const Vector4& v, float s);
    static Vector4  Max(const Vector4& a, const Vector4& b);
    static Vector4  Min(const Vector4& a, const Vector4& b);
    static void     OrthoNormalize(Vector4& n, Vector4& tangent);
    static Vector4  Normalize(const Vector4& v);
    // Assumes Vectors are Normalised;
    static Vector4  Project(const Vector4& a, const Vector4& b);
    static Vector4  Reflect(const Vector4& dir, const Vector4& n);
    static Vector4  Lerp(const Vector4& a, const Vector4& b, float t);
    static Vector4	UnclampedLerp(const Vector4& a, const Vector4& b, float t);
    static Vector4  SmoothStep(const Vector4& min, const Vector4& max, float t);
    static Vector4  CatmullRom(const Vector4& a, const Vector4& b, const Vector4& c, const Vector4& d, float t);
    static Vector4  Bezier(const Vector4& a, const Vector4& b, const Vector4& c, float t);
    static Vector4  BezierCubic(const Vector4& a, const Vector4& b, const Vector4& c, const Vector4& d, float t);
    static Vector4  RoundToInt(const Vector4& a);
};

inline Vector4 operator*(float s, const Vector4& v)
{
    return {v.x * s, v.y * s, v.z * s, v.w * s};
}