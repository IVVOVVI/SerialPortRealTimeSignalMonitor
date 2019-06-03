#pragma once

#include <complex>

namespace Algorithms
{

	///<summary>复数结构</summary>
	struct Complex
	{
		double Real;
		double Imag;
	};

	///<summary>快速傅立叶变换</summary>
	///<param name="t_Data">数据</param>
	///<param name="t_Length">数据长度</param>
	void FFT(Complex* const t_Data, const int t_Length);

}