#include "Math/Matrix4.h"
#include "Math/Matrix3.h"
#include "Math/Vector3.h"
#include "Math/Vector4.h"
#include "Math/Quaternion.h"
#include "Math/Mathf.h"
#include "System/Assert.h"

#define MAT4_I(row, col) (row + (col * 4))

const Matrix4 Matrix4::Identity = Matrix4(1,0,0,0, 0,1,0,0, 0,0,1,0, 0,0,0,1);
const Matrix4 Matrix4::Zero = Matrix4(0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0);

Matrix4::Matrix4()
{
	m[0]  = 1; m[1]  = 0; m[2]  = 0; m[3]  = 0; // Col 1
	m[4]  = 0; m[5]  = 1; m[6]  = 0; m[7]  = 0; // Col 2
	m[8]  = 0; m[9]  = 0; m[10] = 1; m[11] = 0; // Col 3
	m[12] = 0; m[13] = 0; m[14] = 0; m[15] = 1; // Col 4 | Transform
}

Matrix4::Matrix4(float m00, float m10, float m20, float m30,
				 float m01, float m11, float m21, float m31,
				 float m02, float m12, float m22, float m32,
				 float m03, float m13, float m23, float m33)
{
	m[0]  = m00; m[1]  = m10; m[2]  = m20; m[3]  = m30; // Col 1
	m[4]  = m01; m[5]  = m11; m[6]  = m21; m[7]  = m31; // Col 2
	m[8]  = m02; m[9]  = m12; m[10] = m22; m[11] = m32; // Col 3
	m[12] = m03; m[13] = m13; m[14] = m23; m[15] = m33; // Col 4 | Transform
}

Matrix4::Matrix4(const Vector4& x, const Vector4& y, const Vector4& z, const Vector4& t)
{
	m[0]  = x.x; m[1]  = x.y; m[2]  = x.z; m[3]  = x.w; // Col 1
	m[4]  = y.x; m[5]  = y.y; m[6]  = y.z; m[7]  = y.w; // Col 2
	m[8]  = z.x; m[9]  = z.y; m[10] = z.z; m[11] = z.w; // Col 3
	m[12] = t.x; m[13] = t.y; m[14] = t.y; m[15] = t.w; // Col 4 | Transform
}

Matrix4::Matrix4(const Vector3& position, const Vector3& rotation, const Vector3& scale)
{
	*this = Translate(position) * Rotate(rotation) * Scale(scale);
}

Matrix4::Matrix4(const Vector3& position, const Quaternion& rotation, const Vector3& scale)
{
	*this = Translate(position) * Rotate(rotation) * Scale(scale);
}

Matrix4::Matrix4(const Matrix4& other)
{
	m[0]  = other.m[0];  m[1]  = other.m[1];  m[2]  = other.m[2];  m[3]  = other.m[3];  // Col 1
	m[4]  = other.m[4];  m[5]  = other.m[5];  m[6]  = other.m[6];  m[7]  = other.m[7];  // Col 2
	m[8]  = other.m[8];  m[9]  = other.m[9];  m[10] = other.m[10]; m[11] = other.m[11]; // Col 3
	m[12] = other.m[12]; m[13] = other.m[13]; m[14] = other.m[14]; m[15] = other.m[15]; // Col 4 | Transform
}

bool Matrix4::operator==(const Matrix4& other)const
{
	for (int i = 0; i < 16; ++i)
	{
		if (!Mathf::IsEqual(m[i], other.m[i]))
		{
			return false;
		}
	}

	return true;
}

bool Matrix4::operator!=(const Matrix4& other)const
{
	for (int i = 0; i < 16; ++i)
	{
		if (Mathf::IsEqual(m[i], other.m[i]))
		{
			return false;
		}
	}

	return true;
}

float& Matrix4::operator[](int i)
{
	return m[i];
}

float Matrix4::operator[](int i)const
{
	return m[i];
}

Matrix4	Matrix4::operator+(const Matrix4& other)const
{
	return Matrix4(	m[0]  + other.m[0],		m[1]  + other.m[1],		m[2]  + other.m[2],		m[3]  + other.m[3],
					m[4]  + other.m[4],		m[5]  + other.m[5],		m[6]  + other.m[6],		m[7]  + other.m[7],
					m[8]  + other.m[8],		m[9]  + other.m[9],		m[10] + other.m[10],	m[11] + other.m[11],
					m[12] + other.m[12],	m[13] + other.m[13],	m[14] + other.m[14],	m[15] + other.m[15]);
}

