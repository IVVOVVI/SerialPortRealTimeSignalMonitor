using System;
using System.Threading.Tasks;
using Algorithms;
using DllInterface;

namespace Statistics
{

	///<summary>默认统计模组</summary>
	public class DefaultStatisticsModule : IDataReceiver
	{

		///<summary>数据大小</summary>
		private readonly uint m_Size;

		///<summary>数据缓存</summary>
		private float[] m_Cache;

		///<summary>当前数据量</summary>
		private uint m_Length = 0u;

		///<summary>是否已关闭</summary>
		private bool m_IsClosed = false;

		///<summary>最大值</summary>
		private readonly IDataReceiver m_Max;

		///<summary>最小值</summary>
		private readonly IDataReceiver m_Min;

		///<summary>平均值</summary>
		private readonly IDataReceiver m_Average;

		///<summary>标准差</summary>
		private readonly IDataReceiver m_StandardDeviation;

		///<summary>是否使用标准差系数代替标准差</summary>
		private readonly bool m_IsCV;

		///<summary>快速傅立叶变换</summary>
		private readonly IDataReceiver m_FastFourierTransform;

		///<summary>默认统计模组</summary>
		///<param name="max">最大值</param>
		///<param name="min">最小值</param>
		///<param name="average">平均值</param>
		///<param name="sd">标准差</param>
		///<param name="isCV">是否使用标准差系数代替标准差</param>
		///<param name="fft">快速傅立叶变换</param>
		///<param name="value">数据大小（使用FFT时必须大于等于8且为2的次方）</param>
		public DefaultStatisticsModule(IDataReceiver max, IDataReceiver min, IDataReceiver average, IDataReceiver sd = null, bool isCV = false, IDataReceiver fft = null, uint size = 8192u)
		{
			if (fft != null)
			{
				if (size < 8u || (size & (size - 1u)) != 0u) throw new ArgumentException();
			}
			else
			{
				if (size == 0u) throw new ArgumentException();
			}
			m_Max = max;
			m_Min = min;
			m_Average = average;
			m_StandardDeviation = sd;
			m_IsCV = isCV;
			m_FastFourierTransform = fft;
			m_Size = size;
			m_Cache = new float[size];
		}

		///<summary>安全存入数据</summary>
		///<param name="value">值</param>
		public void SafePush(float value)
		{
			Push(value);
		}

		///<summary>存入数据</summary>
		///<param name="value">值</param>
		public void Push(float value)
		{
			float[] temp;
			lock (this)
			{
				m_Cache[m_Length] = value;
				++m_Length;
				if (m_Length < m_Size) return;
				temp = m_Cache;
				CreateNewCache();
			}
			Count(temp);
		}

		///<summary>安全清空</summary>
		public void SafeClear()
		{
			Clear();
		}

		///<summary>清空</summary>
		public void Clear()
		{
			lock (this)
			{
				CreateNewCache();
				if (m_Max != null) m_Max.Clear();
				if (m_Min != null) m_Min.Clear();
				if (m_Average != null) m_Average.Clear();
				if (m_FastFourierTransform != null) m_FastFourierTransform.Clear();
				if (m_StandardDeviation != null) m_StandardDeviation.Clear();
			}
		}

		///<summary>创建新缓存</summary>
		private void CreateNewCache()
		{
			m_Cache = new float[m_Size];
			m_Length = 0u;
		}

		///<summary>统计</summary>
		///<param name="data">要统计的数据</param>
		private void Count(float[] data)
		{
			Task.Run(() =>
			{
				float max, min, average;
				if (m_IsClosed) return;
				Dll.MaxMinAverage(data, out max, out min, out average);
				if (m_IsClosed) return;
				if (m_Max != null) m_Max.SafePush(max);
				if (m_IsClosed) return;
				if (m_Min != null) m_Min.SafePush(min);
				if (m_IsClosed) return;
				if (m_Average != null) m_Average.SafePush(average);
				if (m_IsClosed) return;
				if (m_StandardDeviation != null && average != 0.0f)
				{
					float sd;
					Dll.StandardDeviation(data, average, out sd);
					if (m_IsCV)
					{
						sd *= 100.0f;
						sd /= average;
					}
					if (m_IsClosed) return;
					m_StandardDeviation.Push(sd);
				}
				if (m_IsClosed) return;
				if (m_FastFourierTransform != null)
				{
					Dll.FastFourierTransform(data);
					if (m_IsClosed) return;
					m_FastFourierTransform.SafeReplace(data);
				}
			});
		}

		///<summary>替换全部数据</summary>
		///<param name="value">值</param>
		void IReplace<float>.Replace(float[] value) { }

		///<summary>安全替换全部数据（并行安全）</summary>
		///<param name="value">值</param>
		void IReplace<float>.SafeReplace(float[] value) { }

		///<summary>关闭</summary>
		public void Close()
		{
			m_IsClosed = true;
		}

	}

}