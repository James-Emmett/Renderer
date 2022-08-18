#include "Math/Matrix3.h"
#include "Math/Vector3.h"
#include "Math/Mathf.h"
#include "Math/Quaternion.h"

// Matrix Layed out in columns.
#define MAT3_I(row, col) (row + (col * 3))

const Matrix3 Matrix3::Identity = Matrix3(1,0,0, 0,1,0, 0,0,1);
const Matrix3 Matrix3::Zero = Matrix3(0,0,0,0,0,0,0,0,0);

Matrix3::Matrix3()
{
	m[0] = 0.0f; m[1] = 0.0f; m[2] = 0.0f; // Col 1
	m[3] = 0.0f; m[4] = 0.0f; m[5] = 0.0f; // Col 2
	m[6] = 0.0f; m[7] = 0.0f; m[8] = 0.0f; // Col 3
}

Matrix3::Matrix3(float m00, float m10, float m20,
				 float m01, float m11, float m21,
				 float m02, float m12, float m22)
{
	m[0] = m00; m[1] = m10; m[2] = m20; // Col 1
	m[3] = m01; m[4] = m11; m[5] = m21; // Col 2
	m[6] = m02; m[7] = m12; m[8] = m22; // Col 3
}

Matrix3::Matrix3(const Vector3& x, const Vector3& y, const Vector3& z)
{
	m[0] = x.x; m[1] = x.y; m[2] = x.z; // Col 1
	m[3] = y.x; m[4] = y.y; m[5] = y.z; // Col 2
	m[6] = z.x; m[7] = z.y; m[8] = z.z; // Col 3
}

Matrix3::Matrix3(const Matrix3& other)
{
	m[0] = other.m[0]; m[1] = other.m[1]; m[2] = other.m[2]; // Col 1
	m[3] = other.m[3]; m[4] = other.m[4]; m[5] = other.m[5]; // Col 2
	m[6] = other.m[6]; m[7] = other.m[7]; m[8] = other.m[8]; // Col 3
}

bool Matrix3::operator==(const Matrix3& mat) const
{
	for (unsigned int i = 0; i < 9; ++i)
	{
		if (!Mathf::IsEqual(m[i], mat.m[i]))
		{
			return false;
		}
	}
	return true;
}

bool Matrix3::operator!=(const Matrix3& mat) const
{
	for (unsigned int i = 0; i < 9; ++i)
	{
		if (Mathf::IsEqual(m[i], mat.m[i]))
		{
			return false;
		}
	}
	return true;
}

float& Matrix3::operator[](int i)
{
	return m[i];
}

float Matrix3::operator[](int i) const
{
	return m[i];
}

float& Matrix3::operator()(int row, int col)
{
    return m[MAT3_I(row, col)];
}

const float& Matrix3::operator()(int row, int col) const
{
    return m[MAT3_I(row, col)];
}

Matrix3& Matrix3::operator=(const Matrix3& mat)
{
    if (this != &mat)
    {
        m[0] = mat[0]; m[3] = mat[3]; m[6] = mat[6];
        m[1] = mat[1]; m[4] = mat[4]; m[7] = mat[7];
        m[2] = mat[2]; m[5] = mat[5]; m[8] = mat[8];
    }

    return *this;
}

Matrix3 Matrix3::operator+(const Matrix3& mat) const
{
    return { m[0] + mat[0], m[1] + mat[1], m[2] + mat[2],
                    m[3] + mat[3], m[4] + mat[4], m[5] + mat[5],
                    m[6] + mat[6], m[7] + mat[7], m[8] + mat[8]};
}

Matrix3 Matrix3::operator-(const Matrix3& mat) const
{
    return { m[0] - mat[0], m[1] - mat[1], m[2] - mat[2],
                    m[3] - mat[3], m[4] - mat[4], m[5] - mat[5],
                    m[6] - mat[6], m[7] - mat[7], m[8] - mat[8]};
}

Matrix3 Matrix3::operator*(const Matrix3& mat) const
{
    Matrix3 product;
    for (int r = 0; r < 3; ++r)
    {
        for (int c = 0; c < 3; ++c)
        {
            int i = MAT3_I(r, c);
            product.m[i] = 0.0f;
            for (int mid = 0; mid < 3; ++mid)
            {
                product.m[i] += m[MAT3_I(r, mid)] * mat.m[MAT3_I(mid, c)];
            }
        }
    }
    return product;
}

Matrix3 Matrix3::operator*(float s)
{
    return { m[0] * s, m[1] * s, m[2] * s,
                    m[3] * s, m[4] * s, m[5] * s,
                    m[6] * s, m[7] * s, m[8] * s};
}

