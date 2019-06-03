#include "stdafx.h"
#include "ColorHelper.h"
#include "Oscilloscope.h"
#include "Cache.h"
#include "ZoomProperties.h"
#include "AxisProperties.h"
#include "GraphProperties.h"

using namespace std;
using namespace D2D1;
using namespace Concurrency;
using namespace Microsoft::WRL;
using namespace DllOscilloscope;

///<summary>示波器项结构</summary>
Oscilloscope::OscilloscopeItemStruct::~OscilloscopeItemStruct()
{
	delete DataCache;
}

///<summary>示波器项结构</summary>
Oscilloscope::OscilloscopeItemStruct::OscilloscopeItemStruct(const UINT32 t_Count, const float t_Range, const GraphProperties* const t_GraphProperties, const ZoomProperties* const t_Zoom) :
	Style(t_GraphProperties->GraphStyle),
	ForegroundValue(t_GraphProperties->Foreground),
	Foreground(ColorF(t_GraphProperties->Foreground, GetAlphaChannel(t_GraphProperties->Foreground))),
	DataCache(new Cache(t_Count, t_GraphProperties->BaselineType, t_Range, t_GraphProperties->Offset, t_GraphProperties->AutoTranslate, t_Zoom)),
	GraphColor(nullptr),
	LineStyle(nullptr),
	Geometry(nullptr),
	LineWeight(static_cast<float>(t_GraphProperties->LineWidth)),
	IsEnabled(t_GraphProperties->IsEnabled)
{}

///<summary>平移参数</summary>
Oscilloscope::TranslateArgs::TranslateArgs(ZoomParameters* const t_Args, const int t_Offset) :
	Args(t_Args),
	Translate(static_cast<int>(t_Args->Translate) - t_Offset)
{}

///<summary>缩放参数</summary>
Oscilloscope::ScaleArgs::ScaleArgs(ZoomParameters* const t_Args, const float t_Offset) :
	Args(t_Args),
	Scale(t_Args->Scale + t_Offset)
{}

///<summary>示波器</summary>
Oscilloscope::~Oscilloscope()
{
	delete m_Zoom;
	delete m_Axis;
}

///<summary>示波器</summary>
Oscilloscope::Oscilloscope(ComPtr<ID2D1Factory> const &t_Factory, HWND const t_ControlHwnd, const ConstructionArgs* const t_Args, const GraphProperties* const t_GraphProperties, const UINT t_Size) :
	m_Handle(t_ControlHwnd),
	m_LayerParameters(LayerParameters()),
	m_BackgroundValue(t_Args->Background),
	m_Background(ColorF(t_Args->Background, GetAlphaChannel(t_Args->Background))),
	m_RefreshMode(t_Args->RefreshMode),
	m_Zoom(new ZoomProperties()),
	m_Axis(new AxisProperties(t_Args)),
	m_D2dFactory(t_Factory),
	m_RenderTarget(nullptr),
	m_DatalLength(t_Args->Count),
	m_Layer(nullptr),
	m_Items(make_unique<unique_ptr<OscilloscopeItemStruct>[]>(t_Size)),
	m_ItemsSize(t_Size),
	m_Width(0.0f),
	m_Height(0.0f),
	m_IsNeedRefresh(false)
{
	if (t_Factory == nullptr) throw exception("No D2dFactory!", 1);
	auto const t_ValueRange = abs(t_Args->Range);
	if (static_cast<UINT>(t_ValueRange) == 0u || m_DatalLength < 2) throw exception("Parameter Error!", 0);
	m_Zoom->X.Scale_Limit = t_Args->ScaleLimitX;
	m_Zoom->Y.Scale_Limit = t_Args->ScaleLimitY;
	m_Zoom->X.Scale = t_Args->ScaleX;
	m_Zoom->Y.Scale = t_Args->ScaleY;
	m_Zoom->X.Translate = t_Args->TranslateX;
	m_Zoom->Y.Translate = t_Args->TranslateY;
	auto t_Item = m_Items.get();
	for (auto t_Index = 0u; t_Index != t_Size; ++t_Index, ++t_Item) *t_Item = make_unique<OscilloscopeItemStruct>(m_DatalLength, t_ValueRange, t_GraphProperties + t_Index, m_Zoom);
	m_Recreate();
	m_Initialize(m_Zoom->X, m_Width - m_Axis->AxisY_Space);
	m_Initialize(m_Zoom->Y, m_Height - m_Axis->AxisX_Space);
}

///<summary>水平缩放率【单线程调用】</summary>
float Oscilloscope::ScaleX() const noexcept
{
	return m_Zoom->X.Scale;
}

///<summary>水平缩放率极限值【单线程调用】</summary>
float Oscilloscope::ScaleX_Limit() const noexcept
{
	return m_Zoom->X.Scale_Limit;
}

///<summary>水平位移【单线程调用】</summary>
int Oscilloscope::TranslateX() const noexcept
{
	return m_Zoom->X.Translate;
}

///<summary>水平位移极限值【单线程调用】</summary>
int Oscilloscope::TranslateX_Limit() const noexcept
{
	return m_Zoom->X.Translate_Limit;
}

///<summary>垂直缩放率【单线程调用】</summary>
float Oscilloscope::ScaleY() const noexcept
{
	return m_Zoom->Y.Scale;
}

///<summary>垂直缩放率极限值【单线程调用】</summary>
float Oscilloscope::ScaleY_Limit() const noexcept
{
	return m_Zoom->Y.Scale_Limit;
}

///<summary>垂直位移【单线程调用】</summary>
int Oscilloscope::TranslateY() const noexcept
{
	return m_Zoom->Y.Translate;
}

