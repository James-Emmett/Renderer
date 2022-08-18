//
// Created by james on 02/06/2022.
//

#include "Math/Mathf.h"

namespace Mathf
{
    float ToRadians(float deg)
    {
        return deg * DEG_TO_RAD;
    }

    float ToDegrees(float rad)
    {
        return rad * RAD_TO_DEG;
    }

    float Clamp(float value, float min, float max)
    {
        if (value >= max)
        {
            return max;
        }

        if (value <= min)
        {
            return min;
        }

        return value;
    }

    float Clamp01(float value)
    {
        if (value >= 1)
        {
            return 1.0f;
        }

        if (value <= 0)
        {
            return 0.0f;
        }

        return value;
    }

    int Clamp(int value, int min, int max)
    {
        if (value >= max)
        {
            return max;
        }

        if (value <= min)
        {
            return min;
        }

        return value;
    }

    int Clamp01(int value)
    {
        if (value >= 1)
        {
            return 1;
        }

        if (value <= 0)
        {
            return 0;
        }

        return value;
    }

    unsigned int Clamp(unsigned int value, unsigned int min, unsigned int max)
    {
        if (value >= max)
        {
            return max;
        }

        if (value <= min)
        {
            return min;
        }

        return value;
    }

    unsigned int Clamp01(unsigned int value)
    {
        if (value >= 1)
        {
            return 1;
        }

        if (value <= 0)
        {
            return 1;
        }

        return value;
    }

    float Max(float a, float b)
    {
        return (a > b) ? a : b;
    }

    int Max(int a, int b)
    {
        return (a > b) ? a : b;
    }

    u32 Max(unsigned int a, unsigned int b)
    {
        return (a > b) ? a : b;
    }

    float Min(float a, float b)
    {
        return (a < b) ? a : b;
    }

    int Min(int a, int b)
    {
        return (a < b) ? a : b;
    }

    u32 Min(u32 a, u32 b)
    {
        return (a < b) ? a : b;
    }

    float Sin(float a)
    {
        return sinf(a);
    }

    float Cos(float a)
    {
        return cosf(a);
    }

    float Tan(float a)
    {
        return tanf(a);
    }

    float Asin(float x)
    {
        return asinf(x);
    }

    float Acos(float x)
    {
        return acosf(x);
    }

    float Atan(float x)
    {
        return atanf(x);
    }

    float Atan2(float y, float x)
    {
        return atan2f(y, x);
    }

    unsigned int Abs(int x)
    {
        return abs(x);
    }

    float Abs(float x)
    {
        return fabsf(x);
    }

    float Sqrt(float x)
    {
        return sqrtf(x);
    }

    float RecipSqrt(float x)
    {
        return 1.0f / sqrtf(x);
    }

    float Log2(float x)
    {
        return log2f(x);
    }

    float Ceil(float x)
    {
        return ceilf(x);
    }

    float Floor(float x)
    {
        return floorf(x);
    }

    float Pow(float value, float exponent)
    {
        return powf(value, exponent);
    }

    bool IsInfinite(float x)
    {
        return isinf(x);
    }

    bool IsNan(float x)
    {
        return isnan(x);
    }

    bool IsZero(float x)
    {
        return (Abs(x) <= EPSILON_F);
    }

    // More robust than standard Epsilon Equal!!!
    bool IsEqual(float a, float b)
    {
        // First check with fixed Epsilon to rule otu smaller values
        float diff = Abs(a - b);
        if (diff <= EPSILON_F)
        {
            return true;
        }

        // If that fails then rule out larger floats using a relative Epsilon!
        return diff <= EPSILON_F * Max(Abs(a), Abs(b));
    }

    bool IsEven(int x)
    {
        return (x & 1) == 0;
    }

    bool IsOdd(int x)
    {
        return (x & 1) == 1;
    }

    float Lerp(float v1, float v2, float t)
    {
        t = Clamp01(t);
        return (1 - t) * v1 + (v2 * t);
    }

    float UnclampedLerp(float v1, float v2, float t)
    {
        return (1 - t) * v1 + (v2 * t);
    }

    float SmoothStep(float min, float max, float t)
    {
        t = Clamp01((t - min) / (max - min));
        return t * t * (3 - 2 * t);
    }

    float SmoothStepKP(float min, float max, float t)
    {
        t = Clamp01((t - min) / (max - min));
        return t * t * t * (t * (t * 6 - 15) + 10);
    }

    float CatmullRom(float A, float B, float C, float D, float t)
    {
        float tSqr = t * t;
        float tCubed = tSqr * t;
        return (float)(0.5f * (2.0f * B + (C - A) * t +
                               (2.0f * A - 5.0f * B + 4.0f * C - D) * tSqr +
                               (3.0f * B - A - 3.0f * C + D) * tCubed));
    }

    float Bezier(float A, float B, float C, float t)
    {
        t = Clamp01(t);
        float	oneMinusT = 1.0f - t;
        return	oneMinusT * oneMinusT * A +
                  2.0f * oneMinusT * t * B +
                  t * t * C;
    }

    float BezierCubic(float A, float B, float C, float D, float t)
    {
        t = Clamp01(t);
        float	oneMinusT = 1.0f - t;
        return	oneMinusT * oneMinusT * oneMinusT * A +
                  3.0f * oneMinusT * oneMinusT * t * B +
                  3.0f * oneMinusT * t * t * C +
                  t * t * t * D;
    }

    int Sign(float value)
    {
        return (value > 0) - (value < 0);
    }

    int Sign(int value)
    {
        return (value > 0) - (value < 0);
    }

    u32 AlignSize(u32 size, u32 alignment)
    {
        return ((size + alignment - 1) / alignment) * alignment;
    }

    u64 AlignSize(u64 size, u64 alignment)
    {
        return ((size + alignment - 1) / alignment) * alignment;
    }

    //http://graphics.stanford.edu/~seander/bithacks.html
    u32 NextPowerOfTwo(u32 v)
    {
        // https://graphics.stanford.edu/~seander/bithacks.html#RoundUpPowerOf2
        // This works by filling all the bits lower than the highest bit with 1's
        // then adding 1 at the end forces it to roll over to 1 in the next slot and 0's
        // for the rest of the binary value for instance: 00011111 + 1 = 00100000

        // But why the v--? still unsure!!!
        v--;
        v |= v >> 1;
        v |= v >> 2;
        v |= v >> 4;
        v |= v >> 8;
        v |= v >> 16;
        return v + 1;
    }

    u32 PreviousPowerOfTwo(u32 v)
    {
        return NextPowerOfTwo(v + 1) / 2;
    }

    u32 NearestPowerOfTwo(u32 v)
    {
        const u32 np2 = NextPowerOfTwo(v);
        const u32 pp2 = PreviousPowerOfTwo(v);

        // Find the smallest delta and return accordingly.
        return ((np2 - v) <= (v - pp2)) ? np2 : pp2;
    }

    float ToSrgbFast(float f)
    {
        // Close Approximation to SRGB
        f = Clamp01(f);
        float s1 = Sqrt(f);
        float s2 = Sqrt(s1);
        float s3 = Sqrt(s2);
        return 0.662002687f * s1 + 0.684122060f * s2 - 0.323583601f * s3 - 0.0225411470f * f;
    }

    float ToLinear(float f)
    {
        f = Clamp01(f);
        return f * (f * (f * 0.305306011f + 0.682171111f) + 0.012522878f);
    }
}