#pragma once

struct MultiSampleDesc
{
	u32 Count = 1;
	u32 Quality = 0;

	MultiSampleDesc(u32 count = 1, u32 quality = 0)
	{
		Count = count;
		Quality = quality;
	}
};