Matrix3& Matrix3::operator+=(const Matrix3& mat)
{
    m[0] += mat[0]; m[3] += mat[3]; m[6] += mat[6];
    m[1] += mat[1]; m[4] += mat[4]; m[7] += mat[7];
    m[2] += mat[2]; m[5] += mat[5]; m[8] += mat[8];

    return *this;
}

Matrix3& Matrix3::operator-=(const Matrix3& mat)
{
    m[0] -= mat[0]; m[3] -= mat[3]; m[6] -= mat[6];
    m[1] -= mat[1]; m[4] -= mat[4]; m[7] -= mat[7];
    m[2] -= mat[2]; m[5] -= mat[5]; m[8] -= mat[8];

    return *this;
}

Matrix3& Matrix3::operator*=(const Matrix3& mat)
{
    Matrix3 product = *this;

    for (int row = 0; row < 4; row++)
    {
        for (int col = 0; col < 4; col++)
        {
            int i = MAT3_I(row, col);
            m[i] = 0.0f;
            for (int mid = 0; mid < 4; mid++)
            {
                m[i] += product.m[MAT3_I(row, mid)] * mat.m[MAT3_I(mid, col)];
            }
        }
    }

    return *this;
}

Matrix3& Matrix3::operator*=(float s)
{
    for (size_t i = 0; i < 9; ++i)
    {
        m[i] *= s;
    }
    return *this;
}

Vector3& Matrix3::Column(int i)
{
	return *(Vector3*)(&m[i * 3]);
}

const Vector3& Matrix3::Column(int i)const
{
	return *(Vector3*)(&m[i * 3]);
}

void Matrix3::SetColumn(int i, const Vector3& column)
{
	i *= 3;
	m[i] = column.x; m[i + 1] = column.y; m[i + 2] = column.z;
}

float Matrix3::Trace() const
{
	return m[0] + m[4] + m[8];
}

Matrix3 Matrix3::Minor(const Matrix3& mat)
{
    return Matrix3( (mat[4] * mat[8] - mat[5] * mat[7]),
                    (mat[3] * mat[8] - mat[5] * mat[6]),
                    (mat[3] * mat[7] - mat[4] * mat[6]),

                    (mat[1] * mat[8] - mat[2] * mat[7]),
                    (mat[0] * mat[8] - mat[2] * mat[6]),
                    (mat[0] * mat[7] - mat[1] * mat[6]),

                    (mat[1] * mat[5] - mat[2] * mat[4]),
                    (mat[0] * mat[5] - mat[2] * mat[3]),
                    (mat[0] * mat[4] - mat[1] * mat[3]));
}

Matrix3 Matrix3::Cofactor(const Matrix3& mat)
{
    // Matrix of Minors + Lattice of +,- for cofactor.
    return { Mathf::Abs((mat[4] * mat[8] - mat[5] * mat[7])),
                    -(mat[3] * mat[8] - mat[5] * mat[6]),
                    Mathf::Abs((mat[3] * mat[7] - mat[4] * mat[6])),

                    -(mat[1] * mat[8] - mat[2] * mat[7]),
                    Mathf::Abs((mat[0] * mat[8] - mat[2] * mat[6])),
                    -(mat[0] * mat[7] - mat[1] * mat[6]),

                    Mathf::Abs((mat[1] * mat[5] - mat[2] * mat[4])),
                    -(mat[0] * mat[5] - mat[2] * mat[3]),
                    Mathf::Abs((mat[0] * mat[4] - mat[1] * mat[3]))};
}

Matrix3 Matrix3::Adjoint(const Matrix3& mat)
{
    //Claculate the Matrix of minors, apply the
    //lattice of + and - (cofactor) then transpose.
    //I do all in 1 step!

    return { Mathf::Abs((mat[4] * mat[8] - mat[5] * mat[7])),
                    -(mat[1] * mat[8] - mat[2] * mat[7]), 
                    Mathf::Abs((mat[1] * mat[5] - mat[2] * mat[4])),
                    
                    -(mat[3] * mat[8] - mat[5] * mat[6]),
                    Mathf::Abs((mat[0] * mat[8] - mat[2] * mat[6])),
                    -(mat[0] * mat[5] - mat[2] * mat[3]),

                    Mathf::Abs((mat[3] * mat[7] - mat[4] * mat[6])),
                    -(mat[0] * mat[7] - mat[1] * mat[6]),
                    Mathf::Abs((mat[0] * mat[4] - mat[1] * mat[3]))};
}

