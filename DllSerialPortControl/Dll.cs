using System;
using System.Runtime.InteropServices;

namespace Algorithms
{

	unsafe public static class Dll
	{

		private static readonly bool s_IsX64 = Environment.Is64BitProcess;

		///<summary>计算最大值最小值平均值</summary>
		///<param name="data">数据</param>
		///<param name="max">最大值</param>
		///<param name="min">最小值</param>
		///<param name="average">平均值</param>
		public static void MaxMinAverage(float[] data, out float max, out float min, out float average)
		{
			float temp_max = 0.0f, temp_min = 0.0f, temp_average = 0.0f;
			fixed (float* array = data)
			{
				if (s_IsX64)
				{
					Dll64.MaxMinAverage(new IntPtr(array), (uint)data.Length, new IntPtr(&temp_max), new IntPtr(&temp_min), new IntPtr(&temp_average));
				}
				else
				{
					Dll86.MaxMinAverage(new IntPtr(array), (uint)data.Length, new IntPtr(&temp_max), new IntPtr(&temp_min), new IntPtr(&temp_average));
				}
			}
			max = temp_max;
			min = temp_min;
			average = temp_average;
		}

		///<summary>计算标准差</summary>
		///<param name="data">数据</param>
		///<param name="average">平均值</param>
		///<param name="sd">标准差</param>
		public static void StandardDeviation(float[] data, float average, out float sd)
		{
			float temp = 0.0f;
			fixed (float* array = data)
			{
				if (s_IsX64)
				{
					Dll64.StandardDeviation(new IntPtr(array), (uint)data.Length, average, new IntPtr(&temp));
				}
				else
				{
					Dll86.StandardDeviation(new IntPtr(array), (uint)data.Length, average, new IntPtr(&temp));
				}
			}
			sd = temp;
		}

		///<summary>快速傅立叶变换</summary>
		///<param name="data">数据</param>
		public static void FastFourierTransform(float[] data)
		{
			fixed (float* array = data)
			{
				if (s_IsX64)
				{
					Dll64.FastFourierTransform(new IntPtr(array), (uint)data.Length);
				}
				else
				{
					Dll86.FastFourierTransform(new IntPtr(array), (uint)data.Length);
				}
			}
		}

	}

}