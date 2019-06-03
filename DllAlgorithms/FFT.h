#pragma once

#include <complex>

namespace Algorithms
{

	///<summary>�����ṹ</summary>
	struct Complex
	{
		double Real;
		double Imag;
	};

	///<summary>���ٸ���Ҷ�任</summary>
	///<param name="t_Data">����</param>
	///<param name="t_Length">���ݳ���</param>
	void FFT(Complex* const t_Data, const int t_Length);

}