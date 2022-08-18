/*
	Vectors are stored as columns, matrix takes the form:

	[Xx, Yx, Zx]
	[Xy, Yy, Zy]
	[Xz, Yz, Zz]

	Memory layout is Column-Majour stored linearly and stoed:

	[0, 3, 6]
	[1, 4, 7]
	[2, 5, 8]
*/

#pragma once

class Vector3;
class Quaternion;
class Matrix3
{
public:
	static const Matrix3 Identity;
	static const Matrix3 Zero;
	float m[9] = { 0 };

public:
	Matrix3();
	Matrix3(float m00, float m10, float m20,	// Col 1
			float m01, float m11, float m21,	// Col 2
			float m02, float m12, float m22);	// Col 3
	Matrix3(const Vector3& x, const Vector3& y, const Vector3& z);
	Matrix3(const Matrix3& other);

public:
	bool	operator==(const Matrix3& mat)const;
	bool	operator!=(const Matrix3& mat)const;

	float&	operator[](int i);
	float	operator[](int i)const;

	float& operator()(int row, int col);
	const float& operator()(int row, int col)const;

	Matrix3& operator=(const Matrix3& mat);
	Matrix3	 operator+(const Matrix3& mat)const;
	Matrix3  operator-(const Matrix3& mat)const;
	Matrix3  operator*(const Matrix3& mat)const;
	Matrix3  operator*(float s);

	Matrix3& operator+=(const Matrix3& mat);
	Matrix3& operator-=(const Matrix3& mat);
	Matrix3& operator*=(const Matrix3& mat);
	Matrix3& operator*=(float s);


public:
	Vector3& Column(int i);
	const Vector3& Column(int i)const;
	void SetColumn(int i, const Vector3& column);
	float Trace()const;

public:
	static Matrix3 Minor(const Matrix3& mat);
	static Matrix3 Cofactor(const Matrix3& mat);
	static Matrix3 Adjoint(const Matrix3& mat);
	static float   Determinant(const Matrix3& mat);
	static Matrix3 Inverse(const Matrix3& mat);
	static Matrix3 RotateX(float angle);
	static Matrix3 RotateY(float angle);
	static Matrix3 RotateZ(float angle);
	static Matrix3 Rotate(const Vector3& euler);
	static Matrix3 Rotate(const Quaternion& rotation);
	static Matrix3 Scale(const Vector3& scale);
	static Matrix3 Transpose(const Matrix3& mat);

	Vector3 Transform(const Vector3& vector)const;
};