///<summary>垂直位移极限值【单线程调用】</summary>
int Oscilloscope::TranslateY_Limit() const noexcept
{
	return m_Zoom->Y.Translate_Limit;
}

///<summary>改变缩放极限值【单线程调用】</summary>
bool Oscilloscope::ChangeScaleLimit(const UINT t_Value)
{
	if (t_Value >= 1u)
	{
		m_Zoom->X.Scale_Limit = static_cast<float>(t_Value);
		m_Zoom->Y.Scale_Limit = static_cast<float>(t_Value);
		ScaleArgs t_ArgsX(&m_Zoom->X);
		ScaleArgs t_ArgsY(&m_Zoom->Y);
		auto const t_Limit = static_cast<float>(t_Value);
		auto const t_ResultX = m_ChangeScaleLimit(t_ArgsX, m_Width - m_Axis->AxisY_Space, t_Limit);
		auto const t_ResultY = m_ChangeScaleLimit(t_ArgsY, m_Height - m_Axis->AxisX_Space, t_Limit);
		if (t_ResultX || t_ResultY)
		{
			critical_section::scoped_lock lock(m_CriticalSection);
			if (t_ResultX)
			{
				m_Zoom->X.Scale = t_ArgsX.Scale;
				m_Zoom->X.Translate = static_cast<UINT>(t_ArgsX.Translate);
			}
			if (t_ResultY)
			{
				m_Zoom->Y.Scale = t_ArgsY.Scale;
				m_Zoom->Y.Translate = static_cast<UINT>(t_ArgsY.Translate);
			}
			m_Render();
			return true;
		}
	}
	return false;
}

///<summary>改变水平缩放极限值【单线程调用】</summary>
bool Oscilloscope::ChangeScaleLimitX(const UINT t_Value)
{
	if (t_Value < 1u) return false;
	m_Zoom->X.Scale_Limit = static_cast<float>(t_Value);
	ScaleArgs t_ArgsX(&m_Zoom->X);
	auto const t_ResultX = m_ChangeScaleLimit(t_ArgsX, m_Width - m_Axis->AxisY_Space, static_cast<float>(t_Value));
	if (t_ResultX)
	{
		critical_section::scoped_lock lock(m_CriticalSection);
		m_Zoom->X.Scale = t_ArgsX.Scale;
		m_Zoom->X.Translate = static_cast<UINT>(t_ArgsX.Translate);
		m_Render();
	}
	return t_ResultX;
}

///<summary>改变垂直缩放极限值【单线程调用】</summary>
bool Oscilloscope::ChangeScaleLimitY(const UINT t_Value)
{
	if (t_Value < 1u) return false;
	m_Zoom->Y.Scale_Limit = static_cast<float>(t_Value);
	ScaleArgs t_ArgsY(&m_Zoom->Y);
	auto const t_ResultY = m_ChangeScaleLimit(t_ArgsY, m_Height - m_Axis->AxisX_Space, static_cast<float>(t_Value));
	if (t_ResultY)
	{
		critical_section::scoped_lock lock(m_CriticalSection);
		m_Zoom->Y.Scale = t_ArgsY.Scale;
		m_Zoom->Y.Translate = static_cast<UINT>(t_ArgsY.Translate);
		m_Render();
	}
	return t_ResultY;
}

///<summary>缩放【单线程调用】</summary>
bool Oscilloscope::Scale(const float t_Offset)
{
	ScaleArgs t_ArgsX(&m_Zoom->X, t_Offset);
	ScaleArgs t_ArgsY(&m_Zoom->Y, t_Offset);
	auto const t_ResultX = m_Scale(t_ArgsX, m_Width - m_Axis->AxisY_Space);
	auto const t_ResultY = m_Scale(t_ArgsY, m_Height - m_Axis->AxisX_Space);
	if (t_ResultX || t_ResultY)
	{
		critical_section::scoped_lock lock(m_CriticalSection);
		if (t_ResultX)
		{
			m_Zoom->X.Scale = t_ArgsX.Scale;
			m_Zoom->X.Translate = static_cast<UINT>(t_ArgsX.Translate);
		}
		if (t_ResultY)
		{
			m_Zoom->Y.Scale = t_ArgsY.Scale;
			m_Zoom->Y.Translate = static_cast<UINT>(t_ArgsY.Translate);
		}
		m_Render();
		return true;
	}
	return false;
}

///<summary>水平缩放【单线程调用】</summary>
bool Oscilloscope::ScaleX(const float t_Offset)
{
	ScaleArgs t_ArgsX(&m_Zoom->X, t_Offset);
	auto const t_ResultX = m_Scale(t_ArgsX, m_Width - m_Axis->AxisY_Space);
	if (t_ResultX)
	{
		critical_section::scoped_lock lock(m_CriticalSection);
		m_Zoom->X.Scale = t_ArgsX.Scale;
		m_Zoom->X.Translate = static_cast<UINT>(t_ArgsX.Translate);
		m_Render();
	}
	return t_ResultX;
}

///<summary>垂直缩放【单线程调用】</summary>
bool Oscilloscope::ScaleY(const float t_Offset)
{
	ScaleArgs t_ArgsY(&m_Zoom->Y, t_Offset);
	auto const t_ResultY = m_Scale(t_ArgsY, m_Height - m_Axis->AxisX_Space);
	if (t_ResultY)
	{
		critical_section::scoped_lock lock(m_CriticalSection);
		m_Zoom->Y.Scale = t_ArgsY.Scale;
		m_Zoom->Y.Translate = static_cast<UINT>(t_ArgsY.Translate);
		m_Render();
	}
	return t_ResultY;
}