Matrix4 Matrix4::operator-(const Matrix4& other)const
{
	return Matrix4(	m[0]  - other.m[1],		m[1]  - other.m[1],		m[2]  - other.m[2],		m[3]  - other.m[3],
					m[4]  - other.m[4],		m[5]  - other.m[5],		m[6]  - other.m[6],		m[7]  - other.m[7],
					m[8]  - other.m[8],		m[9]  - other.m[9],		m[10] - other.m[10],	m[11] - other.m[11],
					m[12] - other.m[12],	m[13] - other.m[13],	m[14] - other.m[14],	m[15] - other.m[15]);
}

Matrix4 Matrix4::operator*(const Matrix4& other)const
{
	Matrix4 product;

	for (int r = 0; r < 4; ++r)
	{
		for (int c = 0; c < 4; ++c)
		{
			int index = MAT4_I(r, c);
			product[index] = 0.0f;
			for (int mid = 0; mid < 4; ++mid)
			{
				product.m[index] += m[MAT4_I(r, mid)] * other.m[MAT4_I(mid, c)];
			}
		}
	}

	return product;
}

Matrix4 Matrix4::operator*(const float s)
{
	return Matrix4( m[0]  * s, m[1]  * s, m[2]  * s, m[3]  * s,  // Col 1
					m[4]  * s, m[5]  * s, m[6]  * s, m[7]  * s,  // Col 2
					m[8]  * s, m[9]  * s, m[10] * s, m[11] * s,  // Col 3
					m[12] * s, m[13] * s, m[14] * s, m[15] * s); // Col 4 | Transform)
}

Matrix4& Matrix4::operator=(const Matrix4& other)
{
	m[0]  = other.m[0];  m[1]  = other.m[1];  m[2]  = other.m[2];  m[3]  = other.m[3];  // Col 1
	m[4]  = other.m[4];  m[5]  = other.m[5];  m[6]  = other.m[6];  m[7]  = other.m[7];  // Col 2
	m[8]  = other.m[8];  m[9]  = other.m[9];  m[10] = other.m[10]; m[11] = other.m[11]; // Col 3
	m[12] = other.m[12]; m[13] = other.m[13]; m[14] = other.m[14]; m[15] = other.m[15]; // Col 4 | Transform
	return *this;
}

Matrix4& Matrix4::operator+=(const Matrix4& mat)
{
    m[0]  += mat.m[0];  m[1]  += mat.m[1];  m[2]  += mat.m[2];  m[3]  += mat.m[3];  // Col 0
    m[4]  += mat.m[4];  m[5]  += mat.m[5];  m[6]  += mat.m[6];  m[7]  += mat.m[7];  // Col 1
    m[8]  += mat.m[8];  m[9]  += mat.m[9];  m[10] += mat.m[10]; m[11] += mat.m[11]; // Col 2
    m[12] += mat.m[12]; m[13] += mat.m[13]; m[14] += mat.m[14]; m[15] += mat.m[15]; // Col 3

    return *this;
}

Matrix4& Matrix4::operator-=(const Matrix4& mat)
{
    m[0]  -= mat.m[0];  m[1]  -= mat.m[1];  m[2]  -= mat.m[2];  m[3]  -= mat.m[3];  // Col 0
    m[4]  -= mat.m[4];  m[5]  -= mat.m[5];  m[6]  -= mat.m[6];  m[7]  -= mat.m[7];  // Col 1
    m[8]  -= mat.m[8];  m[9]  -= mat.m[9];  m[10] -= mat.m[10]; m[11] -= mat.m[11]; // Col 2
    m[12] -= mat.m[12]; m[13] -= mat.m[13]; m[14] -= mat.m[14]; m[15] -= mat.m[15]; // Col 3

    return *this;
}

Matrix4& Matrix4::operator*=(const Matrix4& mat)
{
    Matrix4 product = *this;

    for (int row = 0; row < 4; row++)
    {
        for (int col = 0; col < 4; col++)
        {
            int i = MAT4_I(row, col);
            m[i] = 0.0f;

            for (int mid = 0; mid < 4; mid++)
            {
                m[i] += product.m[MAT4_I(row, mid)] * mat.m[MAT4_I(mid, col)];
            }
        }
    }

    return *this;
}

