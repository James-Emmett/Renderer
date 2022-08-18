#include "Math/Color.h"
#include "Math/Vector4.h"
#include "Math/Mathf.h"

// Reciprical of 255, 1.0f / 255.0f multiply to normalise byte colors
#define From255 0.00392156862f

const Color Color::White = Color(1, 1, 1, 1);
const Color Color::Black = Color(0, 0, 0, 1);
const Color Color::Clear = Color(0, 0, 0, 0);
const Color Color::Red = Color(1, 0, 0, 1);
const Color Color::Blue = Color(0, 0, 1, 1);
const Color Color::Green = Color(0, 1, 0, 1);
const Color Color::Orange = Color(1.0f, 0.6f, 0.0f, 1.0f);
const Color Color::Purple = Color(1, 0, 1, 1);
const Color Color::Yellow = Color(1, 1, 0, 1);
const Color Color::Pink = Color(1.0f, 0.2f, 0.6f, 1.0f);
const Color Color::Aqua = Color(0, 1, 1, 1);
const Color Color::Grey = Color(0.45f, 0.45f, 0.45f, 1.0f);
const Color Color::CornflowerBlue = Color(0.392f, 0.584f, 0.929f);

Color::Color() : 
	r(0.0f), g(0.0f), 
	b(0.0f), a(1.0f)
{
}

Color::Color(float R, float G, float B, float A) : 
	r(R), g(G), 
	b(B), a(A)
{
}

Color::Color(const Vector4& vector) :
	r(vector.x), g(vector.y),
	b(vector.z), a(vector.w)
{
}

Color::Color(const Color& color) :
	r(color.r), g(color.g),
	b(color.b), a(color.a)
{
}

Color::~Color()
{
}

Color& Color::operator=(const Color& color)
{
	r = color.r; g = color.g;
	b = color.b; a = color.a;
	return *this;
}

Color& Color::operator=(const Vector4& vector)
{
	r = vector.x; g = vector.y;
	b = vector.z; a = vector.w;
	return *this;
}

Color Color::operator+(const Color& other) const
{
	return Color(r + other.r, g + other.g, b + other.b, a + other.a);
}

Color Color::operator-(const Color& other) const
{
	return Color(r - other.r, g - other.g, b - other.b, a - other.a);
}

Color Color::operator*(const Color& other) const
{
	return Color(r * other.r, g * other.g, b * other.b, a * other.a);
}

Color Color::operator*(const float s) const
{
	return Color(r * s, g * s,
				 b * s, a * s);
}

Color Color::operator/(float s) const
{
	s = 1.0f / s;
	return Color(r * s, g * s, b * s, a * s);
}

Color& Color::operator+=(const Color& other)
{
	r = r + other.r;
	g = g + other.g;
	b = b + other.b;
	a = a + other.a;

	return *this; // Return the ivalue!
}

Color& Color::operator-=(const Color& other)
{
	r = r - other.r;
	g = g - other.g;
	b = b - other.b;
	a = a - other.a;

	return *this; // Return the ivalue!
}

Color& Color::operator*=(const Color& other)
{
	r = r * other.r;
	g = g * other.g;
	b = b * other.b;
	a = a * other.a;

	return *this; // Return the ivalue!
}

Color& Color::operator/=(float s)
{
	float recip = 1.0f / s;
	r = r * recip;
	g = g * recip;
	b = b * recip;
	a = a * recip;

	return *this; // Return the ivalue!
}

bool Color::operator==(const Color& color)const
{
	return	r == color.r && g == color.g &&
			g == color.b && a == color.a;
}

bool Color::operator!=(const Color& color)const
{
	return !(r == color.r && g == color.g &&
			b == color.b && a == color.a);
}

Color operator*(float s, const Color& color)
{
	return Color(color.r * s, color.g * s,
				 color.b * s, color.a * s);
}


Color Color::Linear() const
{
	return Color(Mathf::ToLinear(r), Mathf::ToLinear(g), Mathf::ToLinear(b), a);
}

Color Color::SRGB() const
{
	return Color(Mathf::ToSrgbFast(r), Mathf::ToSrgbFast(g), Mathf::ToSrgbFast(b), a);
}

Color Color::GrayScale() const
{
	float f = (r * 0.3f) + (g * 0.59f) + (b * 0.11f);
	return Color(f, f, f, a);
}

float Color::Intensity() const
{
	return (r + g + b) / 3.0f;
}

//  https://www.cs.rit.edu/~ncs/color/t_convert.html
Color Color::HSVToRGB(float H, float S, float V)
{
	Color col;

	int i;
	float f, p, q, t;

	if (S == 0)
	{
		//Achromatic, ignore H.
		return Color(V, V, V, 1.0f);
	}

	//Undo the degree conversion
	H /= 60;
	i = (int)Mathf::Floor(H); //Remove decimal.
	f = H - i; //Factorial of H
	p = V * (1 - S);
	q = V * (1 - S * f);
	t = V * (1 - S * (1 - f));

	switch (i)
	{
	case 0:
		col.r = V;
		col.g = t;
		col.b = p;
		break;
	case 1:
		col.r = q;
		col.g = V;
		col.b = p;
		break;
	case 2:
		col.r = p;
		col.g = V;
		col.b = t;
		break;
	case 3:
		col.r = p;
		col.g = q;
		col.b = V;
		break;
	case 4:
		col.r = t;
		col.g = p;
		col.b = V;
		break;
	default: //5
		col.r = V;
		col.g = p;
		col.b = q;
		break;
	}

	return col;
}

//  https://www.cs.rit.edu/~ncs/color/t_convert.html
void Color::RGBToHSV(const Color& col, float& h, float& s, float& v)
{
	// Find min and max of the RGB color space.
	float min, max, delta;
	min = Mathf::Min(col.r, Mathf::Min(col.g, col.b));
	max = Mathf::Max(col.r, Mathf::Max(col.g, col.b));
	delta = max - min;

	//Value = the max intensity of the color space
	v = max;

	if (max != 0)
	{
		// S is the difference between color intensity.
		s = delta / max;
	}
	else
	{
		// Undefined as color is 0,0,0
		s = 0;
		h = 0; // Opt for 0 as to not crash with unecpected -1's
		return;
	}

	// Find the delta for the colors H
	if (col.r == max)
	{
		h = (col.g - col.b);
	}
	else if (col.g == max)
	{
		h = 2 + (col.b - col.r);
	}
	else
	{
		h = 4 + (col.r - col.g);
	}

	// Find the degrees
	h *= 60;

	if (h < 0)
	{
		h += 360;
	}
}

Color Color::Lerp(const Color& A, const Color& B, float t)
{
	float invT = (1.0f - t);
	return Color(invT * A.r + t * B.r,
		invT * A.g + t * B.g,
		invT * A.b + t * B.b,
		invT * A.a + t * B.a);
}