///<summary>平移【单线程调用】</summary>
bool Oscilloscope::Translate(const int t_OffsetX, const int t_OffsetY)
{
	TranslateArgs t_ArgsX(&m_Zoom->X, -t_OffsetX);
	TranslateArgs t_ArgsY(&m_Zoom->Y, -t_OffsetY);
	auto const t_ResultX = m_Translate(t_ArgsX);
	auto const t_ResultY = m_Translate(t_ArgsY);
	if (t_ResultX || t_ResultY)
	{
		critical_section::scoped_lock lock(m_CriticalSection);
		if (t_ResultX) m_Zoom->X.Translate = static_cast<UINT>(t_ArgsX.Translate);
		if (t_ResultY) m_Zoom->Y.Translate = static_cast<UINT>(t_ArgsY.Translate);
		m_Render();
		return true;
	}
	return false;
}

///<summary>水平平移【单线程调用】</summary>
bool Oscilloscope::TranslateX(const int t_Offset)
{
	TranslateArgs t_ArgsX(&m_Zoom->X, -t_Offset);
	auto const t_ResultX = m_Translate(t_ArgsX);
	if (t_ResultX)
	{
		critical_section::scoped_lock lock(m_CriticalSection);
		m_Zoom->X.Translate = static_cast<UINT>(t_ArgsX.Translate);
		m_Render();
	}
	return t_ResultX;
}

///<summary>垂直平移【单线程调用】</summary>
bool Oscilloscope::TranslateY(const int t_Offset)
{
	TranslateArgs t_ArgsY(&m_Zoom->Y, -t_Offset);
	auto const t_ResultY = m_Translate(t_ArgsY);
	if (t_ResultY)
	{
		critical_section::scoped_lock lock(m_CriticalSection);
		m_Zoom->Y.Translate = static_cast<UINT>(t_ArgsY.Translate);
		m_Render();
	}
	return t_ResultY;
}

///<summary>改变刷新模式【单线程调用】</summary>
void Oscilloscope::ChangeRefreshMode(const Enum_RefreshMode t_RefreshMode)
{
	if (m_RefreshMode != t_RefreshMode)
	{
		critical_section::scoped_lock lock(m_CriticalSection);
		m_RefreshMode = t_RefreshMode;
		m_Render();
	}
}

///<summary>改变基线类型【单线程调用】</summary>
void Oscilloscope::ChangeBaselineType(const UINT t_GraphIndex, const Enum_BaselineType t_BaselineType)
{
	if (m_Items[t_GraphIndex]->DataCache->BaselineType() != t_BaselineType)
	{
		critical_section::scoped_lock lock(m_CriticalSection);
		m_Items[t_GraphIndex]->DataCache->BaselineType(t_BaselineType);
		m_Render();
	}
}

///<summary>改变图形样式【单线程调用】</summary>
void Oscilloscope::ChangeGraphStyles(const Enum_GraphStyle* const t_GraphStyle)
{
	critical_section::scoped_lock lock(m_CriticalSection);
	auto t_Item = m_Items.get();
	auto t_Style = t_GraphStyle;
	for (auto t_Index = 0u; t_Index != m_ItemsSize; ++t_Index, ++t_Item, ++t_Style)
	{
		t_Item->get()->Style = *t_Style;
	}
	m_Render();
}

///<summary>改变图形样式【单线程调用】</summary>
void Oscilloscope::ChangeGraphStyle(const UINT t_GraphIndex, const Enum_GraphStyle t_GraphStyle)
{
	if (m_Items[t_GraphIndex]->Style != t_GraphStyle)
	{
		critical_section::scoped_lock lock(m_CriticalSection);
		m_Items[t_GraphIndex]->Style = t_GraphStyle;
		m_Render();
	}
}

///<summary>改变坐标轴线宽【单线程调用】</summary>
void Oscilloscope::ChangeAxisLineWidth(const UINT t_AxisLineWidth)
{
	if (static_cast<UINT>(m_Axis->AxisLineWidth) != t_AxisLineWidth)
	{
		critical_section::scoped_lock lock(m_CriticalSection);
		m_Axis->AxisLineWidth = static_cast<float>(t_AxisLineWidth);
		m_Render();
	}
}

///<summary>改变坐标轴颜色【单线程调用】</summary>
void Oscilloscope::ChangeAxisColor(const UINT32 t_AxisColor)
{
	if (m_Axis->AxisColorValue != t_AxisColor)
	{
		critical_section::scoped_lock lock(m_CriticalSection);
		m_Axis->AxisColorValue = t_AxisColor;
		m_Axis->AxisColor = ColorF(t_AxisColor, GetAlphaChannel(t_AxisColor));
		if (m_Axis->Color != nullptr) m_Axis->Color->SetColor(m_Axis->AxisColor);
		m_Render();
	}
}

///<summary>改变背景色【单线程调用】</summary>
void Oscilloscope::ChangeBackgroundColor(const UINT32 t_Background)
{
	if (m_BackgroundValue != t_Background)
	{
		critical_section::scoped_lock lock(m_CriticalSection);
		m_BackgroundValue = t_Background;
		m_Background = ColorF(t_Background, GetAlphaChannel(t_Background));
		m_Render();
	}
}

///<summary>改变前景色【单线程调用】</summary>
void Oscilloscope::ChangeForegroundColors(const UINT32* const t_Foreground)
{
	critical_section::scoped_lock lock(m_CriticalSection);
	auto t_Item = m_Items.get();
	auto t_Color = t_Foreground;
	for (auto t_Index = 0u; t_Index != m_ItemsSize; ++t_Index, ++t_Item, ++t_Color)
	{
		t_Item->get()->Foreground = ColorF(*t_Color, GetAlphaChannel(*t_Color));
		if (t_Item->get()->GraphColor != nullptr) t_Item->get()->GraphColor->SetColor(t_Item->get()->Foreground);
	}
	m_Render();
}