Matrix4& Matrix4::operator*=(const float s)
{
    m[0]  *= s; m[1]  *= s; m[2]  *= s; m[3]  *= s; // Col 0
    m[4]  *= s; m[5]  *= s; m[6]  *= s; m[7]  *= s; // Col 1
    m[8]  *= s; m[9]  *= s; m[10] *= s; m[11] *= s; // Col 2
    m[12] *= s; m[13] *= s; m[14] *= s; m[15] *= s; // Col 3

    return *this;
}

Vector4& Matrix4::Column(int i)
{
	return *((Vector4*)(&m[i * 4]));
}

const Vector4& Matrix4::Column(int i)const
{
	return *((Vector4*)(&m[i * 4]));
}

void Matrix4::SetColumn(int i, const Vector4& column)
{
	i *= 4;
	m[i] = column.x; m[i + 1] = column.y; m[i + 2] = column.z; m[i + 3] = column.w;
}

void Matrix4::Decompose(Vector3& position, Quaternion& rotation, Vector3& scale) const
{
	position.x = m[12]; position.y = m[13]; position.z = m[14];

	// Extract Scale, Get magnitude also removes the sign?
	scale.x = m[15] * Mathf::Sqrt(m[0] * m[0] + m[1] * m[1] + m[2] * m[2]);
	scale.y = m[15] * Mathf::Sqrt(m[4] * m[4] + m[5] * m[5] + m[6] * m[6]);
	scale.z = m[15] * Mathf::Sqrt(m[8] * m[8] + m[9] * m[9] + m[10] * m[10]);

	float x = 1.0f / scale.x;
	float y = 1.0f / scale.y;
	float z = 1.0f / scale.z;

	// Grab Rotation
	Matrix3 matrix(	m[0] * x, m[1] * x, m[2] * x,
					m[4] * y, m[5] * y, m[6] * y,
					m[8] * z, m[9] * z, m[10] * z);

	// Should work?
	rotation = Quaternion::FromMatrix(matrix);
}

float Matrix4::Trace()const
{
	return m[0] + m[5] + m[10] + m[15];
}

Vector3 Matrix4::TransformVector(const Vector3& vector) const
{
	return { vector.x * m[0] + vector.y * m[4] + vector.z * m[8],
					vector.x * m[1] + vector.y * m[5] + vector.z * m[9],
					vector.x * m[2] + vector.y * m[6] + vector.z * m[10]};
}

Vector3 Matrix4::TransformPoint(const Vector3& point) const
{
	// We are essentially doing the Perspective division here!
	float w = point.x * m[3] + point.y * m[7] + point.z * m[11] + m[15];

	if (Mathf::IsZero(w)) { w = 1.0f; }
	w = 1.0f / w;

	return { (point.x * m[0] + point.y * m[4] + point.z * m[8] + m[12]) * w,
                (point.x * m[1] + point.y * m[5] + point.z * m[9] + m[13]) * w,
                (point.x * m[2] + point.y * m[6] + point.z * m[10] + m[14]) * w};
}

Vector3 Matrix4::TransformPoint3x4(const Vector3& point) const
{
	return {	point.x * m[0] + point.y * m[4] + point.z * m[8] + m[12],
                point.x * m[1] + point.y * m[5] + point.z * m[9] + m[13],
                point.x * m[2] + point.y * m[6] + point.z * m[10] + m[14]};
}

Matrix4 Matrix4::Minor(const Matrix4& mat)
{
	Matrix4 product;
	for (unsigned int row = 0; row < 4; row++)
	{
		for (unsigned int col = 0; col < 4; col++)
		{
			//Minors is the deterinant of the 3x3 matrix
			product[MAT4_I(row, col)] = Matrix3::Determinant(Get3x3(mat, row, col));
		}
	}
	return product;
}

Matrix4 Matrix4::Cofactor(const Matrix4& mat)
{
	Matrix4 minor = Minor(mat);
	return {	Mathf::Abs(minor[0]), -minor[1], Mathf::Abs(minor[2]), -minor[3],
                -minor[4], Mathf::Abs(minor[5]), -minor[6], Mathf::Abs(minor[7]),
                Mathf::Abs(minor[8]), -minor[9], Mathf::Abs(minor[10]), -minor[11],
                -minor[12], Mathf::Abs(minor[13]), -minor[14], Mathf::Abs(minor[15])};
}

Matrix4 Matrix4::Adjoint(const Matrix4& mat)
{
	// Still Need to speed this up.
	return Transpose(Cofactor(mat));
}

