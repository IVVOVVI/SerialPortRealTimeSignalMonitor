#pragma once

#include "ColorHelper.h"
#include "ConstructionArgs.h"

using namespace D2D1;

namespace DllOscilloscope
{

	///<summary>���������</summary>
	struct AxisProperties
	{

		///<summary>���Ų���</summary>
		///<param name="t_Args">�����ò���</param>
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

		///<summary>��ɫ</summary>
		Microsoft::WRL::ComPtr<ID2D1SolidColorBrush> Color;

		///<summary>��ɫ</summary>
		UINT32 AxisColorValue;

		///<summary>��ɫ</summary>
		D2D1_COLOR_F AxisColor;

		///<summary>�������߿�</summary>
		float AxisLineWidth;

		///<summary>����̶ȵ���������ֵΪ0ʱ�����ƿ̶ȡ���</summary>
		const UINT AxisX_TicksCount;

		///<summary>����̶ȵ���������ֵΪ0ʱ�����ƿ̶ȡ���</summary>
		const UINT AxisY_TicksCount;

		///<summary>����̶ȵĸ߶�</summary>
		const float AxisX_TickHeight;

		///<summary>����̶ȵĿ��</summary>
		const float AxisY_TickWidth;

		///<summary>����߶�</summary>
		const float AxisX_Height;

		///<summary>������</summary>
		const float AxisY_Width;

		///<summary>����λ��</summary>
		const Enum_AxisPlacement_X AxisX_Placement;

		///<summary>����λ��</summary>
		const Enum_AxisPlacement_Y AxisY_Placement;

		///<summary>�����Ƿ�����</summary>
		const bool AxisX_IsEnabled;

		///<summary>�����Ƿ�����</summary>
		const bool AxisY_IsEnabled;

		///<summary>����ռ�õĿռ�</summary>
		const float AxisX_Space;

		///<summary>����ռ�õĿռ�</summary>
		const float AxisY_Space;

	};

}