///<summary>改变前景色【单线程调用】</summary>
void Oscilloscope::ChangeForegroundColor(const UINT t_GraphIndex, const UINT32 t_Foreground)
{
	if (m_Items[t_GraphIndex]->ForegroundValue != t_Foreground)
	{
		critical_section::scoped_lock lock(m_CriticalSection);
		auto const &t_Item = m_Items[t_GraphIndex];
		t_Item->ForegroundValue = t_Foreground;
		t_Item->Foreground = ColorF(t_Foreground, GetAlphaChannel(t_Foreground));
		if (t_Item->GraphColor != nullptr) t_Item->GraphColor->SetColor(t_Item->Foreground);
		m_Render();
	}
}

///<summary>改变线宽【单线程调用】</summary>
void Oscilloscope::ChangeLineWidth(const UINT t_GraphIndex, const byte t_LineWidth)
{
	if (static_cast<byte>(m_Items[t_GraphIndex]->LineWeight) != t_LineWidth)
	{
		critical_section::scoped_lock lock(m_CriticalSection);
		m_Items[t_GraphIndex]->LineWeight = static_cast<float>(max(t_LineWidth, 1ui8));
		m_Render();
	}
}

///<summary>改变基线偏移值【单线程调用】</summary>
void Oscilloscope::ChangeOffset(const UINT t_GraphIndex, const int t_Offset)
{
	if (static_cast<int>(m_Items[t_GraphIndex]->DataCache->Offset()) != t_Offset)
	{
		critical_section::scoped_lock lock(m_CriticalSection);
		m_Items[t_GraphIndex]->DataCache->Offset(static_cast<float>(t_Offset));
		m_Render();
	}
}

///<summary>改变自动归零【单线程调用】</summary>
void Oscilloscope::ChangeAutoTranslate(const UINT t_GraphIndex, const float t_Offset)
{
	if (m_Items[t_GraphIndex]->DataCache->AutoTranslate() != t_Offset)
	{
		critical_section::scoped_lock lock(m_CriticalSection);
		m_Items[t_GraphIndex]->DataCache->AutoTranslate(t_Offset);
		m_Render();
	}
}

///<summary>是否可用【单线程调用】</summary>
void Oscilloscope::ChangeIsEnabled(const UINT t_GraphIndex, const bool t_IsEnabled) noexcept
{
	if (m_Items[t_GraphIndex]->IsEnabled != t_IsEnabled)
	{
		critical_section::scoped_lock lock(m_CriticalSection);
		m_Items[t_GraphIndex]->IsEnabled = t_IsEnabled;
		m_Render();
	}
}

///<summary>安全存入数据</summary>
void Oscilloscope::SafePush(const UINT t_GraphIndex, const float t_Value) noexcept
{
	m_Items[t_GraphIndex]->DataCache->SafePush(t_Value);
	m_IsNeedRefresh = true;
}

///<summary>存入数据</summary>
void Oscilloscope::Push(const UINT t_GraphIndex, const float t_Value) noexcept
{
	m_Items[t_GraphIndex]->DataCache->Push(t_Value);
	m_IsNeedRefresh = true;
}

///<summary>安全替换全部数据</summary>
void Oscilloscope::SafeReplace(const UINT t_GraphIndex, const float* const t_Value, const UINT t_Length) noexcept
{
	m_Items[t_GraphIndex]->DataCache->SafeReplace(t_Value, t_Length);
	Refresh();
}

///<summary>替换全部数据</summary>
void Oscilloscope::Replace(const UINT t_GraphIndex, const float* const t_Value, const UINT t_Length) noexcept
{
	m_Items[t_GraphIndex]->DataCache->Replace(t_Value, t_Length);
	Refresh();
}

///<summary>安全清空</summary>
void Oscilloscope::SafeClear() noexcept
{
	auto t_Item = m_Items.get();
	for (auto t_Index = 0u; t_Index != m_ItemsSize; ++t_Index, ++t_Item) t_Item->get()->DataCache->SafeClear();
	Refresh();
}

///<summary>清空</summary>
void Oscilloscope::Clear() noexcept
{
	auto t_Item = m_Items.get();
	for (auto t_Index = 0u; t_Index != m_ItemsSize; ++t_Index, ++t_Item) t_Item->get()->DataCache->Clear();
	Refresh();
}

///<summary>改变DPI</summary>
void Oscilloscope::ChangeDPI(const float t_DpiX, const float t_DpiY)
{
	critical_section::scoped_lock lock(m_CriticalSection);
	m_RenderTarget->SetDpi(t_DpiX, t_DpiY);
}

///<summary>改变大小</summary>
void Oscilloscope::Resize()
{
	critical_section::scoped_lock lock(m_CriticalSection);
	CHECK_COM_THROW(m_RenderTarget->Resize(m_GetSize()));
}

///<summary>刷新</summary>
void Oscilloscope::Refresh()
{
	critical_section::scoped_lock lock(m_CriticalSection);
	m_Render();
}

///<summary>渲染</summary>
void Oscilloscope::Render()
{
	if (m_IsNeedRefresh) Refresh();
}

