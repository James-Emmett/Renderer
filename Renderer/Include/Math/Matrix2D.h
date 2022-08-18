#pragma once
#include "Vector2.h"

// 3x3 transformation matrix for 2D points.
// Heavily inspired by:
// https://github.com/craftworkgames/MonoGame.Extended/blob/develop/Source/MonoGame.Extended/Math/Matrix2.cs
class Matrix2D
{
public:
    // X Col Rotation/Scale.
    float M00; // x.
    float M10; // y.
    // M20 == 0 always leave out too save memory

    // Y Col Rotation/Scale.
    float M01; // x.
    float M11; // y.
    // M21 == 0 always leave out too save memory

    // Translation Col for moving.
    float M02; // x.
    float M12; // y.
    // M22 == 1 always leave out to save memory

    static Matrix2D Identity;

public:
    explicit Matrix2D(float m00 = 1.0f, float m10 = 0.0f, float m01 = 0.0f, float m11 = 1.0f, float m02 = 0.0f, float m12 = 0.0f);
    Matrix2D(const Matrix2D& matrix);

public:
    Matrix2D operator*(const Matrix2D& m);
    Matrix2D operator*(float scalar);
    Matrix2D operator+(const Matrix2D& m);
    bool     operator==(Matrix2D m);
    bool     operator!=(Matrix2D m);

public:
    Vector2 Translation()const;
    float   Rotation() const;
    Vector2 Scale()const;
    float   Determinant()const;
    Vector2 Transform(const Vector2& vector)const;

public:
    static Matrix2D CreateTransform(const Vector2& position, float rotation, const Vector2& scale);
    static Matrix2D CreateRotation(float radians);
    static Matrix2D CreateScale(float scale);
    static Matrix2D CreateScale(const Vector2& scale);
    static Matrix2D CreateTranslation(float x, float y);
    static Matrix2D CreateTranslation(Vector2 position);
    static Matrix2D Invert(const Matrix2D& matrix);
};