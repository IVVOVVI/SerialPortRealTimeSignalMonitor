#pragma once

#include "Common.h"

namespace DllOscilloscope
{

	///<summary>图形参数</summary>
	struct GraphProperties
	{

		///<summary>基线类型</summary>
		Enum_BaselineType BaselineType;

		///<summary>图形样式</summary>
		Enum_GraphStyle GraphStyle;

		///<summary>前景色</summary>
		UINT32 Foreground;

		///<summary>线宽（只有线条类图形有效）</summary>
		UINT32 LineWidth;

		///<summary>基线偏移值</summary>
		int Offset;

		///<summary>自动归零（如果是非数值则不使用此功能，否则将最接近横轴的数值归零后加上此值。）</summary>
		float AutoTranslate;

		///<summary>是否启用</summary>
		bool IsEnabled;

	};

}