///<summary>重建</summary>
void Oscilloscope::m_Recreate()
{
	CHECK_COM_THROW(m_D2dFactory->CreateHwndRenderTarget(RenderTargetProperties(D2D1_RENDER_TARGET_TYPE::D2D1_RENDER_TARGET_TYPE_DEFAULT, PixelFormat(DXGI_FORMAT::DXGI_FORMAT_UNKNOWN, D2D1_ALPHA_MODE::D2D1_ALPHA_MODE_PREMULTIPLIED)), HwndRenderTargetProperties(m_Handle, m_GetSize()), m_RenderTarget.ReleaseAndGetAddressOf()));
	CHECK_COM_THROW(m_RenderTarget->CreateSolidColorBrush(m_Axis->AxisColor, m_Axis->Color.ReleaseAndGetAddressOf()));
	if (m_ItemsSize > 1u) CHECK_COM_THROW(m_RenderTarget->CreateLayer(m_Layer.ReleaseAndGetAddressOf()));
	auto t_Item = m_Items.get();
	for (auto t_Index = 0u; t_Index != m_ItemsSize; ++t_Index, ++t_Item)
	{
		CHECK_COM_THROW(m_D2dFactory->CreateStrokeStyle(D2D1_STROKE_STYLE_PROPERTIES{ D2D1_CAP_STYLE::D2D1_CAP_STYLE_FLAT, D2D1_CAP_STYLE::D2D1_CAP_STYLE_FLAT, D2D1_CAP_STYLE::D2D1_CAP_STYLE_FLAT, D2D1_LINE_JOIN::D2D1_LINE_JOIN_MITER, 1.0f, D2D1_DASH_STYLE::D2D1_DASH_STYLE_SOLID, 0.0f }, nullptr, 0u, t_Item->get()->LineStyle.ReleaseAndGetAddressOf()));
		CHECK_COM_THROW(m_RenderTarget->CreateSolidColorBrush(t_Item->get()->Foreground, t_Item->get()->GraphColor.ReleaseAndGetAddressOf()));
	}
}

///<summary>获取绘图区尺寸</summary>
D2D1_SIZE_U Oscilloscope::m_GetSize()
{
	RECT t_Rect;
	if (GetClientRect(m_Handle, &t_Rect) == FALSE) throw 0;
	auto const t_Width = t_Rect.right - t_Rect.left;
	auto const t_Height = t_Rect.bottom - t_Rect.top;
	m_Width = static_cast<float>(t_Width);
	m_Height = static_cast<float>(t_Height);
	return SizeU(t_Width, t_Height);
}

///<summary>初始化</summary>
void Oscilloscope::m_Initialize(ZoomParameters &t_Args, const float t_Size)
{
	if (t_Args.Scale < 1.0f)
	{
		t_Args.Scale = 1.0f;
	}
	else
	{
		if (t_Args.Scale > t_Args.Scale_Limit) t_Args.Scale = t_Args.Scale_Limit;
		if (t_Args.Scale != 1.0f) t_Args.Translate_Limit = static_cast<int>(fma(t_Size, t_Args.Scale, -t_Size));
	}
	if (t_Args.Translate > t_Args.Translate_Limit) t_Args.Translate = t_Args.Translate_Limit;
}

///<summary>改变缩放极限值【单线程调用】</summary>
bool Oscilloscope::m_ChangeScaleLimit(ScaleArgs &t_Args, const float t_Size, const float t_Limit)
{
	if (t_Limit >= t_Args.Args->Scale_Limit && t_Args.Args->Scale <= t_Limit)
	{
		t_Args.Args->Scale_Limit = t_Limit;
		return false;
	}
	else
	{
		t_Args.Args->Scale_Limit = t_Limit;
		t_Args.Scale = t_Limit;
		return m_Scale(t_Args, t_Size);
	}
}

///<summary>缩放【单线程调用】</summary>
bool Oscilloscope::m_Scale(ScaleArgs &t_Args, const float t_Size)
{
	if (t_Args.Scale < 1.0f)
	{
		t_Args.Scale = 1.0f;
	}
	else if (t_Args.Scale > t_Args.Args->Scale_Limit)
	{
		t_Args.Scale = t_Args.Args->Scale_Limit;
	}
	if (t_Args.Scale != t_Args.Args->Scale)
	{
		auto const t_ScaledSize = t_Size * t_Args.Scale;
		t_Args.Args->Translate_Limit = static_cast<int>(t_ScaledSize - t_Size);
		TranslateArgs t_TranslateArgs(t_Args.Args, static_cast<int>(round(fma(t_Size, t_Args.Args->Scale, -t_ScaledSize) / 2.0f)));
		m_Translate(t_TranslateArgs);
		t_Args.Translate = t_TranslateArgs.Translate;
		return true;
	}
	return false;
}

///<summary>平移【单线程调用】</summary>
bool Oscilloscope::m_Translate(TranslateArgs &t_Args)
{
	if (t_Args.Translate < 0)
	{
		t_Args.Translate = 0;
	}
	else
	{
		auto const t_Limit = static_cast<int>(t_Args.Args->Translate_Limit);
		if (t_Args.Translate > t_Limit) t_Args.Translate = t_Limit;
	}
	return t_Args.Translate != static_cast<int>(t_Args.Args->Translate);
}