//http://www.cg.info.hiroshima-cu.ac.jp/~miyazaki/knowledge/teche23.html
float Matrix4::Determinant(const Matrix4& mat)
{
	return mat[0] * mat[5] * mat[10] * mat[15] + mat[0] * mat[6] * mat[11] * mat[13] + mat[0] * mat[7] * mat[9] * mat[14]
		 + mat[1] * mat[4] * mat[11] * mat[14] + mat[1] * mat[6] * mat[8] * mat[15] + mat[1] * mat[7] * mat[10] * mat[12]
		 + mat[2] * mat[4] * mat[9] * mat[15] + mat[2] * mat[5] * mat[11] * mat[12] + mat[2] * mat[7] * mat[8] * mat[13]
		 + mat[3] * mat[4] * mat[10] * mat[13] + mat[3] * mat[5] * mat[8] * mat[14] + mat[3] * mat[6] * mat[9] * mat[12]
		 - mat[0] * mat[5] * mat[11] * mat[14] - mat[0] * mat[6] * mat[9] * mat[15] - mat[0] * mat[7] * mat[10] * mat[13]
		 - mat[1] * mat[4] * mat[10] * mat[15] - mat[1] * mat[6] * mat[11] * mat[12] - mat[1] * mat[7] * mat[8] * mat[14]
		 - mat[2] * mat[4] * mat[11] * mat[13] - mat[2] * mat[5] * mat[8] * mat[15] - mat[2] * mat[7] * mat[9] * mat[12]
		 - mat[3] * mat[4] * mat[9] * mat[14] - mat[3] * mat[5] * mat[10] * mat[12] - mat[3] * mat[6] * mat[8] * mat[13];
}

// I just hope this is correct and dont touch it.
Matrix4 Matrix4::Inverse(const Matrix4& mat)
{
	float det = Determinant(mat);

	if (Mathf::IsZero(det))
	{
		return {};
	}

	float invDet = 1.0f / det;

	Matrix4 product;
	product[0] = (mat[5] * mat[10] * mat[15] + mat[6] * mat[11] * mat[13] + mat[7] * mat[9] * mat[14] - mat[5] * mat[11] * mat[14] - mat[6] * mat[9] * mat[15] - mat[7] * mat[10] * mat[13]) * invDet;
	product[1] = (mat[1] * mat[11] * mat[14] + mat[2] * mat[9] * mat[15] + mat[3] * mat[10] * mat[13] - mat[1] * mat[10] * mat[15] - mat[2] * mat[11] * mat[13] - mat[3] * mat[9] * mat[14]) * invDet;
	product[2] = (mat[1] * mat[6] * mat[15] + mat[2] * mat[7] * mat[13] + mat[3] * mat[5] * mat[14] - mat[1] * mat[7] * mat[14] - mat[2] * mat[5] * mat[15] - mat[3] * mat[6] * mat[13]) * invDet;
	product[3] = (mat[1] * mat[7] * mat[10] + mat[2] * mat[5] * mat[11] + mat[3] * mat[6] * mat[9] - mat[1] * mat[6] * mat[11] - mat[2] * mat[7] * mat[9] - mat[3] * mat[5] * mat[10]) * invDet;
	product[4] = (mat[4] * mat[11] * mat[14] + mat[6] * mat[8] * mat[15] + mat[7] * mat[10] * mat[12] - mat[4] * mat[10] * mat[15] - mat[6] * mat[11] * mat[12] - mat[7] * mat[8] * mat[14]) * invDet;
	product[5] = (mat[0] * mat[10] * mat[15] + mat[2] * mat[11] * mat[12] + mat[3] * mat[8] * mat[14] - mat[0] * mat[11] * mat[14] - mat[2] * mat[8] * mat[15] - mat[3] * mat[10] * mat[12]) * invDet;
	product[6] = (mat[0] * mat[7] * mat[14] + mat[2] * mat[4] * mat[15] + mat[3] * mat[6] * mat[12] - mat[0] * mat[6] * mat[15] - mat[2] * mat[7] * mat[12] - mat[3] * mat[4] * mat[14]) * invDet;
	product[7] = (mat[0] * mat[6] * mat[11] + mat[2] * mat[7] * mat[8] + mat[3] * mat[4] * mat[10] - mat[0] * mat[7] * mat[10] - mat[2] * mat[4] * mat[11] - mat[3] * mat[6] * mat[8]) * invDet;
	product[8] = (mat[4] * mat[9] * mat[15] + mat[5] * mat[11] * mat[12] + mat[7] * mat[8] * mat[13] - mat[4] * mat[11] * mat[13] - mat[5] * mat[8] * mat[15] - mat[7] * mat[9] * mat[12]) * invDet;
	product[9] = (mat[0] * mat[11] * mat[13] + mat[1] * mat[8] * mat[15] + mat[3] * mat[9] * mat[12] - mat[0] * mat[9] * mat[15] - mat[1] * mat[11] * mat[12] - mat[3] * mat[8] * mat[13]) * invDet;
	product[10] = (mat[0] * mat[5] * mat[15] + mat[1] * mat[7] * mat[12] + mat[3] * mat[4] * mat[13] - mat[0] * mat[7] * mat[13] - mat[1] * mat[4] * mat[15] - mat[3] * mat[5] * mat[12]) * invDet;
	product[11] = (mat[0] * mat[7] * mat[9] + mat[1] * mat[4] * mat[11] + mat[3] * mat[5] * mat[8] - mat[0] * mat[5] * mat[11] - mat[1] * mat[7] * mat[8] - mat[3] * mat[4] * mat[9]) * invDet;
	product[12] = (mat[4] * mat[10] * mat[13] + mat[5] * mat[8] * mat[14] + mat[6] * mat[9] * mat[12] - mat[4] * mat[9] * mat[14] - mat[5] * mat[10] * mat[12] - mat[6] * mat[8] * mat[13]) * invDet;
	product[13] = (mat[0] * mat[9] * mat[14] + mat[1] * mat[10] * mat[12] + mat[2] * mat[8] * mat[13] - mat[0] * mat[10] * mat[13] - mat[1] * mat[8] * mat[14] - mat[2] * mat[9] * mat[12]) * invDet;
	product[14] = (mat[0] * mat[6] * mat[13] + mat[1] * mat[4] * mat[14] + mat[2] * mat[5] * mat[12] - mat[0] * mat[5] * mat[14] - mat[1] * mat[6] * mat[12] - mat[2] * mat[4] * mat[13]) * invDet;
	product[15] = (mat[0] * mat[5] * mat[10] + mat[1] * mat[6] * mat[8] + mat[2] * mat[4] * mat[9] - mat[0] * mat[6] * mat[9] - mat[1] * mat[4] * mat[10] - mat[2] * mat[5] * mat[8]) * invDet;

	return product;
}

