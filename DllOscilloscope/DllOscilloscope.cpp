#include "stdafx.h"
#include "Oscilloscope.h"
#include "Oscilloscopes.h"
#include "GraphProperties.h"

#define DLL_EXPORT(type) extern "C" __declspec(dllexport) type __stdcall

using namespace DllOscilloscope;

///<summary>创建示波器组</summary>
///<param name="t_WindowHwnd">窗口句柄</param>
///<param name="t_Fps">帧率（最小值：30）</param>
DLL_EXPORT(void*) CreateOscilloscopes(HWND t_WindowHwnd, byte t_Fps)
{
	return Oscilloscopes::Add(t_WindowHwnd, t_Fps);
}

///<summary>关闭示波器组</summary>
///<param name="t_Ptr">由Initialize返回的指针</param>
DLL_EXPORT(void) CloseOscilloscopes(Oscilloscopes* t_Oscilloscopes)
{
	Oscilloscopes::Remove(t_Oscilloscopes);
}

///<summary>创建示波器</summary>
///<param name="t_Oscilloscopes">示波器组</param>
///<param name="t_ControlHwnd">绘图区句柄</param>
///<param name="t_Args">构造用参数</param>
///<param name="t_GraphProperties">图形参数数组</param>
///<param name="t_Size">数组大小</param>
DLL_EXPORT(void*) CreateOscilloscope(Oscilloscopes* t_Oscilloscopes, HWND const t_ControlHwnd, const ConstructionArgs* t_Args, const GraphProperties* t_GraphProperties, UINT t_Size)
{
	return t_Oscilloscopes->CreateOscilloscope(t_ControlHwnd, t_Args, t_GraphProperties, t_Size);
}

///<summary>刷新DPI</summary>
///<param name="t_Oscilloscopes">示波器组</param>
DLL_EXPORT(void) RefreshDPI(Oscilloscopes* t_Oscilloscopes)
{
	t_Oscilloscopes->RefreshDPI();
}

///<summary>水平缩放率【单线程调用】</summary>
///<param name="t_Oscilloscope">示波器</param>
DLL_EXPORT(float) ScaleX(Oscilloscope* t_Oscilloscope)
{
	return t_Oscilloscope->ScaleX();
}

///<summary>水平缩放率极限值【单线程调用】</summary>
///<param name="t_Oscilloscope">示波器</param>
DLL_EXPORT(float) ScaleX_Limit(Oscilloscope* t_Oscilloscope)
{
	return t_Oscilloscope->ScaleX_Limit();
}

///<summary>水平位移【单线程调用】</summary>
///<param name="t_Oscilloscope">示波器</param>
DLL_EXPORT(int) TranslateX(Oscilloscope* t_Oscilloscope)
{
	return t_Oscilloscope->TranslateX();
}

///<summary>水平位移极限值【单线程调用】</summary>
///<param name="t_Oscilloscope">示波器</param>
DLL_EXPORT(int) TranslateX_Limit(Oscilloscope* t_Oscilloscope)
{
	return t_Oscilloscope->TranslateX_Limit();
}

///<summary>垂直缩放率【单线程调用】</summary>
///<param name="t_Oscilloscope">示波器</param>
DLL_EXPORT(float) ScaleY(Oscilloscope* t_Oscilloscope)
{
	return t_Oscilloscope->ScaleY();
}

///<summary>垂直缩放率极限值【单线程调用】</summary>
///<param name="t_Oscilloscope">示波器</param>
DLL_EXPORT(float) ScaleY_Limit(Oscilloscope* t_Oscilloscope)
{
	return t_Oscilloscope->ScaleY_Limit();
}

///<summary>垂直位移【单线程调用】</summary>
///<param name="t_Oscilloscope">示波器</param>
DLL_EXPORT(int) TranslateY(Oscilloscope* t_Oscilloscope)
{
	return t_Oscilloscope->TranslateY();
}

///<summary>垂直位移极限值【单线程调用】</summary>
///<param name="t_Oscilloscope">示波器</param>
DLL_EXPORT(int) TranslateY_Limit(Oscilloscope* t_Oscilloscope)
{
	return t_Oscilloscope->TranslateY_Limit();
}