///<summary>渲染</summary>
void Oscilloscope::m_Render()
{
	m_IsNeedRefresh = false;
	do
	{
		m_RenderTarget->BeginDraw(); //开始绘制
		m_RenderTarget->Clear(m_Background); //用背景色清屏
		//计算出绘制区域的大小和位置
		auto t_Clip = RectF(0.0f, 0.0f, m_Width, m_Height); //绘图区初始范围
		auto const t_IsAxisEnabled_X = m_Axis->AxisX_IsEnabled && m_Axis->AxisX_TicksCount > 1u; //是否绘制横轴
		auto const t_IsAxisEnabled_Y = m_Axis->AxisY_IsEnabled && m_Axis->AxisY_TicksCount > 1u; //是否绘制纵轴
		if (t_IsAxisEnabled_X)
		{
			switch (m_Axis->AxisX_Placement)
			{
			case Enum_AxisPlacement_X::Bottom:
				t_Clip.bottom -= m_Axis->AxisX_Height; //横轴在底部时占用的空间
				break;
			case Enum_AxisPlacement_X::Top:
				t_Clip.top = m_Axis->AxisX_Height; //横轴在顶部时占用的中间
				break;
			default:
				break;
			}
		}
		if (t_IsAxisEnabled_Y)
		{
			switch (m_Axis->AxisY_Placement)
			{
			case Enum_AxisPlacement_Y::Left:
				t_Clip.left = m_Axis->AxisY_Width; //纵轴在左侧时占用的空间
				break;
			case Enum_AxisPlacement_Y::Right:
				t_Clip.right -= m_Axis->AxisY_Width; //纵轴在右侧时占用的空间
				break;
			default:
				break;
			}
		}
		auto const t_Size = SizeF((t_Clip.right - t_Clip.left) * m_Zoom->X.Scale, (t_Clip.bottom - t_Clip.top) * m_Zoom->Y.Scale); //除去坐标轴并进行了缩放的绘图区尺寸
		auto const t_Zero = Point2F(t_Clip.left - static_cast<float>(m_Zoom->X.Translate), t_Clip.top - static_cast<float>(m_Zoom->Y.Translate)); //绘图区移动后的原点位置
		auto const t_Area = RectF(t_Zero.x, t_Zero.y, t_Zero.x + t_Size.width, t_Zero.y + t_Size.height); //实际绘图区域的范围
		auto const t_Step = t_Size.width / static_cast<float>(m_DatalLength);
		auto const t_HalfStep = t_Step / 2.0f;
		//开始绘制图形
		if (m_ItemsSize > 0u)
		{
			auto t_Item = m_Items.get();
			if (m_RefreshMode == Enum_RefreshMode::Scanning)
			{
				if (m_ItemsSize == 1u)
				{
					m_RenderTarget->PushLayer(m_LayerParameters, m_Layer.Get());
					if (t_Item->get()->IsEnabled && t_Item->get()->DataCache->IsNotEmpty(t_Item->get()->Style)) m_Draw_Scanning(t_Item->get(), t_Clip, t_Size, t_Area, t_Step); //绘制仅有的一个图形
					m_RenderTarget->PopLayer();
				}
				else
				{
					for (auto t_Index = 0u; t_Index != m_ItemsSize; ++t_Index, ++t_Item)
					{
						m_RenderTarget->PushLayer(m_LayerParameters, m_Layer.Get());
						if (t_Item->get()->IsEnabled && t_Item->get()->DataCache->IsNotEmpty(t_Item->get()->Style)) m_Draw_Scanning(t_Item->get(), t_Clip, t_Size, t_Area, t_Step); //绘制每一个图形
						m_RenderTarget->PopLayer();
					}
				}
			}
			else
			{
				if (m_ItemsSize == 1u)
				{
					if (t_Item->get()->IsEnabled && t_Item->get()->DataCache->IsNotEmpty(t_Item->get()->Style)) m_Draw_Scrolling(t_Item->get(), t_Clip, t_Size, t_Area, t_Step); //绘制仅有的一个图形
				}
				else
				{
					for (auto t_Index = 0u; t_Index != m_ItemsSize; ++t_Index, ++t_Item)
					{
						if (t_Item->get()->IsEnabled && t_Item->get()->DataCache->IsNotEmpty(t_Item->get()->Style)) m_Draw_Scrolling(t_Item->get(), t_Clip, t_Size, t_Area, t_Step); //绘制每一个图形
					}
				}
			}
		}
		//开始绘制坐标轴
		auto const t_AxisColor = m_Axis->Color.Get();
		auto const t_HalfLineWidth = m_Axis->AxisLineWidth / 2.0f;
		if (t_IsAxisEnabled_X)
		{
			//横轴的绘制范围和轴线大小及位置
			auto t_AxisAreaX = RectF(0.0f, 0.0f, m_Width, 0.0f);
			switch (m_Axis->AxisX_Placement)
			{
			case Enum_AxisPlacement_X::Bottom:
				t_AxisAreaX.top = m_Height - m_Axis->AxisLineWidth;
				t_Clip.top = m_Height - m_Axis->AxisX_TickHeight;
				t_Clip.bottom = m_Height;
				break;
			case Enum_AxisPlacement_X::Center:
				t_AxisAreaX.top = floor(fma(t_Size.height, 0.5f, t_Zero.y) - t_HalfLineWidth);
				t_Clip.top = 0.0f;
				t_Clip.bottom = m_Height;
				break;
			case Enum_AxisPlacement_X::Top:
				t_Clip.top = 0.0f;
				t_Clip.bottom = m_Axis->AxisX_TickHeight;
				break;
			default:
				break;
			}
			t_AxisAreaX.bottom = t_AxisAreaX.top + m_Axis->AxisLineWidth;
			switch (m_Axis->AxisY_Placement)
			{
			case Enum_AxisPlacement_Y::Left:
				t_Clip.left = m_Axis->AxisY_Width;
				t_Clip.right = m_Width;
				break;
			case Enum_AxisPlacement_Y::Center:
				t_Clip.left = 0.0;
				t_Clip.right = m_Width;
				break;
			case Enum_AxisPlacement_Y::Right:
				t_Clip.left = 0.0;
				t_Clip.right = m_Width - m_Axis->AxisY_Width;
				break;
			default:
				break;
			}
			t_Clip.left = floor(t_Clip.left + t_HalfStep);
			t_Clip.right = ceil(t_Clip.right - t_HalfStep);
			m_RenderTarget->PushAxisAlignedClip(t_Clip, D2D1_ANTIALIAS_MODE::D2D1_ANTIALIAS_MODE_ALIASED);
			m_RenderTarget->FillRectangle(t_AxisAreaX, t_AxisColor); //绘制轴线
			//横轴的刻度线
			if (m_Axis->AxisX_Placement != Enum_AxisPlacement_X::Center)
			{
				t_AxisAreaX.top = 0.0f;
				t_AxisAreaX.bottom = m_Height;
			}
			else
			{
				t_AxisAreaX.top = floor(fma(t_Size.height, 0.5f, t_Zero.y) - (m_Axis->AxisX_TickHeight / 2.0f));
				t_AxisAreaX.bottom = t_AxisAreaX.top + m_Axis->AxisX_TickHeight;
			}
			auto t_TickSize = m_Axis->AxisX_TicksCount;
			auto const t_AxisXStep = (t_Size.width - t_Step) / static_cast<float>(t_TickSize);
			auto t_Left = t_Area.left + t_HalfStep;
			auto t_Right = t_Left + m_Axis->AxisLineWidth;
			for (auto t_Index = 0u; t_Index != t_TickSize; ++t_Index)
			{
				t_AxisAreaX.left = floor(t_Left);
				t_AxisAreaX.right = floor(t_Right);
				m_RenderTarget->FillRectangle(t_AxisAreaX, t_AxisColor); //绘制每一条刻度线
				t_Left += t_AxisXStep;
				t_Right += t_AxisXStep;
			}
			t_AxisAreaX.right = floor(t_Area.right - t_HalfStep);
			t_AxisAreaX.left = t_AxisAreaX.right - m_Axis->AxisLineWidth;
			m_RenderTarget->FillRectangle(t_AxisAreaX, t_AxisColor); //绘制最后的刻度线
			m_RenderTarget->PopAxisAlignedClip();
		}
		if (t_IsAxisEnabled_Y)
		{
			//纵轴的绘制范围和轴线大小及位置
			auto t_AxisAreaY = RectF(0.0f, 0.0f, 0.0f, m_Height);
			switch (m_Axis->AxisY_Placement)
			{
			case Enum_AxisPlacement_Y::Left:
				t_Clip.left = 0.0;
				t_Clip.right = m_Axis->AxisY_TickWidth;
				break;
			case Enum_AxisPlacement_Y::Center:
				t_AxisAreaY.left = floor(fma(t_Size.width, 0.5f, t_Zero.x) - t_HalfLineWidth);
				t_Clip.left = 0.0f;
				t_Clip.right = m_Width;
				break;
			case Enum_AxisPlacement_Y::Right:
				t_AxisAreaY.left = m_Width - m_Axis->AxisLineWidth;
				t_Clip.left = m_Width - m_Axis->AxisY_TickWidth;
				t_Clip.right = m_Width;
				break;
			default:
				break;
			}
			t_AxisAreaY.right = t_AxisAreaY.left + m_Axis->AxisLineWidth;
			switch (m_Axis->AxisX_Placement)
			{
			case Enum_AxisPlacement_X::Bottom:
				t_Clip.top = 0.0f;
				t_Clip.bottom = m_Height - m_Axis->AxisX_Height;
				break;
			case Enum_AxisPlacement_X::Center:
				t_Clip.top = 0.0f;
				t_Clip.bottom = m_Height;
				break;
			case Enum_AxisPlacement_X::Top:
				t_Clip.top = m_Axis->AxisX_Height;
				t_Clip.bottom = m_Height;
				break;
			default:
				break;
			}
			m_RenderTarget->PushAxisAlignedClip(t_Clip, D2D1_ANTIALIAS_MODE::D2D1_ANTIALIAS_MODE_ALIASED);
			m_RenderTarget->FillRectangle(t_AxisAreaY, t_AxisColor); //绘制轴线
			//纵轴的刻度线
			if (m_Axis->AxisY_Placement != Enum_AxisPlacement_Y::Center)
			{
				t_AxisAreaY.left = 0.0f;
				t_AxisAreaY.right = m_Width;
			}
			else
			{
				t_AxisAreaY.left = floor(fma(t_Size.width, 0.5f, t_Zero.x) - (m_Axis->AxisY_TickWidth / 2.0f));
				t_AxisAreaY.right = t_AxisAreaY.left + m_Axis->AxisY_TickWidth;
			}
			auto t_TickSize = m_Axis->AxisY_TicksCount;
			auto const t_AxisYStep = t_Size.height / static_cast<float>(t_TickSize);
			auto t_Bottom = t_Area.bottom;
			auto t_Top = t_Bottom - m_Axis->AxisLineWidth;
			for (auto t_Index = 0u; t_Index != t_TickSize; ++t_Index)
			{
				t_AxisAreaY.top = floor(t_Top);
				t_AxisAreaY.bottom = floor(t_Bottom);
				m_RenderTarget->FillRectangle(t_AxisAreaY, t_AxisColor); //绘制每一条刻度线
				t_Top -= t_AxisYStep;
				t_Bottom -= t_AxisYStep;
			}
			t_AxisAreaY.top = ceil(t_Area.top);
			t_AxisAreaY.bottom = t_AxisAreaY.top + m_Axis->AxisLineWidth;
			m_RenderTarget->FillRectangle(t_AxisAreaY, t_AxisColor); //绘制最后的刻度线
			m_RenderTarget->PopAxisAlignedClip();
		}
		auto const t_Result = m_RenderTarget->EndDraw(); //完成绘制
		if (t_Result == D2DERR_RECREATE_TARGET)
		{
			//重建绘制目标，并利用循环（do...while...）重新开始绘制。
			m_Recreate();
		}
		else
		{
			//成功或遇到无法恢复的错误时结束绘制。
			CHECK_COM_THROW(t_Result);
			break;
		}
	} while (true);
}

