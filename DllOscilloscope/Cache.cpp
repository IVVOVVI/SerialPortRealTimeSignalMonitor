#include "stdafx.h"
#include "Cache.h"
#include "ZoomProperties.h"

using namespace std;
using namespace D2D1;
using namespace Concurrency;
using namespace DllOscilloscope;

///<summary>缓存</summary>
Cache::Cache(const UINT t_Count, const Enum_BaselineType t_BaselineType, const float t_Range, const int t_Offset, const float t_AutoTranslate, const ZoomProperties* const t_Zoom) :
	m_Length(t_Count),
	m_IndexEnd(t_Count - 1u),
	m_BaselineType(t_BaselineType),
	m_Range(t_Range),
	m_Offset(static_cast<float>(t_Offset)),
	m_AutoTranslate(t_AutoTranslate),
	m_Zoom(t_Zoom),
	m_Cache(make_unique<atomic<float>[]>(m_Length)),
	m_Index(0u),
	m_Count(0u)
{}

///<summary>安全清空</summary>
void Cache::SafeClear() noexcept
{
	critical_section::scoped_lock lock(m_CriticalSection);
	Clear();
}

///<summary>清空</summary>
void Cache::Clear() noexcept
{
	m_Count = 0u;
	m_Index = 0u;
}

///<summary>安全存入数据</summary>
void Cache::SafePush(const float t_Value) noexcept
{
	critical_section::scoped_lock lock(m_CriticalSection);
	Push(t_Value);
}

///<summary>存入数据</summary>
void Cache::Push(const float t_Value) noexcept
{
	m_Cache[m_Index] = t_Value;
	m_Index == m_IndexEnd ? m_Index = 0u : ++m_Index;
	if (m_Count <= m_Length) ++m_Count;
}

///<summary>安全替换全部数据</summary>
void Cache::SafeReplace(const float* const t_Value, const UINT t_Length) noexcept
{
	critical_section::scoped_lock lock(m_CriticalSection);
	Replace(t_Value, t_Length);
}

///<summary>替换全部数据</summary>
void Cache::Replace(const float* const t_Value, const UINT t_Length) noexcept
{
	Clear();
	auto const t_Size = min(m_Length, t_Length);
	memcpy(m_Cache.get(), t_Value, t_Size * sizeof(float));
	m_Count = m_Length + 1u;
	m_Index = m_IndexEnd;
}

///<summary>是否不为空</summary>
bool Cache::IsNotEmpty(const Enum_GraphStyle t_Style) const noexcept
{
	switch (t_Style)
	{
	case Enum_GraphStyle::Bar:
		return m_Count != 0u;
	case Enum_GraphStyle::Line:
		return m_Count > 1u;
	default:
		return false;
	}
}

///<summary>填充数据（柱状图）</summary>
pair<float, float> Cache::FillData_Bar(ID2D1RenderTarget* const t_Target, ID2D1Brush* const t_Color, const float t_Step, const D2D1_RECT_F &t_Area, const D2D1_RECT_F &t_Clip, const Enum_RefreshMode t_RefreshMode) const noexcept
{
	auto const t_Height = t_Area.bottom - t_Area.top;
	auto t_Rectangle = t_Area;
	float t_ValueOffset;
	switch (m_BaselineType)
	{
	case Enum_BaselineType::Bottom:
		t_ValueOffset = 0.0f;
		break;
	case Enum_BaselineType::Center:
		t_ValueOffset = m_Range / 2.0f;
		t_Rectangle.bottom = floor(fma(t_Height, 0.5f, t_Rectangle.top));
		break;
	default:
		t_ValueOffset = m_Range;
		t_Rectangle.bottom = t_Rectangle.top;
		break;
	}
	struct
	{
		ID2D1RenderTarget* const Target;
		D2D1_RECT_F &Rectangle;
		ID2D1Brush* const Color;
		const float Step;
		float X;
		float Temp;
		float Bottom;
	} t_Temp{ t_Target, t_Rectangle, t_Color, t_Step, t_Area.left, floor(t_Area.left), t_Area.bottom };
	auto const t_Fun_Begin = [this, &t_Temp, t_Min = t_Clip.left](const float t_Y)
	{
		t_Temp.X += t_Temp.Step;
		t_Temp.Temp = floor(t_Temp.X);
		if (t_Temp.Temp == t_Temp.Rectangle.left) ++t_Temp.Temp;
		if (t_Temp.Temp > t_Min)
		{
			t_Temp.Rectangle.left = t_Min;
			t_Temp.Rectangle.right = t_Temp.Temp;
			t_Temp.Rectangle.top = floor(t_Temp.Bottom - t_Y);
			t_Temp.Target->FillRectangle(t_Temp.Rectangle, t_Temp.Color);
			return true;
		}
		return false;
	};
	auto const t_Fun_Add = [this, &t_Temp, t_Max = t_Clip.right](const float t_Y)
	{
		if (t_Temp.Temp < t_Max)
		{
			t_Temp.Rectangle.left = t_Temp.Temp;
			t_Temp.X += t_Temp.Step;
			t_Temp.Temp = floor(t_Temp.X);
			if (t_Temp.Temp == t_Temp.Rectangle.left) ++t_Temp.Temp;
			t_Temp.Rectangle.right = t_Temp.Temp;
			t_Temp.Rectangle.top = floor(t_Temp.Bottom - t_Y);
			t_Temp.Target->FillRectangle(t_Temp.Rectangle, t_Temp.Color);
			return false;
		}
		return true;
	};
	switch (t_RefreshMode)
	{
	case Enum_RefreshMode::Scrolling:
		m_FillData_Scrolling(t_Step, t_Height, t_ValueOffset, t_Fun_Begin, t_Fun_Add);
		break;
	case Enum_RefreshMode::Scanning:
		t_Temp.X = (m_FillData_Scanning(t_Step, t_Height, t_ValueOffset, t_Fun_Begin, t_Fun_Add) * t_Step) + t_Area.left;
	default:
		break;
	}
	return make_pair(floor(t_Temp.X), ceil(t_Temp.X + t_Step));
}

