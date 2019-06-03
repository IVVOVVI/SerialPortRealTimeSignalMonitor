using System.Runtime.InteropServices;

namespace Oscilloscope
{

	///<summary>构造用参数</summary>
	[StructLayoutAttribute(LayoutKind.Sequential)]
	public struct ConstructionArgs
	{

		///<summary>背景色</summary>
		public uint Background;

		///<summary>要呈现的数据量</summary>
		public uint Count;

		///<summary>量程（例如：[-50,50]的量程为100）</summary>
		public float Range;

		///<summary>水平缩放率</summary>
		public float ScaleX;

		///<summary>水平最大缩放率</summary>
		public float ScaleLimitX;

		///<summary>垂直缩放率</summary>
		public float ScaleY;

		///<summary>垂直最大缩放率</summary>
		public float ScaleLimitY;

		///<summary>水平位移值</summary>
		public uint TranslateX;

		///<summary>垂直位移值</summary>
		public uint TranslateY;

		///<summary>刷新模式</summary>
		public Enum_RefreshMode RefreshMode;

		///<summary>坐标轴颜色</summary>
		public uint AxisColor;

		///<summary>坐标轴线宽</summary>
		public uint AxisLineWidth;

		///<summary>横轴刻度的数量（不包含原点刻度，即总刻度数量为此设定值加1。此值为0时不绘制刻度。）</summary>
		public uint AxisX_TicksCount;

		///<summary>纵轴刻度的数量（不包含原点刻度，即总刻度数量为此设定值加1。此值为0时不绘制刻度。）</summary>
		public uint AxisY_TicksCount;

		///<summary>横轴刻度的高度</summary>
		public uint AxisX_TickHeight;

		///<summary>纵轴刻度的宽度</summary>
		public uint AxisY_TickWidth;

		///<summary>横轴高度</summary>
		public uint AxisX_Height;

		///<summary>纵轴宽度</summary>
		public uint AxisY_Width;

		///<summary>横轴位置</summary>
		public Enum_AxisPlacement_X AxisX_Placement;

		///<summary>纵轴位置</summary>
		public Enum_AxisPlacement_Y AxisY_Placement;

	}

}