///<summary>改变缩放极限值【单线程调用】</summary>
///<param name="t_Oscilloscope">示波器</param>
///<param name="t_Value">值（>=1）</param>
DLL_EXPORT(bool) ChangeScaleLimit(Oscilloscope* t_Oscilloscope, UINT t_Value)
{
	return t_Oscilloscope->ChangeScaleLimit(t_Value);
}

///<summary>改变水平缩放极限值【单线程调用】</summary>
///<param name="t_Oscilloscope">示波器</param>
///<param name="t_Value">值（>=1）</param>
DLL_EXPORT(bool) ChangeScaleLimitX(Oscilloscope* t_Oscilloscope, UINT t_Value)
{
	return t_Oscilloscope->ChangeScaleLimitX(t_Value);
}

///<summary>改变垂直缩放极限值【单线程调用】</summary>
///<param name="t_Oscilloscope">示波器</param>
///<param name="t_Value">值（>=1）</param>
DLL_EXPORT(bool) ChangeScaleLimitY(Oscilloscope* t_Oscilloscope, UINT t_Value)
{
	return t_Oscilloscope->ChangeScaleLimitY(t_Value);
}

///<summary>改变缩放【单线程调用】</summary>
///<param name="t_Oscilloscope">示波器</param>
///<param name="t_Offset">差值</param>
DLL_EXPORT(bool) ChangeScale(Oscilloscope* t_Oscilloscope, float t_Offset)
{
	return t_Oscilloscope->Scale(t_Offset);
}

///<summary>改变水平缩放【单线程调用】</summary>
///<param name="t_Oscilloscope">示波器</param>
///<param name="t_Offset">差值</param>
DLL_EXPORT(bool) ChangeScaleX(Oscilloscope* t_Oscilloscope, float t_Offset)
{
	return t_Oscilloscope->ScaleX(t_Offset);
}

///<summary>改变垂直缩放【单线程调用】</summary>
///<param name="t_Oscilloscope">示波器</param>
///<param name="t_Offset">差值</param>
DLL_EXPORT(bool) ChangeScaleY(Oscilloscope* t_Oscilloscope, float t_Offset)
{
	return t_Oscilloscope->ScaleY(t_Offset);
}

///<summary>改变平移【单线程调用】</summary>
///<param name="t_Oscilloscope">示波器</param>
///<param name="t_OffsetX">水平位移值</param>
///<param name="t_OffsetY">垂直位移值</param>
DLL_EXPORT(bool) ChangeTranslate(Oscilloscope* t_Oscilloscope, int t_OffsetX, int t_OffsetY)
{
	return t_Oscilloscope->Translate(t_OffsetX, t_OffsetY);
}

///<summary>改变水平平移【单线程调用】</summary>
///<param name="t_Oscilloscope">示波器</param>
///<param name="t_Offset">位移值</param>
DLL_EXPORT(bool) ChangeTranslateX(Oscilloscope* t_Oscilloscope, int t_Offset)
{
	return t_Oscilloscope->TranslateX(t_Offset);
}

///<summary>改变垂直平移【单线程调用】</summary>
///<param name="t_Oscilloscope">示波器</param>
///<param name="t_Offset">位移值</param>
DLL_EXPORT(bool) ChangeTranslateY(Oscilloscope* t_Oscilloscope, int t_Offset)
{
	return t_Oscilloscope->TranslateY(t_Offset);
}

///<summary>改变示波器刷新模式【单线程调用】</summary>
///<param name="t_Oscilloscope">示波器</param>
///<param name="t_RefreshMode">刷新模式</param>
DLL_EXPORT(void) ChangeRefreshMode(Oscilloscope* t_Oscilloscope, Enum_RefreshMode t_RefreshMode)
{
	t_Oscilloscope->ChangeRefreshMode(t_RefreshMode);
}

///<summary>改变基线类型【单线程调用】</summary>
///<param name="t_Oscilloscope">示波器</param>
///<param name="t_GraphIndex">图形序号</param>
///<param name="t_BaselineType">基线类型</param>
DLL_EXPORT(void) ChangeBaselineType(Oscilloscope* t_Oscilloscope, UINT t_GraphIndex, Enum_BaselineType t_BaselineType)
{
	t_Oscilloscope->ChangeBaselineType(t_GraphIndex, t_BaselineType);
}

