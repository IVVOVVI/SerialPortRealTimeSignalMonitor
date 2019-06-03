#include "stdafx.h"
#include "FFT.h"

#define _USE_MATH_DEFINES

#include <math.h>
#include <cmath>
#include <memory>

using namespace std;

void Algorithms::FFT(Complex* t_Data, const int t_Length)
{
	//此部分代码来源自网络开源共享内容，未经校验，不确定生成的FFT数据是否正确。
	auto const t_Weights = make_unique<Complex[]>(t_Length);
	auto const t_X = make_unique<Complex[]>(t_Length);
	auto const t_Bits = make_unique<int[]>(t_Length);
	auto const t_Factor = (-2.0 * M_PI) / static_cast<double>(t_Length);
	double t_Angle;
	auto t_Half = t_Length / 2;
	int t_IndexI;
	for (t_IndexI = 0; t_IndexI != t_Half; ++t_IndexI)
	{
		t_Angle = t_IndexI * t_Factor;
		t_Weights[t_IndexI].Real = cos(t_Angle);
		t_Weights[t_IndexI].Imag = sin(t_Angle);
	}
	for (t_IndexI = t_Half; t_IndexI != t_Length; ++t_IndexI)
	{
		t_Weights[t_IndexI].Real = -(t_Weights[t_IndexI - t_Length / 2].Real);
		t_Weights[t_IndexI].Imag = -(t_Weights[t_IndexI - t_Length / 2].Imag);
	}
	int t_R;
	auto t_Layers = 0;
	auto t_Temp = t_Length;
	if (t_Temp == 2)
	{
		t_R = 1;
	}
	else
	{
		do
		{
			t_Temp /= 2;
			++t_Layers;
			if (t_Temp == 2)
			{
				t_R = t_Layers + 1;
				break;
			}
			if (t_Temp < 1)
			{
				t_R = -1;
				break;
			}
		} while (true);
	}
	auto t_Index = 0;
	int t_M;
	for (t_IndexI = 0; t_IndexI != t_Length; ++t_IndexI)
	{
		t_Index = 0;
		for (t_M = t_R - 1; t_M >= 0; --t_M) t_Index += (1 && (t_IndexI & (1 << t_M))) << (t_R - t_M - 1);
		t_Bits[t_IndexI] = t_Index;
		t_X[t_IndexI].Real = t_Data[t_Bits[t_IndexI]].Real;
		t_X[t_IndexI].Imag = t_Data[t_Bits[t_IndexI]].Imag;
	}
	int t_Offset, t_Distance, t_W, t_B, t_N, t_Mid, t_IndexN, t_IndexJ;
	for (auto L = 1; L <= t_R; ++L)
	{
		t_Offset = 1 << (L - 1);
		t_W = 1 << (t_R - L);
		t_B = t_Length >> L;
		t_N = t_Length / t_B;
		for (t_IndexJ = 0; t_IndexJ != t_B; ++t_IndexJ)
		{
			t_Mid = t_IndexJ * t_N;
			t_Half = t_N / 2;
			for (t_IndexN = 0; t_IndexN != t_Half; ++t_IndexN)
			{
				t_Index = t_IndexN + t_Mid;
				t_Distance = t_Index + t_Offset;
				t_Data[t_Index].Real = t_X[t_Index].Real + (t_Weights[t_IndexN * t_W].Real * t_X[t_Distance].Real - t_Weights[t_IndexN * t_W].Imag * t_X[t_Distance].Imag);
				t_Data[t_Index].Imag = t_X[t_Index].Imag + t_Weights[t_IndexN * t_W].Imag * t_X[t_Distance].Real + t_Weights[t_IndexN * t_W].Real * t_X[t_Distance].Imag;
			}
			for (t_IndexN = t_Half; t_IndexN != t_N; ++t_IndexN)
			{
				t_Index = t_IndexN + t_Mid;
				t_Data[t_Index].Real = t_X[t_Index - t_Offset].Real + t_Weights[t_IndexN * t_W].Real * t_X[t_Index].Real - t_Weights[t_IndexN * t_W].Imag * t_X[t_Index].Imag;
				t_Data[t_Index].Imag = t_X[t_Index - t_Offset].Imag + t_Weights[t_IndexN * t_W].Imag * t_X[t_Index].Real + t_Weights[t_IndexN * t_W].Real * t_X[t_Index].Imag;
			}
		}
		memcpy(t_X.get(), t_Data, t_Length * sizeof(Complex));
	}
}