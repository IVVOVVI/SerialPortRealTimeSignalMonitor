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

///<summary>ʾ������ṹ</summary>
Oscilloscope::OscilloscopeItemStruct::~OscilloscopeItemStruct()
{
	delete DataCache;
}

///<summary>ʾ������ṹ</summary>
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

///<summary>ƽ�Ʋ���</summary>
Oscilloscope::TranslateArgs::TranslateArgs(ZoomParameters* const t_Args, const int t_Offset) :
	Args(t_Args),
	Translate(static_cast<int>(t_Args->Translate) - t_Offset)
{}

///<summary>���Ų���</summary>
Oscilloscope::ScaleArgs::ScaleArgs(ZoomParameters* const t_Args, const float t_Offset) :
	Args(t_Args),
	Scale(t_Args->Scale + t_Offset)
{}

///<summary>ʾ����</summary>
Oscilloscope::~Oscilloscope()
{
	delete m_Zoom;
	delete m_Axis;
}

///<summary>ʾ����</summary>
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

///<summary>ˮƽ�����ʡ����̵߳��á�</summary>
float Oscilloscope::ScaleX() const noexcept
{
	return m_Zoom->X.Scale;
}

///<summary>ˮƽ�����ʼ���ֵ�����̵߳��á�</summary>
float Oscilloscope::ScaleX_Limit() const noexcept
{
	return m_Zoom->X.Scale_Limit;
}

///<summary>ˮƽλ�ơ����̵߳��á�</summary>
int Oscilloscope::TranslateX() const noexcept
{
	return m_Zoom->X.Translate;
}

///<summary>ˮƽλ�Ƽ���ֵ�����̵߳��á�</summary>
int Oscilloscope::TranslateX_Limit() const noexcept
{
	return m_Zoom->X.Translate_Limit;
}

///<summary>��ֱ�����ʡ����̵߳��á�</summary>
float Oscilloscope::ScaleY() const noexcept
{
	return m_Zoom->Y.Scale;
}

///<summary>��ֱ�����ʼ���ֵ�����̵߳��á�</summary>
float Oscilloscope::ScaleY_Limit() const noexcept
{
	return m_Zoom->Y.Scale_Limit;
}

///<summary>��ֱλ�ơ����̵߳��á�</summary>
int Oscilloscope::TranslateY() const noexcept
{
	return m_Zoom->Y.Translate;
}

///<summary>��ֱλ�Ƽ���ֵ�����̵߳��á�</summary>
int Oscilloscope::TranslateY_Limit() const noexcept
{
	return m_Zoom->Y.Translate_Limit;
}

///<summary>�ı����ż���ֵ�����̵߳��á�</summary>
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

///<summary>�ı�ˮƽ���ż���ֵ�����̵߳��á�</summary>
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

///<summary>�ı䴹ֱ���ż���ֵ�����̵߳��á�</summary>
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

///<summary>���š����̵߳��á�</summary>
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

///<summary>ˮƽ���š����̵߳��á�</summary>
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

///<summary>��ֱ���š����̵߳��á�</summary>
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

///<summary>ƽ�ơ����̵߳��á�</summary>
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

///<summary>ˮƽƽ�ơ����̵߳��á�</summary>
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

///<summary>��ֱƽ�ơ����̵߳��á�</summary>
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

///<summary>�ı�ˢ��ģʽ�����̵߳��á�</summary>
void Oscilloscope::ChangeRefreshMode(const Enum_RefreshMode t_RefreshMode)
{
	if (m_RefreshMode != t_RefreshMode)
	{
		critical_section::scoped_lock lock(m_CriticalSection);
		m_RefreshMode = t_RefreshMode;
		m_Render();
	}
}

///<summary>�ı�������͡����̵߳��á�</summary>
void Oscilloscope::ChangeBaselineType(const UINT t_GraphIndex, const Enum_BaselineType t_BaselineType)
{
	if (m_Items[t_GraphIndex]->DataCache->BaselineType() != t_BaselineType)
	{
		critical_section::scoped_lock lock(m_CriticalSection);
		m_Items[t_GraphIndex]->DataCache->BaselineType(t_BaselineType);
		m_Render();
	}
}

///<summary>�ı�ͼ����ʽ�����̵߳��á�</summary>
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

///<summary>�ı�ͼ����ʽ�����̵߳��á�</summary>
void Oscilloscope::ChangeGraphStyle(const UINT t_GraphIndex, const Enum_GraphStyle t_GraphStyle)
{
	if (m_Items[t_GraphIndex]->Style != t_GraphStyle)
	{
		critical_section::scoped_lock lock(m_CriticalSection);
		m_Items[t_GraphIndex]->Style = t_GraphStyle;
		m_Render();
	}
}