Matrix4 Matrix4::RotateX(float angle)
{
	float fCos = Mathf::Cos(angle);
	float fSin = Mathf::Sin(angle);

	return {	1.0f,	0,		0, 0,
                0.0f,	fCos,	fSin, 0,
                0,		-fSin,	fCos, 0,
                0,		0,		0, 1.0f};
}

Matrix4 Matrix4::RotateY(float angle)
{
	float fSin = sinf(angle);
	float fCos = cosf(angle);

	return {	fCos, 0,	-fSin, 0,
                0.0f, 1.0f,  0, 0,
                fSin, 0,	 fCos, 0,
                0,    0,     0, 1.0f};
}

Matrix4 Matrix4::RotateZ(float angle)
{
	float fSin = Mathf::Sin(angle);
	float fCos = Mathf::Cos(angle);

	return {fCos, fSin, 0, 0,
            -fSin, fCos, 0, 0,
            0, 0, 1.0f, 0,
            0, 0, 0, 1.0f};
}

Matrix4 Matrix4::Rotate(const Vector3& euler)
{
	return RotateZ(euler.z) * RotateX(euler.x);// *RotateY(euler.y);
}

// I still dont get how they derived this.
Matrix4 Matrix4::Rotate(const Quaternion& q)
{
	//assert(q.IsUnit());
	Matrix4 product;

	float xs, ys, zs, wx, wy, wz, xx, xy, xz, yy, yz, zz;

	xs = q.x + q.x;
	ys = q.y + q.y;
	zs = q.z + q.z;
	wx = q.w * xs;
	wy = q.w * ys;
	wz = q.w * zs;
	xx = q.x * xs;
	xy = q.x * ys;
	xz = q.x * zs;
	yy = q.y * ys;
	yz = q.y * zs;
	zz = q.z * zs;

	product[0] = 1.0f - (yy + zz);
	product[4] = xy - wz;
	product[8] = xz + wy;
	product[12] = 0.0f;

	product[1] = xy + wz;
	product[5] = 1.0f - (xx + zz);
	product[9] = yz - wx;
	product[13] = 0.0f;

	product[2] = xz - wy;
	product[6] = yz + wx;
	product[10] = 1.0f - (xx + yy);
	product[14] = 0.0f;

	product[3] = 0.0f;
	product[7] = 0.0f;
	product[11] = 0.0f;
	product[15] = 1.0f;

	return product;
}