//http://www.cg.info.hiroshima-cu.ac.jp/~miyazaki/knowledge/teche23.html
float Matrix3::Determinant(const Matrix3& mat)
{
    return  mat[0] * (mat[4] * mat[8] - mat[5] * mat[7])
        - mat[1] * (mat[3] * mat[8] - mat[5] * mat[6])
        + mat[2] * (mat[3] * mat[7] - mat[4] * mat[6]);
}

Matrix3 Matrix3::Inverse(const Matrix3& mat)
{
    float det = mat[0] * (mat[4] * mat[8] - mat[5] * mat[7])
        - mat[1] * (mat[3] * mat[8] - mat[5] * mat[6])
        + mat[2] * (mat[3] * mat[7] - mat[4] * mat[6]);

    Matrix3 product;

    // Matrix fo minors, cofactor'd  with Adjoint
    product[0] = Mathf::Abs((mat[4] * mat[8] - mat[5] * mat[7]));
    product[3] = -(mat[3] * mat[8] - mat[5] * mat[6]);
    product[6] = Mathf::Abs((mat[3] * mat[7] - mat[4] * mat[6]));

    product[1] = -(mat[1] * mat[8] - mat[2] * mat[7]);
    product[4] = Mathf::Abs((mat[0] * mat[8] - mat[2] * mat[6]));
    product[7] = -(mat[0] * mat[7] - mat[1] * mat[6]);

    product[2] = Mathf::Abs((mat[1] * mat[5] - mat[2] * mat[4]));
    product[5] = -(mat[0] * mat[5] - mat[2] * mat[3]);
    product[8] = Mathf::Abs((mat[0] * mat[4] - mat[1] * mat[3]));

    return product * (1.0f / det);
}

Matrix3 Matrix3::RotateX(float angle)
{
    float fSin = Mathf::Sin(angle);
    float fCos = Mathf::Cos(angle);

    return { 1.0f, 0.0f, 0.0f,
                    0.0f, fCos, fSin,
                    0.0f, -fSin, fCos};
}

Matrix3 Matrix3::RotateY(float angle)
{
    float fSin = Mathf::Sin(angle);
    float fCos = Mathf::Cos(angle);

    return { fCos, 0.0f, -fSin,
                    0.0f, 1.0f, 0.0f,
                    fSin, 0.0f, fCos};
}

Matrix3 Matrix3::RotateZ(float angle)
{
    float fCos = Mathf::Cos(angle);
    float fSin = Mathf::Sin(angle);

    return { fCos, fSin, 0.0f,
                    -fSin, fCos, 0.0f,
                    0.0f, 0.0f, 1.0f};
}

Matrix3 Matrix3::Rotate(const Vector3& euler)
{
    return RotateZ(euler.z) * RotateX(euler.x) * RotateY(euler.y);
}

Matrix3 Matrix3::Rotate(const Quaternion& q)
{
    Matrix3 product;

    float s, xs, ys, zs, wx, wy, wz, xx, xy, xz, yy, yz, zz;

    // Normalised rotation s = 2.0f as 2.0/1.0f = 2.0f;
    s = 2.0f / (q.x * q.x + q.y * q.y + q.z * q.z + q.w * q.w);

    xs = s * q.x;  ys = s * q.y; zs = s * q.z;
    wx = xs * q.w; wy = ys * q.w; wz = zs * q.w;
    xx = xs * q.x; xy = ys * q.x; xz = zs * q.x;
    yy = ys * q.y; yz = zs * q.y; zz = zs * q.z;

    product[0] = 1.0f - (yy + zz);
    product[3] = xy - wz;
    product[6] = xz + wy;

    product[1] = xy + wz;
    product[4] = 1.0f - (xx + zz);
    product[7] = yz - wx;

    product[2] = xz - wy;
    product[5] = yz - wx;
    product[8] = 1.0f - (xx + yy);

    return product;
}

Matrix3 Matrix3::Scale(const Vector3& scale)
{
    return Matrix3( scale.x, 0.0f, 0.0f,
                    0.0f, scale.y, 0.0f,
                    0.0f, 0.0f, scale.z);
}

Matrix3 Matrix3::Transpose(const Matrix3& mat)
{
    return Matrix3( mat[0], mat[3], mat[6],
                    mat[1], mat[4], mat[7],
                    mat[2], mat[5], mat[8]);
}

Vector3 Matrix3::Transform(const Vector3& vector) const
{
    return Vector3(vector.x * m[0] + vector.y * m[3] + vector.z * m[6],
                   vector.x * m[1] + vector.y * m[4] + vector.z * m[7],
                   vector.x * m[2] + vector.y * m[5] + vector.z * m[8]);
}