///<summary>�ı��������߿����̵߳��á�</summary>
void Oscilloscope::ChangeAxisLineWidth(const UINT t_AxisLineWidth)
{
	if (static_cast<UINT>(m_Axis->AxisLineWidth) != t_AxisLineWidth)
	{
		critical_section::scoped_lock lock(m_CriticalSection);
		m_Axis->AxisLineWidth = static_cast<float>(t_AxisLineWidth);
		m_Render();
	}
}

///<summary>�ı���������ɫ�����̵߳��á�</summary>
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

///<summary>�ı䱳��ɫ�����̵߳��á�</summary>
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

///<summary>�ı�ǰ��ɫ�����̵߳��á�</summary>
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

///<summary>�ı�ǰ��ɫ�����̵߳��á�</summary>
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

///<summary>�ı��߿����̵߳��á�</summary>
void Oscilloscope::ChangeLineWidth(const UINT t_GraphIndex, const byte t_LineWidth)
{
	if (static_cast<byte>(m_Items[t_GraphIndex]->LineWeight) != t_LineWidth)
	{
		critical_section::scoped_lock lock(m_CriticalSection);
		m_Items[t_GraphIndex]->LineWeight = static_cast<float>(max(t_LineWidth, 1ui8));
		m_Render();
	}
}

///<summary>�ı����ƫ��ֵ�����̵߳��á�</summary>
void Oscilloscope::ChangeOffset(const UINT t_GraphIndex, const int t_Offset)
{
	if (static_cast<int>(m_Items[t_GraphIndex]->DataCache->Offset()) != t_Offset)
	{
		critical_section::scoped_lock lock(m_CriticalSection);
		m_Items[t_GraphIndex]->DataCache->Offset(static_cast<float>(t_Offset));
		m_Render();
	}
}

///<summary>�ı��Զ����㡾���̵߳��á�</summary>
void Oscilloscope::ChangeAutoTranslate(const UINT t_GraphIndex, const float t_Offset)
{
	if (m_Items[t_GraphIndex]->DataCache->AutoTranslate() != t_Offset)
	{
		critical_section::scoped_lock lock(m_CriticalSection);
		m_Items[t_GraphIndex]->DataCache->AutoTranslate(t_Offset);
		m_Render();
	}
}

///<summary>�Ƿ���á����̵߳��á�</summary>
void Oscilloscope::ChangeIsEnabled(const UINT t_GraphIndex, const bool t_IsEnabled) noexcept
{
	if (m_Items[t_GraphIndex]->IsEnabled != t_IsEnabled)
	{
		critical_section::scoped_lock lock(m_CriticalSection);
		m_Items[t_GraphIndex]->IsEnabled = t_IsEnabled;
		m_Render();
	}
}

///<summary>��ȫ��������</summary>
void Oscilloscope::SafePush(const UINT t_GraphIndex, const float t_Value) noexcept
{
	m_Items[t_GraphIndex]->DataCache->SafePush(t_Value);
	m_IsNeedRefresh = true;
}

///<summary>��������</summary>
void Oscilloscope::Push(const UINT t_GraphIndex, const float t_Value) noexcept
{
	m_Items[t_GraphIndex]->DataCache->Push(t_Value);
	m_IsNeedRefresh = true;
}

///<summary>��ȫ�滻ȫ������</summary>
void Oscilloscope::SafeReplace(const UINT t_GraphIndex, const float* const t_Value, const UINT t_Length) noexcept
{
	m_Items[t_GraphIndex]->DataCache->SafeReplace(t_Value, t_Length);
	Refresh();
}

///<summary>�滻ȫ������</summary>
void Oscilloscope::Replace(const UINT t_GraphIndex, const float* const t_Value, const UINT t_Length) noexcept
{
	m_Items[t_GraphIndex]->DataCache->Replace(t_Value, t_Length);
	Refresh();
}

///<summary>��ȫ���</summary>
void Oscilloscope::SafeClear() noexcept
{
	auto t_Item = m_Items.get();
	for (auto t_Index = 0u; t_Index != m_ItemsSize; ++t_Index, ++t_Item) t_Item->get()->DataCache->SafeClear();
	Refresh();
}

///<summary>���</summary>
void Oscilloscope::Clear() noexcept
{
	auto t_Item = m_Items.get();
	for (auto t_Index = 0u; t_Index != m_ItemsSize; ++t_Index, ++t_Item) t_Item->get()->DataCache->Clear();
	Refresh();
}

///<summary>�ı�DPI</summary>
void Oscilloscope::ChangeDPI(const float t_DpiX, const float t_DpiY)
{
	critical_section::scoped_lock lock(m_CriticalSection);
	m_RenderTarget->SetDpi(t_DpiX, t_DpiY);
}

