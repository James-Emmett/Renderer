#pragma once

class Vector2
{
public:
    static const Vector2 Up;
    static const Vector2 Down;
    static const Vector2 Left;
    static const Vector2 Right;
    static const Vector2 One;
    static const Vector2 Zero;

public:
    float x,y = 0.0f;

public:
    Vector2(float X = 0.0f, float Y = 0.0f);
    Vector2(const Vector2& other);

public:
    //--Access Operations--
    float& operator[](int i);
    const float& operator[](int i)const;

    //--Comparison Operations--
    bool operator==(const Vector2& other)const;
    bool operator!=(const Vector2& other)const;

    //--Assignment Operations--
    Vector2& operator=(const float s);
    Vector2& operator=(const Vector2& other);
    Vector2& operator+=(const Vector2& other);
    Vector2& operator-=(const Vector2& other);
    Vector2& operator*=(float s);
    Vector2& operator/=(float s);

    //--Arthritic Operations--
    Vector2 operator+(const Vector2& other)const;
    Vector2 operator-(const Vector2& other)const;
    Vector2 operator*(float s)const;
    Vector2 operator/(float s)const;
    Vector2 operator-()const;

public:
    float Magnitude()const;
    float SqrMagnitude()const;
    Vector2& Normalized();

public:
    static float    Angle(const Vector2& a, const Vector2& b);
    // Angle too rotate from A's Tip too B's Tip on unit circle?
    static float    AngleDifference(const Vector2& a, const Vector2& b);
    static float    Dot(const Vector2& a, const Vector2& b);
    static Vector2  Perpendicular(const Vector2& v);
    static float    Distance(const Vector2& a, const Vector2& b);
    static float    DistanceSquared(const Vector2& a, const Vector2& b);
    static Vector2  Clamp(const Vector2& v, Vector2 min, Vector2 max);
    static Vector2  ClampLength(const Vector2& v, float s);
    static Vector2  Max(const Vector2& a, const Vector2& b);
    static Vector2  Min(const Vector2& a, const Vector2& b);
    static void     OrthoNormalize(Vector2& n, Vector2& tangent);
    static Vector2  Normalize(const Vector2& v);
    static Vector2  Project(const Vector2& a, const Vector2& b);
    static Vector2  Reflect(const Vector2& dir, const Vector2& n);
    static Vector2  Lerp(const Vector2& a, const Vector2& b, float t);
    static Vector2	UnclampedLerp(const Vector2& a, const Vector2& b, float t);
    static void		Barycentric(const Vector2& a, const Vector2& b, const Vector2& c, const Vector2& p, float& u, float& v, float& w);
    static Vector2  CatmullRom(const Vector2& a, const Vector2& b, const Vector2& c, const Vector2& d, float t);
    static Vector2  Bezier(const Vector2& a, const Vector2& b, const Vector2& c, float t);
    static Vector2  BezierCubic(const Vector2& a, const Vector2& b, const Vector2& c, const Vector2& d, float t);
    static Vector2  RoundToInt(const Vector2& a);
};

inline Vector2 operator*(float s, const Vector2& other)
{
    return Vector2(s * other.x, s * other.y);
}