///<summary>绘制：扫描模式</summary>
void Oscilloscope::m_Draw_Scanning(OscilloscopeItemStruct* const t_Item, const D2D1_RECT_F &t_Clip, const D2D1_SIZE_F &t_Size, const D2D1_RECT_F &t_Area, const float t_Step)
{
	//开始绘制图形
	m_RenderTarget->PushAxisAlignedClip(t_Clip, D2D1_ANTIALIAS_MODE::D2D1_ANTIALIAS_MODE_ALIASED);
	switch (t_Item->Style)
	{
	case Enum_GraphStyle::Bar:
	{
		//柱状图
		auto t_Temp = t_Item->DataCache->FillData_Bar(m_RenderTarget.Get(), t_Item->GraphColor.Get(), t_Step, t_Area, t_Clip, m_RefreshMode);
		if (t_Temp.first != t_Temp.second && t_Temp.second > t_Clip.left && t_Temp.first < t_Clip.right)
		{
			auto t_Scanline = RectF(t_Temp.first, 0.0f, t_Temp.second, t_Size.height);
			if (t_Scanline.left < t_Clip.left) t_Scanline.left = t_Clip.left;
			if (t_Scanline.right > t_Clip.right) t_Scanline.right = t_Clip.right;
			if (t_Scanline.top < t_Clip.top) t_Scanline.top = t_Clip.top;
			if (t_Scanline.bottom > t_Clip.bottom) t_Scanline.bottom = t_Clip.bottom;
			m_RenderTarget->PopAxisAlignedClip();
			m_RenderTarget->PushAxisAlignedClip(t_Scanline, D2D1_ANTIALIAS_MODE::D2D1_ANTIALIAS_MODE_ALIASED);
			m_RenderTarget->Clear(); //擦除扫描线区域
		}
	}
	break;
	case Enum_GraphStyle::Line:
	{
		//折线图
		CHECK_COM_THROW(m_D2dFactory->CreatePathGeometry(t_Item->Geometry.ReleaseAndGetAddressOf()));
		ComPtr<ID2D1GeometrySink> t_GeometrySink;
		CHECK_COM_THROW(t_Item->Geometry->Open(t_GeometrySink.GetAddressOf()));
		bool t_Result;
		auto t_Temp = t_Item->DataCache->FillData_Line(t_GeometrySink.Get(), t_Step, t_Area, t_Clip, m_RefreshMode, t_Result);
		if (t_Result)
		{
			m_RenderTarget->DrawGeometry(t_Item->Geometry.Get(), t_Item->GraphColor.Get(), t_Item->LineWeight, t_Item->LineStyle.Get());
			if (t_Temp.first != t_Temp.second && t_Temp.second > t_Clip.left && t_Temp.first < t_Clip.right)
			{
				auto t_Scanline = RectF(t_Temp.first - t_Item->LineWeight, 0.0f, t_Temp.second + t_Item->LineWeight, t_Size.height);
				if (t_Scanline.left < t_Clip.left) t_Scanline.left = t_Clip.left;
				if (t_Scanline.right > t_Clip.right) t_Scanline.right = t_Clip.right;
				if (t_Scanline.top < t_Clip.top) t_Scanline.top = t_Clip.top;
				if (t_Scanline.bottom > t_Clip.bottom) t_Scanline.bottom = t_Clip.bottom;
				m_RenderTarget->PopAxisAlignedClip();
				m_RenderTarget->PushAxisAlignedClip(t_Scanline, D2D1_ANTIALIAS_MODE::D2D1_ANTIALIAS_MODE_ALIASED);
				m_RenderTarget->Clear(); //擦除扫描线区域
			}
		}
	}
	break;
	default:
		break;
	}
	m_RenderTarget->PopAxisAlignedClip();
}