///<summary>�ı��С</summary>
void Oscilloscope::Resize()
{
	critical_section::scoped_lock lock(m_CriticalSection);
	CHECK_COM_THROW(m_RenderTarget->Resize(m_GetSize()));
}

///<summary>ˢ��</summary>
void Oscilloscope::Refresh()
{
	critical_section::scoped_lock lock(m_CriticalSection);
	m_Render();
}

///<summary>��Ⱦ</summary>
void Oscilloscope::Render()
{
	if (m_IsNeedRefresh) Refresh();
}

///<summary>�ؽ�</summary>
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

///<summary>��ȡ��ͼ���ߴ�</summary>
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

///<summary>��ʼ��</summary>
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

///<summary>�ı����ż���ֵ�����̵߳��á�</summary>
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

///<summary>���š����̵߳��á�</summary>
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

///<summary>ƽ�ơ����̵߳��á�</summary>
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

///<summary>��Ⱦ</summary>
void Oscilloscope::m_Render()
{
	m_IsNeedRefresh = false;
	do
	{
		m_RenderTarget->BeginDraw(); //��ʼ����
		m_RenderTarget->Clear(m_Background); //�ñ���ɫ����
		//�������������Ĵ�С��λ��
		auto t_Clip = RectF(0.0f, 0.0f, m_Width, m_Height); //��ͼ����ʼ��Χ
		auto const t_IsAxisEnabled_X = m_Axis->AxisX_IsEnabled && m_Axis->AxisX_TicksCount > 1u; //�Ƿ���ƺ���
		auto const t_IsAxisEnabled_Y = m_Axis->AxisY_IsEnabled && m_Axis->AxisY_TicksCount > 1u; //�Ƿ��������
		if (t_IsAxisEnabled_X)
		{
			switch (m_Axis->AxisX_Placement)
			{
			case Enum_AxisPlacement_X::Bottom:
				t_Clip.bottom -= m_Axis->AxisX_Height; //�����ڵײ�ʱռ�õĿռ�
				break;
			case Enum_AxisPlacement_X::Top:
				t_Clip.top = m_Axis->AxisX_Height; //�����ڶ���ʱռ�õ��м�
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
				t_Clip.left = m_Axis->AxisY_Width; //���������ʱռ�õĿռ�
				break;
			case Enum_AxisPlacement_Y::Right:
				t_Clip.right -= m_Axis->AxisY_Width; //�������Ҳ�ʱռ�õĿռ�
				break;
			default:
				break;
			}
		}
		auto const t_Size = SizeF((t_Clip.right - t_Clip.left) * m_Zoom->X.Scale, (t_Clip.bottom - t_Clip.top) * m_Zoom->Y.Scale); //��ȥ�����Ტ���������ŵĻ�ͼ���ߴ�
		auto const t_Zero = Point2F(t_Clip.left - static_cast<float>(m_Zoom->X.Translate), t_Clip.top - static_cast<float>(m_Zoom->Y.Translate)); //��ͼ���ƶ����ԭ��λ��
		auto const t_Area = RectF(t_Zero.x, t_Zero.y, t_Zero.x + t_Size.width, t_Zero.y + t_Size.height); //ʵ�ʻ�ͼ����ķ�Χ
		auto const t_Step = t_Size.width / static_cast<float>(m_DatalLength);
		auto const t_HalfStep = t_Step / 2.0f;
		//��ʼ����ͼ��
		if (m_ItemsSize > 0u)
		{
			auto t_Item = m_Items.get();
			if (m_RefreshMode == Enum_RefreshMode::Scanning)
			{
				if (m_ItemsSize == 1u)
				{
					m_RenderTarget->PushLayer(m_LayerParameters, m_Layer.Get());
					if (t_Item->get()->IsEnabled && t_Item->get()->DataCache->IsNotEmpty(t_Item->get()->Style)) m_Draw_Scanning(t_Item->get(), t_Clip, t_Size, t_Area, t_Step); //���ƽ��е�һ��ͼ��
					m_RenderTarget->PopLayer();
				}
				else
				{
					for (auto t_Index = 0u; t_Index != m_ItemsSize; ++t_Index, ++t_Item)
					{
						m_RenderTarget->PushLayer(m_LayerParameters, m_Layer.Get());
						if (t_Item->get()->IsEnabled && t_Item->get()->DataCache->IsNotEmpty(t_Item->get()->Style)) m_Draw_Scanning(t_Item->get(), t_Clip, t_Size, t_Area, t_Step); //����ÿһ��ͼ��
						m_RenderTarget->PopLayer();
					}
				}
			}
			else
			{
				if (m_ItemsSize == 1u)
				{
					if (t_Item->get()->IsEnabled && t_Item->get()->DataCache->IsNotEmpty(t_Item->get()->Style)) m_Draw_Scrolling(t_Item->get(), t_Clip, t_Size, t_Area, t_Step); //���ƽ��е�һ��ͼ��
				}
				else
				{
					for (auto t_Index = 0u; t_Index != m_ItemsSize; ++t_Index, ++t_Item)
					{
						if (t_Item->get()->IsEnabled && t_Item->get()->DataCache->IsNotEmpty(t_Item->get()->Style)) m_Draw_Scrolling(t_Item->get(), t_Clip, t_Size, t_Area, t_Step); //����ÿһ��ͼ��
					}
				}
			}
		}
		//��ʼ����������
		auto const t_AxisColor = m_Axis->Color.Get();
		auto const t_HalfLineWidth = m_Axis->AxisLineWidth / 2.0f;
		if (t_IsAxisEnabled_X)
		{
			//����Ļ��Ʒ�Χ�����ߴ�С��λ��
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
			m_RenderTarget->FillRectangle(t_AxisAreaX, t_AxisColor); //��������
			//����Ŀ̶���
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
				m_RenderTarget->FillRectangle(t_AxisAreaX, t_AxisColor); //����ÿһ���̶���
				t_Left += t_AxisXStep;
				t_Right += t_AxisXStep;
			}
			t_AxisAreaX.right = floor(t_Area.right - t_HalfStep);
			t_AxisAreaX.left = t_AxisAreaX.right - m_Axis->AxisLineWidth;
			m_RenderTarget->FillRectangle(t_AxisAreaX, t_AxisColor); //�������Ŀ̶���
			m_RenderTarget->PopAxisAlignedClip();
		}
		if (t_IsAxisEnabled_Y)
		{
			//����Ļ��Ʒ�Χ�����ߴ�С��λ��
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
			m_RenderTarget->FillRectangle(t_AxisAreaY, t_AxisColor); //��������
			//����Ŀ̶���
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
				m_RenderTarget->FillRectangle(t_AxisAreaY, t_AxisColor); //����ÿһ���̶���
				t_Top -= t_AxisYStep;
				t_Bottom -= t_AxisYStep;
			}
			t_AxisAreaY.top = ceil(t_Area.top);
			t_AxisAreaY.bottom = t_AxisAreaY.top + m_Axis->AxisLineWidth;
			m_RenderTarget->FillRectangle(t_AxisAreaY, t_AxisColor); //�������Ŀ̶���
			m_RenderTarget->PopAxisAlignedClip();
		}
		auto const t_Result = m_RenderTarget->EndDraw(); //��ɻ���
		if (t_Result == D2DERR_RECREATE_TARGET)
		{
			//�ؽ�����Ŀ�꣬������ѭ����do...while...�����¿�ʼ���ơ�
			m_Recreate();
		}
		else
		{
			//�ɹ��������޷��ָ��Ĵ���ʱ�������ơ�
			CHECK_COM_THROW(t_Result);
			break;
		}
	} while (true);
}

