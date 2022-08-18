/*
	Vectors are stored as columns, matrix takes the form:

	[Xx, Yx, Zx, Tx]
	[Xy, Yy, Zy, Ty]
	[Xz, Yz, Zz, Tz]
	[Xw, Yw, Zw, Tw]

	Memory layout is Column-Majour stored linearly and stored:

	[0, 4, 8,  12]
	[1, 5, 9,  13]
	[2, 6, 10, 14]
	[3, 7, 11, 15]
*/

#pragma once

class Vector3;
class Vector4;
class Quaternion;
class Matrix3;
class Matrix4
{
public:
	static const Matrix4 Identity;
	static const Matrix4 Zero;
	float m[16] = { 0 };

public:
	Matrix4();
	Matrix4(float m00, float m10, float m20, float m30,	// Col 1
			float m01, float m11, float m21, float m31,	// Col 2
			float m02, float m12, float m22, float m32, // Col 3
			float m03, float m13, float m23, float m33);// Col 4
	Matrix4(const Vector4& x, const Vector4& y, const Vector4& z, const Vector4& t);
	Matrix4(const Vector3& position, const Vector3& rotation, const Vector3& scale);
	Matrix4(const Vector3& position, const Quaternion& rotation, const Vector3& scale);
	Matrix4(const Matrix4& other);

public:
	bool	operator==(const Matrix4& other)const;
	bool	operator!=(const Matrix4& other)const;

	//--Access Operations--
	float&  operator[](int i);
	float	operator[](int i)const;

	//--Math Operations--
	Matrix4	 operator+(const Matrix4& other)const;
	Matrix4  operator-(const Matrix4& other)const;
	Matrix4  operator*(const Matrix4& other)const;
	Matrix4  operator*(const float s);

	//--Asignment Operations--
	Matrix4& operator=(const Matrix4& other);
	Matrix4& operator+=(const Matrix4& mat);
	Matrix4& operator-=(const Matrix4& mat);
	Matrix4& operator*=(const Matrix4& mat);
	Matrix4& operator*=(const float s);

public:
	Vector4& Column(int i);
	const Vector4& Column(int i)const;
	void	SetColumn(int i, const Vector4& column);
	void	Decompose(Vector3& position, Quaternion& rotation, Vector3& scale)const;
	float	Trace()const;
	Vector3 TransformVector(const Vector3& vector)const;
	Vector3 TransformPoint(const Vector3& point)const;
	Vector3 TransformPoint3x4(const Vector3& point)const;

public:
	static Matrix4 Minor(const Matrix4& other);
	static Matrix4 Cofactor(const Matrix4& other);
	static Matrix4 Adjoint(const Matrix4& other);
	static float   Determinant(const Matrix4& other);
	static Matrix4 Inverse(const Matrix4& other);
	static Matrix4 RotateX(float angle);
	static Matrix4 RotateY(float angle);
	static Matrix4 RotateZ(float angle);
	static Matrix4 Rotate(const Vector3& euler);
	static Matrix4 Rotate(const Quaternion& rotation);
	static Matrix4 LookAt(const Vector3& from, const Vector3& to, const Vector3& up);
	static Matrix4 Orthographic(int width, int height, float zNear, float zFar);
	static Matrix4 PerspectiveFov(float fov, float aspect, float zNear, float zFar);
	static Matrix4 Perspective(float width, float height, float zNear, float zFar);
	static Matrix4 PerspectiveOffCenter(float left, float right, float top, float bottom, float zNear, float zFar);
	static Matrix4 Scale(const Vector3& scale);
	static Matrix4 Translate(const Vector3& vector);
	static Matrix4 Transpose(const Matrix4& other);

private:
	static Matrix3 Get3x3(const Matrix4& mat, unsigned int row, unsigned int col);
};