///<summary>改变示波器图形样式【单线程调用】</summary>
///<param name="t_Oscilloscope">示波器</param>
///<param name="t_GraphStyle">图形样式数组</param>
DLL_EXPORT(void) ChangeGraphStyles(Oscilloscope* t_Oscilloscope, const Enum_GraphStyle* t_GraphStyle)
{
	t_Oscilloscope->ChangeGraphStyles(t_GraphStyle);
}

///<summary>改变示波器图形样式【单线程调用】</summary>
///<param name="t_Oscilloscope">示波器</param>
///<param name="t_GraphIndex">图形序号</param>
///<param name="t_GraphStyle">图形样式</param>
DLL_EXPORT(void) ChangeGraphStyle(Oscilloscope* t_Oscilloscope, UINT t_GraphIndex, Enum_GraphStyle t_GraphStyle)
{
	t_Oscilloscope->ChangeGraphStyle(t_GraphIndex, t_GraphStyle);
}

///<summary>改变坐标轴线宽【单线程调用】</summary>
///<param name="t_Oscilloscope">示波器</param>
///<param name="t_AxisLineWidth">坐标轴线宽</param>
DLL_EXPORT(void) ChangeAxisLineWidth(Oscilloscope* t_Oscilloscope, UINT t_AxisLineWidth)
{
	t_Oscilloscope->ChangeAxisLineWidth(t_AxisLineWidth);
}

///<summary>改变坐标轴颜色【单线程调用】</summary>
///<param name="t_Oscilloscope">示波器</param>
///<param name="t_AxisColor">坐标轴颜色</param>
DLL_EXPORT(void) ChangeAxisColor(Oscilloscope* t_Oscilloscope, UINT32 t_AxisColor)
{
	t_Oscilloscope->ChangeAxisColor(t_AxisColor);
}

///<summary>改变示波器背景色【单线程调用】</summary>
///<param name="t_Oscilloscope">示波器</param>
///<param name="t_Background">背景色</param>
DLL_EXPORT(void) ChangeBackgroundColor(Oscilloscope* t_Oscilloscope, UINT32 t_Background)
{
	t_Oscilloscope->ChangeBackgroundColor(t_Background);
}

///<summary>改变示波器前景色【单线程调用】</summary>
///<param name="t_Oscilloscope">示波器</param>
///<param name="t_Foreground">前景色数组</param>
DLL_EXPORT(void) ChangeForegroundColors(Oscilloscope* t_Oscilloscope, const UINT32* t_Foreground)
{
	t_Oscilloscope->ChangeForegroundColors(t_Foreground);
}

///<summary>改变示波器前景色【单线程调用】</summary>
///<param name="t_Oscilloscope">示波器</param>
///<param name="t_GraphIndex">图形序号</param>
///<param name="t_Foreground">前景色</param>
DLL_EXPORT(void) ChangeForegroundColor(Oscilloscope* t_Oscilloscope, UINT t_GraphIndex, UINT32 t_Foreground)
{
	t_Oscilloscope->ChangeForegroundColor(t_GraphIndex, t_Foreground);
}

///<summary>改变线宽【单线程调用】</summary>
///<param name="t_Oscilloscope">示波器</param>
///<param name="t_GraphIndex">图形序号</param>
///<param name="t_LineWidth">线宽</param>
DLL_EXPORT(void) ChangeLineWidth(Oscilloscope* t_Oscilloscope, UINT t_GraphIndex, byte t_LineWidth)
{
	t_Oscilloscope->ChangeLineWidth(t_GraphIndex, t_LineWidth);
}

///<summary>改变基线偏移值【单线程调用】</summary>
///<param name="t_Oscilloscope">示波器</param>
///<param name="t_GraphIndex">图形序号</param>
///<param name="t_Offset">基线偏移值</param>
DLL_EXPORT(void) ChangeOffset(Oscilloscope* t_Oscilloscope, UINT t_GraphIndex, int t_Offset)
{
	t_Oscilloscope->ChangeOffset(t_GraphIndex, t_Offset);
}