///<summary>���ƣ�ɨ��ģʽ</summary>
void Oscilloscope::m_Draw_Scanning(OscilloscopeItemStruct* const t_Item, const D2D1_RECT_F &t_Clip, const D2D1_SIZE_F &t_Size, const D2D1_RECT_F &t_Area, const float t_Step)
{
	//��ʼ����ͼ��
	m_RenderTarget->PushAxisAlignedClip(t_Clip, D2D1_ANTIALIAS_MODE::D2D1_ANTIALIAS_MODE_ALIASED);
	switch (t_Item->Style)
	{
	case Enum_GraphStyle::Bar:
	{
		//��״ͼ
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
			m_RenderTarget->Clear(); //����ɨ��������
		}
	}
	break;
	case Enum_GraphStyle::Line:
	{
		//����ͼ
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
				m_RenderTarget->Clear(); //����ɨ��������
			}
		}
	}
	break;
	default:
		break;
	}
	m_RenderTarget->PopAxisAlignedClip();
}

///<summary>���ƣ�����ģʽ</summary>
void Oscilloscope::m_Draw_Scrolling(OscilloscopeItemStruct* const t_Item, const D2D1_RECT_F &t_Clip, const D2D1_SIZE_F &t_Size, const D2D1_RECT_F &t_Area, const float t_Step)
{
	//��ʼ����ͼ��
	m_RenderTarget->PushAxisAlignedClip(t_Clip, D2D1_ANTIALIAS_MODE::D2D1_ANTIALIAS_MODE_ALIASED);
	switch (t_Item->Style)
	{
	case Enum_GraphStyle::Bar:
	{
		//��״ͼ
		t_Item->DataCache->FillData_Bar(m_RenderTarget.Get(), t_Item->GraphColor.Get(), t_Step, t_Area, t_Clip, m_RefreshMode);
	}
	break;
	case Enum_GraphStyle::Line:
	{
		//����ͼ
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