#include "Math/Matrix2D.h"
#include "Math/Mathf.h"

Matrix2D Matrix2D::Identity = Matrix2D(1, 0, 0, 1, 0, 0);

Matrix2D::Matrix2D(float m00, float m10, float m01, float m11, float m02, float m12)
{
    // X Column
    M00 = m00;
    M10 = m10;

    // Y Column
    M01 = m01;
    M11 = m11;

    // transform Column
    M02 = m02;
    M12 = m12;
}

Matrix2D::Matrix2D(const Matrix2D& matrix)
{
    M00 = matrix.M00;
    M01 = matrix.M01;

    M10 = matrix.M10;
    M11 = matrix.M11;

    M02 = matrix.M02;
    M12 = matrix.M12;
}

Matrix2D Matrix2D::operator*(const Matrix2D& other)
{
    // X Column
    float m00 = M00 * other.M00 + M01 * other.M10;
    float m10 = M10 * other.M00 + M11 * other.M10;

    // Y column
    float m01 = M00 * other.M01 + M01 * other.M11;
    float m11 = M10 * other.M01 + M11 * other.M11;

    // Transform Column
    float m02 = M00 * other.M02 + M01 * other.M12 + M02;
    float m12 = M10 * other.M02 + M11 * other.M12 + M12;

    return Matrix2D(m00, m10, m01, m11, m02, m12);
}

Matrix2D Matrix2D::operator*(float scalar)
{
    return Matrix2D(M00 * scalar, M10 * scalar,
                    M01 * scalar, M11 * scalar,
                    M02 * scalar, M12 * scalar);
}

Matrix2D Matrix2D::operator+(const Matrix2D& other)
{
    return Matrix2D(M00 + other.M00, M10 + other.M10,
                    M01 + other.M01, M11 + other.M11,
                    M02 + other.M02, M12 + other.M12);
}

bool Matrix2D::operator==(Matrix2D other)
{
    return  (M00 == other.M00) && (M01 == other.M01) && (M10 == other.M10) &&
            (M11 == other.M11) && (M02 == other.M02) && (M12 == other.M12);
}

bool Matrix2D::operator!=(Matrix2D other)
{
    return  (M00 != other.M00) || (M01 != other.M01) || (M10 != other.M10) ||
            (M11 != other.M11) || (M02 != other.M02) || (M12 != other.M12);
}

Vector2 Matrix2D::Translation() const
{
    return {M02, M12};
}

float Matrix2D::Rotation() const
{
    return (float)Mathf::Atan2(M01, M00);
}

Vector2 Matrix2D::Scale() const
{
    // Take length of columns, sqrt negates the -sin(0)
    return { Mathf::Sqrt(M00 * M00 + M01 * M01),
             Mathf::Sqrt(M10 * M10 + M11 * M11)};
}

Vector2 Matrix2D::Transform(const Vector2& v)const
{
    Vector2 product;
    product.x = v.x * M00 + v.y * M01 + M02;
    product.y = v.x * M10 + v.y * M11 + M12;
    return product;
}

float Matrix2D::Determinant() const
{
    return M00 * M11 - M10 * M01;
}

Matrix2D Matrix2D::CreateTransform(const Vector2& position, float rotation, const Vector2& scale)
{
    return CreateTranslation(position) * CreateRotation(rotation) * CreateScale(scale);
}

Matrix2D Matrix2D::CreateRotation(float radians)
{
    float cos = Mathf::Cos(radians);
    float sin = Mathf::Sin(radians);

    return Matrix2D(cos, sin, -sin, cos, 0, 0);
}

Matrix2D Matrix2D::CreateScale(float scale)
{
    return Matrix2D(scale, 0, 0, scale, 0, 0);
}

Matrix2D Matrix2D::CreateScale(const Vector2& scale)
{
    return Matrix2D(scale.x, 0, 0, scale.y, 0, 0);
}

Matrix2D Matrix2D::CreateTranslation(float x, float y)
{
    return Matrix2D(1, 0, 0, 1, x, y);
}

Matrix2D Matrix2D::CreateTranslation(Vector2 v)
{
    return Matrix2D(1, 0, 0, 1, v.x, v.y);
}

Matrix2D Matrix2D::Invert(const Matrix2D& matrix)
{
    // Take matrix of minors, M11, M12, M21, M22 just get single value as
    // 0 cancels the - bracket term out i.e M11 = (M22 * M33(1.0f)) - (M23(0.0f) * M32) == M22
    // Note we can ignore M31 and M32 as there value comes from M13 and M23 instead! due to Adjoint!
    // So we save Memory

    // With Minors now do the cofactors, i.e put negatives in front of M12, M21, M32
    // Now calculate adjoint, basically swap M12 with M22, M31 with M13 etc
    // Finally divide by Determinant

    // Below all steps are performned in 1 go
    float det = 1.0f / matrix.Determinant();

    return Matrix2D
    (
        // X Col
        matrix.M11 * det,
        -matrix.M10 * det,

        // Y col
        -matrix.M01 * det,
        matrix.M00 * det,

        //Transform Col
        (matrix.M01 * matrix.M12 - matrix.M02 * matrix.M11) * det,
        -(matrix.M00 * matrix.M12 - matrix.M02 * matrix.M10) * det
    );
}