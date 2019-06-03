#include "stdafx.h"
#include "FFT.h"

#include <memory>
#include <cmath>

#define DLL_EXPORT(type) extern "C" __declspec(dllexport) type __stdcall

using namespace std;
using namespace Algorithms;

///<summary>计算最大值最小值平均值</summary>
///<param name="t_Data">数据</param>
///<param name="t_Length">数据长度</param>
///<param name="t_Max">最大值</param>
///<param name="t_Min">最小值</param>
///<param name="t_Average">平均值</param>
DLL_EXPORT(void) MaxMinAverage(const float* t_Data, UINT t_Length, float* t_Max, float* t_Min, float* t_Average)
{
	auto t_Index = 0u;
	auto t_Temp = t_Data[t_Index];
	auto t_MinValue = t_Temp;
	auto t_MaxValue = t_Temp;
	double t_Sum = t_Temp;
	for (++t_Index, ++t_Data; t_Index != t_Length; ++t_Index, ++t_Data)
	{
		t_Temp = *t_Data;
		if (t_Temp < t_MinValue)
		{
			t_MinValue = t_Temp;
		}
		else if (t_Temp > t_MaxValue)
		{
			t_MaxValue = t_Temp;
		}
		t_Sum += t_Temp;
	}
	*t_Max = t_MaxValue;
	*t_Min = t_MinValue;
	*t_Average = static_cast<float>(t_Sum / static_cast<double>(t_Length));
}

///<summary>计算标准差</summary>
///<param name="t_Data">数据</param>
///<param name="t_Length">数据长度</param>
///<param name="t_Average">平均值</param>
///<param name="t_SD">标准差</param>
DLL_EXPORT(void) StandardDeviation(const float* t_Data, UINT t_Length, float t_Average, float* t_SD)
{
	float t_Temp;
	double t_Sum = 0.0;
	for (auto t_Index = 0u; t_Index != t_Length; ++t_Index, ++t_Data)
	{
		t_Temp = *t_Data;
		t_Temp -= t_Average;
		t_Temp *= t_Temp;
		t_Sum += t_Temp;
	}
	t_Sum = t_Sum / static_cast<double>(t_Length);
	*t_SD = static_cast<float>(sqrt(t_Sum));
}

///<summary>快速傅立叶变换</summary>
///<param name="t_Data">数据</param>
///<param name="t_Length">数据长度（必须大于等于8且为2的次方）</param>
DLL_EXPORT(void) FastFourierTransform(float* t_Data, UINT t_Length)
{
	if (t_Length >= 8u && (t_Length & (t_Length - 1u)) == 0u)
	{
		auto const t_Complex = make_unique<Complex[]>(t_Length);
		auto t_Temp = t_Data;
		auto t_Array = t_Complex.get();
		for (auto t_Index = 0u; t_Index != t_Length; ++t_Index, ++t_Array, ++t_Temp)
		{
			t_Array->Real = *t_Temp;
		}
		FFT(t_Complex.get(), t_Length);
		auto const t_Size = static_cast<double>(t_Length);
		t_Temp = t_Data;
		t_Array = t_Complex.get();
		*t_Temp = 0.0f; //直流分量：static_cast<float>(sqrt(pow(t_Array->Real, 2.0) + pow(t_Array->Imag, 2.0)) / t_Size);
		++t_Temp;
		++t_Array;
		for (auto t_Index = 1u; t_Index != t_Length; ++t_Index, ++t_Array, ++t_Temp)
		{
			*t_Temp = static_cast<float>(sqrt(pow(t_Array->Real, 2.0) + pow(t_Array->Imag, 2.0)) * 2.0 / t_Size);
		}
	}
}