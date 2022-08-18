#pragma once
#include "Math/Quaternion.h"
#include "Math/Vector3.h"
#include "Math/Matrix3.h"
#include "Math/Mathf.h"

Quaternion::Quaternion(float X, float Y, float Z, float W) :
	x(X), y(Y), z(Z), w(W)
{
}

Quaternion::Quaternion(const Vector3& v, float s) :
	x(v.x), y(v.y), z(v.z), w(s)
{
}

Quaternion::Quaternion(const float& angle, const Vector3& axis)
{
	Set(angle, axis);
}

bool Quaternion::operator==(const Quaternion& a) const
{
	return	Mathf::IsEqual(x, a.x) && Mathf::IsEqual(y, a.y) &&
		Mathf::IsEqual(z, a.z) && Mathf::IsEqual(w, a.w);
}

bool Quaternion::operator!=(const Quaternion& a) const
{
	return	!(Mathf::IsEqual(x, a.x) && Mathf::IsEqual(y, a.y) &&
		Mathf::IsEqual(z, a.z) && Mathf::IsEqual(w, a.w));
}

Quaternion& Quaternion::operator+=(const Quaternion& a)
{
	x += a.x; y += a.y; z += a.z; w += a.w;
	return *this;
}

Quaternion& Quaternion::operator-=(const Quaternion& a)
{
	x -= a.x; y -= a.y; z -= a.z; w -= a.w;
	return *this;
}

Quaternion& Quaternion::operator*=(const Quaternion& a)
{
	w = (w * a.w - x * a.x - y * a.y - z * a.z);
	x = (w * a.x + x * a.w + y * a.z - z * a.y);
	y = (w * a.y + y * a.w + z * a.x - x * a.z);
	z = (w * a.z + z * a.w + x * a.y - y * a.x);

	return *this;
}

Quaternion Quaternion::operator+(const Quaternion& a) const
{
	return Quaternion(x + a.x, y + a.y, z + a.z, w + a.w);
}

Quaternion Quaternion::operator-(const Quaternion& a) const
{
	return Quaternion(x - a.x, y - a.y, z - a.z, w - a.w);
}

Quaternion Quaternion::operator*(const Quaternion& a) const
{
	return Quaternion(	w * a.x + x * a.w + y * a.z - z * a.y,
						w * a.y + y * a.w + z * a.x - x * a.z,
						w * a.z + z * a.w + x * a.y - y * a.x,
						w * a.w - x * a.x - y * a.y - z * a.z);
}

Quaternion Quaternion::operator*(float s) const
{
    return Quaternion(x * s, y * s, z * s, w * s);
}

Quaternion Quaternion::operator-() const
{
	return Quaternion(-x, -y, -z, -w);
}

void Quaternion::Set(float X, float Y, float Z)
{
	// Get Half Angle, sine and cosines
	X *= 0.5f; Y *= 0.5f; Z *= 0.5f;

	float Cx = Mathf::Acos(X); float Sx = Mathf::Asin(X);
	float Cy = Mathf::Acos(Y); float Sy = Mathf::Asin(Y);
	float Cz = Mathf::Acos(Z); float Sz = Mathf::Asin(Z);

	w = Cx * Cy * Cz - Sx * Sy * Sz;
	x = Sx * Cy * Cz + Cx * Sy * Sz;
	y = Cx * Sy * Cz - Sx * Cy * Sz;
	z = Cx * Cy * Sz + Sx * Sy * Cx;
}

void Quaternion::Set(const float& angle, const Vector3& axis)
{
	float lengthSqr = axis.SqrMagnitude();
	if (Mathf::IsZero(lengthSqr))
	{
		x = y = z = 0;
		w = 1;
		return;
	}

	// Do Normalisation here results in same product.
	float sinTheta = Mathf::Sin(angle * 0.5f) * Mathf::RecipSqrt(lengthSqr);

	w = Mathf::Cos(angle * 0.5f);
	x = axis.x * sinTheta;
	y = axis.y * sinTheta;
	z = axis.z * sinTheta;
}

