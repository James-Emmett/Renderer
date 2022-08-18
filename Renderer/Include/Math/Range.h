#pragma once
#include "Mathf.h"

struct Range
{
private:
	int m_Min = 0;
	int m_Max = 0;
	int m_Length = 0;
	int m_Radius = 0;
	int m_Centre = 0;

public:
	Range(int min, int max)
	{
		if (min > max)
		{
			m_Min = max;
			m_Max = min;
		}
		else
		{
			m_Min = min;
			m_Max = max;
		}

		m_Length = Mathf::Abs(m_Max - m_Min);
		m_Radius = (int)(m_Length * 0.5f);
		m_Centre = m_Min + m_Radius;
	}

	int Min()const { return m_Min; }
	int Max()const { return m_Max; }
	int Length()const { return m_Length; }
	int Radius()const { return m_Radius; }
	int Centre()const { return m_Centre; }
	bool  Contains(int value)const { return ((value >= m_Min) && (value <= m_Max)); }
};

struct Rangef
{
private:
	float m_Min = 0.0f;
	float m_Max = 0.0f;
	float m_Length = 0.0f;
	float m_Radius = 0.0f;
	float m_Centre = 0.0f;

public:
	Rangef(float min, float max)
	{
		if (min > max)
		{
			m_Min = max;
			m_Max = min;
		}
		else
		{
			m_Min = min;
			m_Max = max;
		}

		m_Length = Mathf::Abs(m_Max - m_Min);
		m_Radius = m_Length * 0.5f;
		m_Centre = m_Min + m_Radius;
	}

	float Min()const { return m_Min; }
	float Max()const { return m_Max; }
	float Length()const { return m_Length; }
	float Radius()const { return m_Radius; }
	float Centre()const { return m_Centre; }
	bool  Contains(float value)const { return ((value >= m_Min) && (value <= m_Max)); }
};