///<summary>填充数据（折线图）</summary>
pair<float, float> Cache::FillData_Line(ID2D1GeometrySink* const t_Sink, const float t_Step, const D2D1_RECT_F &t_Area, const D2D1_RECT_F &t_Clip, const Enum_RefreshMode t_RefreshMode, bool &t_Result) const noexcept
{
	auto const t_Height = t_Area.bottom - t_Area.top;
	float t_ValueOffset;
	switch (m_BaselineType)
	{
	case Enum_BaselineType::Bottom:
		t_ValueOffset = 0.0f;
		break;
	case Enum_BaselineType::Center:
		t_ValueOffset = m_Range / 2.0f;
		break;
	default:
		t_ValueOffset = m_Range;
		break;
	}
	auto const t_HalfStep = t_Step / 2.0f;
	struct
	{
		ID2D1GeometrySink* const Sink;
		const float Bottom;
		const float Step;
		float X;
	} t_Temp{ t_Sink, t_Area.bottom, t_Step, t_Area.left + t_HalfStep };
	auto const t_Fun_Begin = [this, &t_Temp, t_Min = t_Clip.left](const float t_Y)
	{
		t_Temp.X += t_Temp.Step;
		if (t_Temp.X > t_Min)
		{
			t_Temp.Sink->BeginFigure(Point2F(t_Temp.X - t_Temp.Step, t_Temp.Bottom - t_Y), D2D1_FIGURE_BEGIN::D2D1_FIGURE_BEGIN_HOLLOW);
			return true;
		}
		return false;
	};
	auto const t_Fun_Add = [this, &t_Temp, t_Max = t_Clip.right](const float t_Y)
	{
		t_Temp.Sink->AddLine(Point2F(t_Temp.X, t_Temp.Bottom - t_Y));
		if (t_Temp.X < t_Max)
		{
			t_Temp.X += t_Temp.Step;
			return false;
		}
		return true;
	};
	switch (t_RefreshMode)
	{
	case Enum_RefreshMode::Scrolling:
		m_FillData_Scrolling(t_Step, t_Height, t_ValueOffset, t_Fun_Begin, t_Fun_Add);
		break;
	case Enum_RefreshMode::Scanning:
		t_Temp.X = (m_FillData_Scanning(t_Step, t_Height, t_ValueOffset, t_Fun_Begin, t_Fun_Add) * t_Step) + t_Area.left;
		break;
	default:
		break;
	}
	t_Sink->EndFigure(D2D1_FIGURE_END::D2D1_FIGURE_END_OPEN);
	if (SUCCEEDED(t_Sink->Close()))
	{
		t_Result = true;
		return make_pair(floor(t_Temp.X - t_HalfStep), ceil(t_Temp.X + t_Step + t_HalfStep));
	}
	else
	{
		t_Result = false;
		return make_pair(0.0f, 0.0f);
	}
}

///<summary>计算原点偏移值</summary>
float Cache::m_CalculateZeroOffset(const UINT t_Count) const noexcept
{
	if (!(isfinite(m_AutoTranslate))) return 0.0f;
	auto t_Index = 0u;
	switch (m_BaselineType)
	{
	case Enum_BaselineType::Bottom:
	{
		float t_Temp = m_Cache[t_Index];
		for (++t_Index; t_Index != t_Count; ++t_Index)
		{
			if (m_Cache[t_Index] < t_Temp) t_Temp = m_Cache[t_Index];
		}
		return (-t_Temp) + abs(m_AutoTranslate) + 1.0f;
	}
	case Enum_BaselineType::Top:
	{
		float t_Temp = m_Cache[t_Index];
		for (++t_Index; t_Index != t_Count; ++t_Index)
		{
			if (m_Cache[t_Index] > t_Temp) t_Temp = m_Cache[t_Index];
		}
		return (-t_Temp) - abs(m_AutoTranslate) - 1.0f;
	}
	default:
	{
		auto t_Sum = 0.0;
		for (; t_Index != t_Count; ++t_Index)
		{
			t_Sum += m_Cache[t_Index];
		}
		return (-(static_cast<float>(t_Sum / static_cast<double>(t_Count)))) + m_AutoTranslate;
	}
	}
}