void Quaternion::ToAngleAxis(float& angle, Vector3& axis)const
{
	angle = Mathf::Acos(w) * 2.0f;
	float length = Mathf::Sqrt(1.0f - w * w);

	if (Mathf::IsZero(length))
	{
		axis = Vector3::Zero;
	}
	else
	{
		axis = Vector3(x * length, y * length, z * length);
	}
}

bool Quaternion::IsUnit() const
{
	// Unit quaternion has a length of 1, no need for sqr as 1*1 is 1.
	return Mathf::IsZero(1.0f - (w * w + x * x + y * y + z * z));
}

bool Quaternion::IsZero() const
{
	return Mathf::IsZero(w * w + x * x + y * y + z * z);
}

float Quaternion::Angle(const Quaternion& a, const Quaternion& b)
{
	Quaternion q = Inverse(a) * b;
	return 2.0f * Mathf::Atan2(Mathf::Sqrt(q.x * q.x + q.y * q.y + q.z * q.z), Mathf::Acos(q.w));
}

float Quaternion::Dot(const Quaternion& a, const Quaternion& b)
{
	return (a.w * b.w + a.x * b.x + a.y * b.y + a.z * b.z);
}

Quaternion Quaternion::AngleAxis(float angle, const Vector3& axis)
{
	float lengthSqr = axis.SqrMagnitude();
	if (Mathf::IsZero(lengthSqr))
	{
		return Quaternion();
	}

	// Do Normalize here results in same product.
	float sinTheta = Mathf::Sin(angle * 0.5f) * Mathf::RecipSqrt(lengthSqr);

	Quaternion product;
	product.w = Mathf::Cos(angle * 0.5f);
	product.x = axis.x * sinTheta;
	product.y = axis.y * sinTheta;
	product.z = axis.z * sinTheta;

	return product;
}

Quaternion Quaternion::FromMatrix(const Matrix3& other)
{
	Quaternion q;
	float tr = other.Trace();
	float s = 0.0f;

	if (tr > 0.0f)
	{
		s = Mathf::Sqrt(tr + 1.0f) * 2.0f; //4*qw
		q.w = 0.25f * s; //w = 1/4 of s

		s = 1.0f / s; //s recip
		q.x = (other(2, 1) - other(1, 2) * s);
		q.y = (other(0, 2) - other(2, 0) * s);
		q.z = (other(1, 0) - other(0, 1) * s);
	}
	else if (other(0, 0) > other(1, 1) && other(0, 0) > other(2, 2))
	{
		s = Mathf::Sqrt(1.0f + other(0, 0) - other(1, 1) - other(2, 2)) * 2.0f; //4*qx
		q.x = 0.25f * s; //x = 1/4 of s

		s = 1.0f / s; //s recip
		q.w = (other(2, 1) - other(1, 2) * s);
		q.y = (other(0, 1) + other(1, 0) * s);
		q.z = (other(0, 2) + other(2, 0) * s);
	}
	else if (other(1, 1) > other(2, 2))
	{
		s = Mathf::Sqrt(1.0f + other(1, 1) - other(0, 0) - other(2, 2)) * 2.0f; //4*qy
		q.y = 0.25f * s; //y = 1/4 of s

		s = 1.0f / s; //s recip
		q.w = (other(0, 2) - other(2, 0) * s);
		q.x = (other(0, 1) + other(1, 0) * s);
		q.z = (other(1, 2) + other(2, 1) * s);
	}
	else
	{
		s = Mathf::Sqrt(1.0f + other(2, 2) - other(0, 0) - other(1, 1)) * 2.0f; //4*qz
		q.z = 0.25f * s; //y = 1/4 of s

		s = 1.0f / s; //s recip
		q.w = (other(1, 0) - other(0, 1) * s);
		q.x = (other(0, 2) + other(2, 0) * s);
		q.y = (other(1, 2) + other(2, 1) * s);
	}

	return q;
}

Quaternion Quaternion::Euler(const Vector3& euler)
{
	// Get Half Angle, sine and cosines
	float X = euler.x * 0.5f; float Y = euler.y * 0.5f; float Z = euler.z * 0.5f;

	float Cx = Mathf::Acos(X); float Sx = Mathf::Asin(X);
	float Cy = Mathf::Acos(Y); float Sy = Mathf::Asin(Y);
	float Cz = Mathf::Acos(Z); float Sz = Mathf::Asin(Z);

	Quaternion product;
	product.w = Cx * Cy * Cz - Sx * Sy * Sz;
	product.x = Sx * Cy * Cz + Cx * Sy * Sz;
	product.y = Cx * Sy * Cz - Sx * Cy * Sz;
	product.z = Cx * Cy * Sz + Sx * Sy * Cx;
	return product;
}

