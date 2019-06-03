#pragma once

#include "Common.h"

namespace DllOscilloscope
{

	///<summary>缩放参数</summary>
	struct ZoomProperties;

	///<summary>缓存</summary>
	class Cache final
	{
		
	public:

		///<summary>缓存</summary>
		~Cache() = default;

		///<summary>缓存</summary>
		///<param name="t_Count">要呈现的数据量</param>
		///<param name="t_BaselineType">基线类型</param>
		///<param name="t_Range">量程（例如：[-50,50]的量程为100）</param>
		///<param name="t_Offset">基线偏移值</param>
		///<param name="t_AutoTranslate">自动归零（如果是非数值则不使用此功能，否则将最接近横轴的数值归零后加上此值。）</param>
		///<param name="t_Zoom">缩放参数</param>
		Cache(const UINT t_Count, const Enum_BaselineType t_BaselineType, const float t_Range, const int t_Offset, const float t_AutoTranslate, const ZoomProperties* const t_Zoom);

		///<summary>基线类型</summary>
		Enum_BaselineType BaselineType() const noexcept
		{
			return m_BaselineType;
		}

		///<summary>基线类型</summary>
		///<param name="t_BaselineType">基线类型</param>
		void BaselineType(const Enum_BaselineType t_BaselineType) noexcept
		{
			m_BaselineType = t_BaselineType;
		}

		///<summary>基线偏移值</summary>
		float Offset() const noexcept
		{
			return m_Offset;
		}

		///<summary>基线偏移值</summary>
		///<param name="t_Offset">基线偏移值</param>
		void Offset(const float t_Offset) noexcept
		{
			m_Offset = t_Offset;
		}

		///<summary>自动归零</summary>
		float AutoTranslate() const noexcept
		{
			return m_AutoTranslate;
		}

		///<summary>自动归零</summary>
		///<param name="t_Offset">自动归零偏移值（如果是非数值则不使用此功能，否则将最接近横轴的数值归零后加上此值。）</param>
		void AutoTranslate(const float t_Offset) noexcept
		{
			m_AutoTranslate = t_Offset;
		}

		///<summary>安全清空</summary>
		void SafeClear() noexcept;

		///<summary>清空</summary>
		void Clear() noexcept;

		///<summary>安全存入数据</summary>
		///<param name="t_Value">值</param>
		void SafePush(const float t_Value) noexcept;

		///<summary>存入数据</summary>
		///<param name="t_Value">值</param>
		void Push(const float t_Value) noexcept;

		///<summary>安全替换全部数据</summary>
		///<param name="t_Value">值</param>
		///<param name="t_Length">值数量</param>
		void SafeReplace(const float* const t_Value, const UINT t_Length) noexcept;

		///<summary>替换全部数据</summary>
		///<param name="t_Value">值</param>
		///<param name="t_Length">值数量</param>
		void Replace(const float* const t_Value, const UINT t_Length) noexcept;

		///<summary>是否不为空</summary>
		///<param name="t_Style">样式</param>
		bool IsNotEmpty(const Enum_GraphStyle t_Style) const noexcept;

		///<summary>填充数据（柱状图）</summary>
		///<param name="t_Target">绘制目标</param>
		///<param name="t_Color">颜色</param>
		///<param name="t_Step">每一段线段的长度</param>
		///<param name="t_Area">绘图区</param>
		///<param name="t_Clip">裁切区</param>
		///<param name="t_RefreshMode">刷新模式</param>
		///<param name="t_PixelCompensation">像素补偿（在WinForm中当绘制区域不是窗口时会出现不明原因的像素缺失现象，此参数用于补偿这一）</param>
		std::pair<float, float> FillData_Bar(ID2D1RenderTarget* const t_Target, ID2D1Brush* const t_Color, const float t_Step, const D2D1_RECT_F &t_Area, const D2D1_RECT_F &t_Clip, const Enum_RefreshMode t_RefreshMode) const noexcept;

		///<summary>填充数据（折线图）</summary>
		///<param name="t_Sink">折线</param>
		///<param name="t_Step">每一段线段的长度</param>
		///<param name="t_Area">绘图区</param>
		///<param name="t_Clip">裁切区</param>
		///<param name="t_RefreshMode">刷新模式</param>
		///<param name="t_Result">返回值（true - 成功 | false - 出错）</param>
		std::pair<float, float> FillData_Line(ID2D1GeometrySink* const t_Sink, const float t_Step, const D2D1_RECT_F &t_Area, const D2D1_RECT_F &t_Clip, const Enum_RefreshMode t_RefreshMode, bool &t_Result) const noexcept;

