#pragma once


struct ViewPort
{
	float X = 0;
	float Y = 0;
	float Width = 0;
	float Height = 0;
	float MinDepth = 0;
	float MaxDepth = 1; // Inverse this for better percision?

	ViewPort(float x, float y, float width, float height, float minDepth = 0.0f, float maxDepth = 1.0f)
	{
		X = x; Y = y;
		Width = width; Height = height;
		MinDepth = minDepth; MaxDepth = maxDepth;
	}

	float AspectRatio()
	{
		return Width / Height;
	}
};