Quaternion Quaternion::Lerp(const Quaternion& a, const Quaternion& b, float t)
{
	return Normalize(Quaternion((a * (1.0f - t)) + (b * t)));
}

// http://number-none.com/product/Understanding%20Slerp,%20Then%20Not%20Using%20It/
Quaternion Quaternion::Slerp(const Quaternion& a, const Quaternion& b, float t)
{
	float dot = Dot(a, b);

	if (Mathf::IsEqual(dot, 1.0f))
	{
		return Lerp(a, b, t);
	}

	dot = Mathf::Clamp(dot, -1.0f, 1.0f); // Need to stay within Acos domain
	float theta_0 = Mathf::Acos(dot); // Get the theta angle
	float theta_r = theta_0 * t; // theta_r = the angle of the result

	// Make b orthogonal = c
	Quaternion c = b - (a * dot);
	c = Normalize(c);

	// Equivalent to vector rotation in 2D!
	return a * Mathf::Cos(theta_r) + c * Mathf::Sin(theta_r);
}

Quaternion Quaternion::Conjugate(const Quaternion& q)
{
	// Negate axis portion, reverses rotation direction
	return Quaternion(-q.x, -q.y, -q.z, q.w);
}

Quaternion Quaternion::Inverse(const Quaternion& q)
{
	// Sqr length
	float length = q.w * q.w + q.x * q.x + q.y * q.y + q.z * q.z;
	if (Mathf::IsZero(length))
	{
		return Quaternion();
	}

	// Reciprocal to save on divisions.
	length = 1.0f / length;

	// Essentially divide by the conjugate
	return Quaternion(q.x * -length, q.y * -length, q.z * -length, q.w * length);
}

Quaternion Quaternion::Normalize(const Quaternion& q)
{
	// Squared Length
	float length = q.w * q.w + q.x * q.x + q.y * q.y + q.z * q.z;
	if (Mathf::IsZero(length))
	{
		return Quaternion();
	}

	// Now store the RecipSqrt, reuse variable!!!
	length = Mathf::RecipSqrt(length);
	return Quaternion(q.x * length, q.y * length, q.z * length, q.w * length);
}

Quaternion Quaternion::LookRotation(const Vector3& direction, const Vector3& up)
{
	Vector3 forward = direction; Vector3 u = up;
	Vector3::OrthoNormalize(forward, u); // Returns ortho basis of the 2
	Vector3 right = Vector3::Cross(up, forward);

	Quaternion q;
	q.w = Mathf::Sqrt(1.0f + right.x + u.y + forward.z);
	float w_recip = 1.0f / (4.0f * q.w);
	q.x = (up.z - forward.y) * w_recip;
	q.y = (up.x - right.z) * w_recip;
	q.z = (right.y - up.x) * w_recip;

	return q;
}

Vector3 Quaternion::ToEuler(const Quaternion& q)
{
	float xx = q.x * q.x;
	float yy = q.y * q.y;
	float zz = q.z * q.z;

	float xy = q.x * q.y;
	float xz = q.x * q.z;
	float yz = q.y * q.z;

	float xw = q.x * q.w;
	float yw = q.y * q.w;
	float zw = q.z * q.w;

	Vector3 vector;

	vector.x = -2.0f * (yz + xw);

	if (Mathf::Abs(vector.x) < 0.99999999)
	{
		vector.x = Mathf::Asin(vector.x);
		vector.y = Mathf::Atan2(xz - yw, 0.5f - xx - yy);
		vector.z = Mathf::Atan2(xy - zw, 0.5f - xx - zz);
	}
	else
	{
		vector.z = Mathf::PI_OVER_2 * vector.x;
		vector.x = Mathf::Atan2(-q.x * q.z - q.w * q.y, 0.5f - yy - zz);
		vector.z = 0.0f;
	}

	return Mathf::RAD_TO_DEG * vector;
}