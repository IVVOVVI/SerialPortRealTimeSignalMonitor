#pragma once

namespace DllOscilloscope
{

	///<summary>构造用参数</summary>
	struct ConstructionArgs
	{

		///<summary>背景色</summary>
		UINT32 Background;

		///<summary>要呈现的数据量</summary>
		UINT32 Count;

		///<summary>量程（例如：[-50,50]的量程为100）</summary>
		float Range;

		///<summary>水平缩放率</summary>
		float ScaleX;

		///<summary>水平最大缩放率</summary>
		float ScaleLimitX;

		///<summary>垂直缩放率</summary>
		float ScaleY;

		///<summary>垂直最大缩放率</summary>
		float ScaleLimitY;

		///<summary>水平位移值</summary>
		UINT TranslateX;

		///<summary>垂直位移值</summary>
		UINT TranslateY;

		///<summary>刷新模式</summary>
		Enum_RefreshMode RefreshMode;

		///<summary>坐标轴颜色</summary>
		UINT32 AxisColor;

		///<summary>坐标轴线宽</summary>
		UINT AxisLineWidth;

		///<summary>横轴刻度的数量（不包含原点刻度，即总刻度数量为此设定值加1。此值为0时不绘制刻度。）</summary>
		UINT AxisX_TicksCount;

		///<summary>纵轴刻度的数量（不包含原点刻度，即总刻度数量为此设定值加1。此值为0时不绘制刻度。）</summary>
		UINT AxisY_TicksCount;

		///<summary>横轴刻度的高度</summary>
		UINT AxisX_TickHeight;

		///<summary>纵轴刻度的宽度</summary>
		UINT AxisY_TickWidth;

		///<summary>横轴高度</summary>
		UINT AxisX_Height;

		///<summary>纵轴宽度</summary>
		UINT AxisY_Width;

		///<summary>横轴位置</summary>
		Enum_AxisPlacement_X AxisX_Placement;

		///<summary>纵轴位置</summary>
		Enum_AxisPlacement_Y AxisY_Placement;

	};

}