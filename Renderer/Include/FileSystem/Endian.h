#pragma once

enum class Endian
{
	Little, // 90% of cases are this
	Big
};

// I saw this trick in Noel Berrys Blah code base, but his has an error
// it should be AB == 0x4142 not 0x4243 as below.
inline bool IsBigEndian()
{
	return (*((int*)"AB")) == 0x4142;
}

inline bool IsLittleEndian()
{
	return (*((int*)"AB")) != 0x4142;
}

inline Endian GetSystemEndian()
{
	return (*((int*)"AB")) != 0x4142 ? Endian::Little : Endian::Big;
}