#pragma once

#include "Common.h"

namespace DllOscilloscope
{

	///<summary>���Ų���</summary>
	struct ZoomProperties;

	///<summary>����</summary>
	class Cache final
	{
		
	public:

		///<summary>����</summary>
		~Cache() = default;

		///<summary>����</summary>
		///<param name="t_Count">Ҫ���ֵ�������</param>
		///<param name="t_BaselineType">��������</param>
		///<param name="t_Range">���̣����磺[-50,50]������Ϊ100��</param>
		///<param name="t_Offset">����ƫ��ֵ</param>
		///<param name="t_AutoTranslate">�Զ����㣨����Ƿ���ֵ��ʹ�ô˹��ܣ�������ӽ��������ֵ�������ϴ�ֵ����</param>
		///<param name="t_Zoom">���Ų���</param>
		Cache(const UINT t_Count, const Enum_BaselineType t_BaselineType, const float t_Range, const int t_Offset, const float t_AutoTranslate, const ZoomProperties* const t_Zoom);

		///<summary>��������</summary>
		Enum_BaselineType BaselineType() const noexcept
		{
			return m_BaselineType;
		}

		///<summary>��������</summary>
		///<param name="t_BaselineType">��������</param>
		void BaselineType(const Enum_BaselineType t_BaselineType) noexcept
		{
			m_BaselineType = t_BaselineType;
		}

		///<summary>����ƫ��ֵ</summary>
		float Offset() const noexcept
		{
			return m_Offset;
		}

		///<summary>����ƫ��ֵ</summary>
		///<param name="t_Offset">����ƫ��ֵ</param>
		void Offset(const float t_Offset) noexcept
		{
			m_Offset = t_Offset;
		}

		///<summary>�Զ�����</summary>
		float AutoTranslate() const noexcept
		{
			return m_AutoTranslate;
		}

		///<summary>�Զ�����</summary>
		///<param name="t_Offset">�Զ�����ƫ��ֵ������Ƿ���ֵ��ʹ�ô˹��ܣ�������ӽ��������ֵ�������ϴ�ֵ����</param>
		void AutoTranslate(const float t_Offset) noexcept
		{
			m_AutoTranslate = t_Offset;
		}

		///<summary>��ȫ���</summary>
		void SafeClear() noexcept;

		///<summary>���</summary>
		void Clear() noexcept;

		///<summary>��ȫ��������</summary>
		///<param name="t_Value">ֵ</param>
		void SafePush(const float t_Value) noexcept;

		///<summary>��������</summary>
		///<param name="t_Value">ֵ</param>
		void Push(const float t_Value) noexcept;

		///<summary>��ȫ�滻ȫ������</summary>
		///<param name="t_Value">ֵ</param>
		///<param name="t_Length">ֵ����</param>
		void SafeReplace(const float* const t_Value, const UINT t_Length) noexcept;

		///<summary>�滻ȫ������</summary>
		///<param name="t_Value">ֵ</param>
		///<param name="t_Length">ֵ����</param>
		void Replace(const float* const t_Value, const UINT t_Length) noexcept;

		///<summary>�Ƿ�Ϊ��</summary>
		///<param name="t_Style">��ʽ</param>
		bool IsNotEmpty(const Enum_GraphStyle t_Style) const noexcept;

		///<summary>������ݣ���״ͼ��</summary>
		///<param name="t_Target">����Ŀ��</param>
		///<param name="t_Color">��ɫ</param>
		///<param name="t_Step">ÿһ���߶εĳ���</param>
		///<param name="t_Area">��ͼ��</param>
		///<param name="t_Clip">������</param>
		///<param name="t_RefreshMode">ˢ��ģʽ</param>
		///<param name="t_PixelCompensation">���ز�������WinForm�е����������Ǵ���ʱ����ֲ���ԭ�������ȱʧ���󣬴˲������ڲ�����һ��</param>
		std::pair<float, float> FillData_Bar(ID2D1RenderTarget* const t_Target, ID2D1Brush* const t_Color, const float t_Step, const D2D1_RECT_F &t_Area, const D2D1_RECT_F &t_Clip, const Enum_RefreshMode t_RefreshMode) const noexcept;

		///<summary>������ݣ�����ͼ��</summary>
		///<param name="t_Sink">����</param>
		///<param name="t_Step">ÿһ���߶εĳ���</param>
		///<param name="t_Area">��ͼ��</param>
		///<param name="t_Clip">������</param>
		///<param name="t_RefreshMode">ˢ��ģʽ</param>
		///<param name="t_Result">����ֵ��true - �ɹ� | false - ����</param>
		std::pair<float, float> FillData_Line(ID2D1GeometrySink* const t_Sink, const float t_Step, const D2D1_RECT_F &t_Area, const D2D1_RECT_F &t_Clip, const Enum_RefreshMode t_RefreshMode, bool &t_Result) const noexcept;

	private:

		///<summary>�ٽ���</summary>
		Concurrency::critical_section m_CriticalSection;

		///<summary>������</summary>
		const UINT m_Length;

		///<summary>ĩ������ֵ</summary>
		const UINT m_IndexEnd;

		///<summary>��������</summary>
		Enum_BaselineType m_BaselineType;

		///<summary>����</summary>
		const float m_Range;

		///<summary>����ƫ��ֵ</summary>
		float m_Offset;

		///<summary>�Զ����㣨����Ƿ���ֵ��ʹ�ô˹��ܣ�������ӽ��������ֵ�������ϴ�ֵ����</summary>
		float m_AutoTranslate;
			   
		///<summary>���Ų���</summary>
		const ZoomProperties* const m_Zoom;

		///<summary>���漯��</summary>
		std::unique_ptr<std::atomic<float>[]> const m_Cache;

		///<summary>��ǰ����λ��</summary>
		std::atomic_uint m_Index;

		///<summary>�Ѵ����������</summary>
		std::atomic_uint m_Count;

		///<summary>����ԭ��ƫ��ֵ</summary>
		///<param name="t_Count">����</param>
		float m_CalculateZeroOffset(const UINT t_Count) const noexcept;
		
#define CALCULATE_VALUE ((m_Cache[t_Index] + t_ZeroOffset) * t_Factor)

		///<summary>������ݣ�������</summary>
		///<param name="t_Step">����ֵ</param>
		///<param name="t_Height">��ͼ���߶�</param>
		///<param name="t_ValueOffset">����ֵ</param>
		///<param name="t_FunBegin">��㴦����</param>
		///<param name="t_FunAdd">�����㴦����</param>
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

		///<summary>������ݣ�ɨ�裩</summary>
		///<param name="t_Step">����ֵ</param>
		///<param name="t_Height">��ͼ���߶�</param>
		///<param name="t_ValueOffset">����ֵ</param>
		///<param name="t_FunBegin">��㴦����</param>
		///<param name="t_FunAdd">�����㴦����</param>
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