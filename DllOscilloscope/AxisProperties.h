#pragma once

#include "ColorHelper.h"
#include "ConstructionArgs.h"

using namespace D2D1;

namespace DllOscilloscope
{

	///<summary>坐标轴参数</summary>
	struct AxisProperties
	{

		///<summary>缩放参数</summary>
		///<param name="t_Args">构造用参数</param>
		AxisProperties(const ConstructionArgs* const t_Args) :
			Color(nullptr),
			AxisColorValue(t_Args->AxisColor),
			AxisColor(ColorF(t_Args->AxisColor, GetAlphaChannel(t_Args->AxisColor))),
			AxisLineWidth(static_cast<float>(t_Args->AxisLineWidth)),
			AxisX_TicksCount(t_Args->AxisX_TicksCount),
			AxisY_TicksCount(t_Args->AxisY_TicksCount),
			AxisX_TickHeight(static_cast<float>(t_Args->AxisX_TickHeight)),
			AxisY_TickWidth(static_cast<float>(t_Args->AxisY_TickWidth)),
			AxisX_Height(static_cast<float>(t_Args->AxisX_Height)),
			AxisY_Width(static_cast<float>(t_Args->AxisY_Width)),
			AxisX_Placement(t_Args->AxisX_Placement),
			AxisY_Placement(t_Args->AxisY_Placement),
			AxisX_IsEnabled(t_Args->AxisX_TicksCount != 0u),
			AxisY_IsEnabled(t_Args->AxisY_TicksCount != 0u),
			AxisX_Space((AxisX_IsEnabled && AxisX_Placement != Enum_AxisPlacement_X::Center) ? AxisX_Height : 0.0f),
			AxisY_Space((AxisY_IsEnabled && AxisY_Placement != Enum_AxisPlacement_Y::Center) ? AxisY_Width : 0.0f)
		{}

		///<summary>颜色</summary>
		Microsoft::WRL::ComPtr<ID2D1SolidColorBrush> Color;

		///<summary>颜色</summary>
		UINT32 AxisColorValue;

		///<summary>颜色</summary>
		D2D1_COLOR_F AxisColor;

		///<summary>坐标轴线宽</summary>
		float AxisLineWidth;

		///<summary>横轴刻度的数量（此值为0时不绘制刻度。）</summary>
		const UINT AxisX_TicksCount;

		///<summary>纵轴刻度的数量（此值为0时不绘制刻度。）</summary>
		const UINT AxisY_TicksCount;

		///<summary>横轴刻度的高度</summary>
		const float AxisX_TickHeight;

		///<summary>纵轴刻度的宽度</summary>
		const float AxisY_TickWidth;

		///<summary>横轴高度</summary>
		const float AxisX_Height;

		///<summary>纵轴宽度</summary>
		const float AxisY_Width;

		///<summary>横轴位置</summary>
		const Enum_AxisPlacement_X AxisX_Placement;

		///<summary>纵轴位置</summary>
		const Enum_AxisPlacement_Y AxisY_Placement;

		///<summary>横轴是否启用</summary>
		const bool AxisX_IsEnabled;

		///<summary>纵轴是否启用</summary>
		const bool AxisY_IsEnabled;

		///<summary>横轴占用的空间</summary>
		const float AxisX_Space;

		///<summary>纵轴占用的空间</summary>
		const float AxisY_Space;

	};

}