///<summary>绘制：滚动模式</summary>
void Oscilloscope::m_Draw_Scrolling(OscilloscopeItemStruct* const t_Item, const D2D1_RECT_F &t_Clip, const D2D1_SIZE_F &t_Size, const D2D1_RECT_F &t_Area, const float t_Step)
{
	//开始绘制图形
	m_RenderTarget->PushAxisAlignedClip(t_Clip, D2D1_ANTIALIAS_MODE::D2D1_ANTIALIAS_MODE_ALIASED);
	switch (t_Item->Style)
	{
	case Enum_GraphStyle::Bar:
	{
		//柱状图
		t_Item->DataCache->FillData_Bar(m_RenderTarget.Get(), t_Item->GraphColor.Get(), t_Step, t_Area, t_Clip, m_RefreshMode);
	}
	break;
	case Enum_GraphStyle::Line:
	{
		//折线图
		CHECK_COM_THROW(m_D2dFactory->CreatePathGeometry(t_Item->Geometry.ReleaseAndGetAddressOf()));
		ComPtr<ID2D1GeometrySink> t_GeometrySink;
		CHECK_COM_THROW(t_Item->Geometry->Open(t_GeometrySink.GetAddressOf()));
		bool t_Result;
		t_Item->DataCache->FillData_Line(t_GeometrySink.Get(), t_Step, t_Area, t_Clip, m_RefreshMode, t_Result);
		if (t_Result) m_RenderTarget->DrawGeometry(t_Item->Geometry.Get(), t_Item->GraphColor.Get(), t_Item->LineWeight, t_Item->LineStyle.Get());
	}
	break;
	default:
		break;
	}
	m_RenderTarget->PopAxisAlignedClip();
}