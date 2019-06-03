using System;
using System.Runtime.InteropServices;

namespace Algorithms
{

	class Dll86
	{

		private const string DllFileName = @"x86/DllAlgorithms.dll";

		///<summary>计算最大值最小值平均值</summary>
		///<param name="data">数据</param>
		///<param name="length">数据长度</param>
		///<param name="max">最大值</param>
		///<param name="min">最小值</param>
		///<param name="average">平均值</param>
		[DllImport(DllFileName)]
		internal extern static void MaxMinAverage(IntPtr data, uint length, IntPtr max, IntPtr min, IntPtr average);

		///<summary>计算标准差</summary>
		///<param name="data">数据</param>
		///<param name="length">数据长度</param>
		///<param name="average">平均值</param>
		///<param name="sd">标准差</param>
		[DllImport(DllFileName)]
		internal extern static void StandardDeviation(IntPtr data, uint length, float average, IntPtr sd);

		///<summary>快速傅立叶变换</summary>
		///<param name="data">数据</param>
		///<param name="length">数据长度</param>
		[DllImport(DllFileName)]
		internal extern static void FastFourierTransform(IntPtr data, uint length);

	}

}