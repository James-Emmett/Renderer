#pragma once

class Matrix3;
class Vector3;
class Quaternion
{
public:
	float x, y, z, w;

public:
	explicit Quaternion(float X = 0.0f, float Y = 0.0f, float Z = 0.0f, float W = 1.0f);
	Quaternion(const Vector3& v, float s);
	Quaternion(const float& angle, const Vector3& axis);

public:
	bool operator==(const Quaternion& a)const;
	bool operator!=(const Quaternion& a)const;

	Quaternion& operator+=(const Quaternion& a);
	Quaternion& operator-=(const Quaternion& a);
	Quaternion& operator*=(const Quaternion& a);

	Quaternion operator+(const Quaternion& a)const;
	Quaternion operator-(const Quaternion& a)const;
	Quaternion operator*(const Quaternion& a)const;
    Quaternion operator*(float s)const;

	Quaternion operator-()const;

public:
	// From Euler Fixed Angles too Quaternion
	void Set(float X, float Y, float Z);
	void Set(const float& angle, const Vector3& axis);
	void ToAngleAxis(float& angle, Vector3& axis)const;
	bool IsUnit()const;
	bool IsZero()const;

public:
	static float Angle(const Quaternion& a, const Quaternion& b);
	static float Dot(const Quaternion& a, const Quaternion& b);
	static Quaternion AngleAxis(float angle, const Vector3& axis);
	static Quaternion FromMatrix(const Matrix3& mat);
	static Quaternion Euler(const Vector3& euler);
	static Quaternion Lerp(const Quaternion& a, const Quaternion& b, float t);
	static Quaternion Slerp(const Quaternion& a, const Quaternion& b, float t);
	static Quaternion Conjugate(const Quaternion& q);
	// Assums q is normalized
	static Quaternion Inverse(const Quaternion& q);
	static Quaternion Normalize(const Quaternion& q);
	static Quaternion LookRotation(const Vector3& direction, const Vector3& up);
	static Vector3 ToEuler(const Quaternion& q);
};