	private:

		///<summary>临界区</summary>
		Concurrency::critical_section m_CriticalSection;

		///<summary>数据量</summary>
		const UINT m_Length;

		///<summary>末端索引值</summary>
		const UINT m_IndexEnd;

		///<summary>基线类型</summary>
		Enum_BaselineType m_BaselineType;

		///<summary>量程</summary>
		const float m_Range;

		///<summary>基线偏移值</summary>
		float m_Offset;

		///<summary>自动归零（如果是非数值则不使用此功能，否则将最接近横轴的数值归零后加上此值。）</summary>
		float m_AutoTranslate;
			   
		///<summary>缩放参数</summary>
		const ZoomProperties* const m_Zoom;

		///<summary>缓存集合</summary>
		std::unique_ptr<std::atomic<float>[]> const m_Cache;

		///<summary>当前存入位置</summary>
		std::atomic_uint m_Index;

		///<summary>已存入的数据量</summary>
		std::atomic_uint m_Count;

		///<summary>计算原点偏移值</summary>
		///<param name="t_Count">数量</param>
		float m_CalculateZeroOffset(const UINT t_Count) const noexcept;
		
#define CALCULATE_VALUE ((m_Cache[t_Index] + t_ZeroOffset) * t_Factor)

		///<summary>填充数据（滚动）</summary>
		///<param name="t_Step">步进值</param>
		///<param name="t_Height">绘图区高度</param>
		///<param name="t_ValueOffset">补偿值</param>
		///<param name="t_FunBegin">起点处理函数</param>
		///<param name="t_FunAdd">后续点处理函数</param>
		template <typename FunA, typename FunB>
		void m_FillData_Scrolling(const float t_Step, const float t_Height, const float t_ValueOffset, FunA const &t_FunBegin, FunB const &t_FunAdd) const noexcept
		{
			UINT t_Count = m_Count;
			auto t_Index = 0u;
			auto const t_Factor = t_Height / m_Range;
			auto const t_IsFull = t_Count > m_Length;
			auto const t_ZeroOffset = m_CalculateZeroOffset(t_IsFull ? m_Length : t_Count) + t_ValueOffset + m_Offset;
			if (t_IsFull)
			{
				t_Index = m_Index;
				++t_Index;
				t_Count = t_Index;
				if (t_Index != m_Length)
				{
					auto t_IsBeginAdded = false;
					for (; t_Index != m_Length; ++t_Index)
					{
						if (t_FunBegin(CALCULATE_VALUE))
						{
							t_IsBeginAdded = true;
							++t_Index;
							break;
						}
					}
					if (t_IsBeginAdded)
					{
						for (; t_Index != m_Length; ++t_Index)
						{
							if (t_FunAdd(CALCULATE_VALUE))
							{
								++t_Index;
								break;
							}
						}
					}
					else
					{
						t_Index = 0u;
					}
				}
				else
				{
					t_Index = 0u;
				}
			}
			if (t_Index == 0u)
			{
				for (; t_Index != m_Length; ++t_Index)
				{
					if (t_FunBegin(CALCULATE_VALUE))
					{
						++t_Index;
						break;
					}
				}
			}
			else
			{
				t_Index = 0u;
			}
			for (; t_Index != t_Count; ++t_Index) if (t_FunAdd(CALCULATE_VALUE)) break;
		}

		///<summary>填充数据（扫描）</summary>
		///<param name="t_Step">步进值</param>
		///<param name="t_Height">绘图区高度</param>
		///<param name="t_ValueOffset">补偿值</param>
		///<param name="t_FunBegin">起点处理函数</param>
		///<param name="t_FunAdd">后续点处理函数</param>
		template <typename FunA, typename FunB>
		UINT m_FillData_Scanning(const float t_Step, const float t_Height, const float t_ValueOffset, FunA const &t_FunBegin, FunB const &t_FunAdd) const noexcept
		{
			const UINT t_DataIndex = m_Index;
			UINT t_Count = m_Count;
			if (t_Count > m_Length) t_Count = m_Length;
			auto const t_ZeroOffset = m_CalculateZeroOffset(t_Count) + t_ValueOffset + m_Offset;
			auto t_Index = 0u;
			auto const t_Factor = t_Height / m_Range;
			for (; t_Index != t_Count; ++t_Index)
			{
				if (t_FunBegin(CALCULATE_VALUE))
				{
					++t_Index;
					break;
				}
			}
			for (; t_Index != t_Count; ++t_Index)
			{
				if (t_FunAdd(CALCULATE_VALUE)) break;
			}
			return t_DataIndex;
		}

	};

}