Matrix4 Matrix4::LookAt(const Vector3& from, const Vector3& to, const Vector3& up)
{
	Vector3 z = (to - from).Normalized();
	Vector3 x = Vector3::Cross(up, z).Normalized();
	Vector3 y = Vector3::Cross(z, x);

	return {	x.x, y.x, z.x, 0.0f,
                x.y, y.y, z.y, 0.0f,
                x.z, y.z, z.z, 0.0f,
                -Vector3::Dot(x, from), -Vector3::Dot(y, from), -Vector3::Dot(z, from), 1.0f};
}

Matrix4 Matrix4::Orthographic(int width, int height, float zNear, float zFar)
{
	if (zNear  == zFar)	{ zFar += 0.1f;	}
	if (width  <= 0)	{ width = 1;	}
	if (height <= 0)    { height = 1;	}

	return {	2.0f / width, 0, 0, 0,
                0, 2.0f / height, 0, 0,
                0, 0, 1.0f / (zFar - zNear), 0,
                0, 0, -zNear / (zNear - zFar), 1.0f};
}

Matrix4 Matrix4::PerspectiveFov(float fov, float aspect, float zNear, float zFar)
{
	float tanY = Mathf::Tan(fov * 0.5f);
	if (Mathf::IsZero(tanY)) { tanY = 0.001f; }
	float yScale = 1.0f / tanY; // Cotangent, Gets smaller as angle is bigger, and bigger as smaller, essentially a multiplicative inverse

	if (Mathf::IsZero(aspect)) aspect = 0.001f;
	float xScale = yScale / aspect; // X is modified by aspect ratio

	if (0.0f == aspect) { aspect = 0.001f; }
	if (zNear == zFar)   zFar = zNear + 0.1f;

	float range = zFar / (zFar - zNear);

	return {	xScale, 0, 0, 0,
                0, yScale, 0, 0,
                0, 0, range, 1.0f,
                0, 0, -range * zNear, 0};
}

Matrix4 Matrix4::Perspective(float width, float height, float zNear, float zFar)
{
	float twoNear = zNear * zNear;
	float range = zFar / (zFar - zNear);

	return {	twoNear / width, 0.0f, 0.0f, 0.0f,
                0.0f, twoNear / height, 0.0f, 0.0f,
                0.0f, 0.0f, range, 1.0f,
                0.0f, 0.0f, -range * zNear, 0.0f};
}

Matrix4 Matrix4::PerspectiveOffCenter(float left, float right, float top, float bottom, float zNear, float zFar)
{
	float twoNear = zNear + zNear;
	float recipWidth = 1.0f / (right - left);
	float recipHeight = 1.0f / (top - bottom);
	float range = zFar / (zFar - zNear);

	return {	twoNear * recipWidth, 0.0f, 0.0f, 0.0f,
                0.0f, twoNear * recipHeight, 0.0f, 0.0f,
                -(left + right) * recipWidth, -(top + bottom) * recipHeight, range, 1.0f,
                0.0f, 0.0f, -range * zNear, 0.0f};
}

Matrix4 Matrix4::Scale(const Vector3& scale)
{
	return {	scale.x,0,0,0,	// Col 1
                0,scale.y,0,0,	// Col 2
                0,0,scale.z,0,	// Col 3
                0, 0, 0, 1};		// Col 4
}

Matrix4 Matrix4::Translate(const Vector3& vector)
{
	return {	1,0,0,0,
                0,1,0,0,
                0,0,1,0,
                vector.x, vector.y, vector.z, 1.0f};
}

Matrix4 Matrix4::Transpose(const Matrix4& mat)
{
	Matrix4 transpose;
	for (unsigned int row = 0; row < 4; ++row)
	{
		for (unsigned int col = 0; col < 4; ++col)
		{
			transpose.m[MAT4_I(row, col)] = mat.m[MAT4_I(col, row)];
		}
	}

	return transpose;
}

Matrix3 Matrix4::Get3x3(const Matrix4& mat, unsigned int row, unsigned int col)
{
	// Row column elimination to find the 3x3 for detemrinant.
	Matrix3 product;
	int index = 0;

	for (unsigned int i = 0; i < 4; i++)
	{
		for (unsigned int j = 0; j < 4; j++)
		{
			if (i == row || j == col)
			{
				//skip
				continue;
			}

			product[++index] = mat.m[MAT4_I(row, col)];
		}
	}
	return product;
}