///<summary>改变是否自动归零【单线程调用】</summary>
///<param name="t_Oscilloscope">示波器</param>
///<param name="t_GraphIndex">图形序号</param>
///<param name="t_IsAutoTranslate">是否自动归零</param>
DLL_EXPORT(void) ChangeIsAutoTranslate(Oscilloscope* t_Oscilloscope, UINT t_GraphIndex, bool t_IsAutoTranslate)
{
	t_Oscilloscope->ChangeAutoTranslate(t_GraphIndex, t_IsAutoTranslate ? 0.0f : NAN);
}

///<summary>改变自动归零【单线程调用】</summary>
///<param name="t_Oscilloscope">示波器</param>
///<param name="t_GraphIndex">图形序号</param>
///<param name="t_AutoTranslateOffset">自动归零偏移值（如果是非数值则不使用此功能，否则将最接近横轴的数值归零后加上此值。）</param>
DLL_EXPORT(void) ChangeAutoTranslate(Oscilloscope* t_Oscilloscope, UINT t_GraphIndex, float t_AutoTranslateOffset)
{
	t_Oscilloscope->ChangeAutoTranslate(t_GraphIndex, t_AutoTranslateOffset);
}

///<summary>示波器图形是否可用【单线程调用】</summary>
///<param name="t_Oscilloscope">示波器</param>
///<param name="t_GraphIndex">图形序号</param>
///<param name="t_IsEnabled">是否可用</param>
DLL_EXPORT(void) ChangeIsEnabled(Oscilloscope* t_Oscilloscope, UINT t_GraphIndex, bool t_IsEnabled)
{
	t_Oscilloscope->ChangeIsEnabled(t_GraphIndex, t_IsEnabled);
}

///<summary>安全存入数据（并行安全）</summary>
///<param name="t_Oscilloscope">示波器</param>
///<param name="t_GraphIndex">图形序号</param>
///<param name="t_Value">值</param>
DLL_EXPORT(void) SafePush(Oscilloscope* t_Oscilloscope, UINT t_GraphIndex, float t_Value)
{
	t_Oscilloscope->SafePush(t_GraphIndex, t_Value);
}

///<summary>存入数据</summary>
///<param name="t_Oscilloscope">示波器</param>
///<param name="t_GraphIndex">图形序号</param>
///<param name="t_Value">值</param>
DLL_EXPORT(void) Push(Oscilloscope* t_Oscilloscope, UINT t_GraphIndex, float t_Value)
{
	t_Oscilloscope->Push(t_GraphIndex, t_Value);
}

///<summary>安全替换全部数据（并行安全）</summary>
///<param name="t_Oscilloscope">示波器</param>
///<param name="t_GraphIndex">图形序号</param>
///<param name="t_Value">值</param>
///<param name="t_Length">值数量</param>
DLL_EXPORT(void) SafeReplace(Oscilloscope* t_Oscilloscope, UINT t_GraphIndex, const float* t_Value, UINT t_Length)
{
	t_Oscilloscope->SafeReplace(t_GraphIndex, t_Value, t_Length);
}

///<summary>替换全部数据</summary>
///<param name="t_Oscilloscope">示波器</param>
///<param name="t_GraphIndex">图形序号</param>
///<param name="t_Value">值</param>
///<param name="t_Length">值数量</param>
DLL_EXPORT(void) Replace(Oscilloscope* t_Oscilloscope, UINT t_GraphIndex, const float* t_Value, UINT t_Length)
{
	t_Oscilloscope->Replace(t_GraphIndex, t_Value, t_Length);
}

///<summary>安全清空（并行安全）</summary>
///<param name="t_Oscilloscope">示波器</param>
DLL_EXPORT(void) SafeClear(Oscilloscope* t_Oscilloscope)
{
	t_Oscilloscope->SafeClear();
}

///<summary>清空</summary>
///<param name="t_Oscilloscope">示波器</param>
DLL_EXPORT(void) Clear(Oscilloscope* t_Oscilloscope)
{
	t_Oscilloscope->Clear();
}

///<summary>改变示波器尺寸</summary>
///<param name="t_Oscilloscope">示波器</param>
DLL_EXPORT(void) Resize(Oscilloscope* t_Oscilloscope)
{
	t_Oscilloscope->Resize();
}

///<summary>刷新示波器</summary>
///<param name="t_Oscilloscope">示波器</param>
DLL_EXPORT(void) Refresh(Oscilloscope* t_Oscilloscope)
{
	t_Oscilloscope->Refresh();
}