using System;

namespace Oscilloscope
{

	///<summary>构造参数</summary>
	public class ConstructionParameters
	{

		///<summary>构造参数（多图形）</summary>
		///<param name="window">窗口</param>
		///<param name="background">背景色</param>
		///<param name="count">要呈现的数据量</param>
		///<param name="range">量程（例如：[-50,50]的量程为100）</param>
		///<param name="refreshMode">刷新模式</param>
		///<param name="scaleX">水平缩放率</param>
		///<param name="scaleLimitX">水平最大缩放率</param>
		///<param name="ScaleY">垂直缩放率</param>
		///<param name="ScaleLimitY">垂直最大缩放率</param>
		///<param name="translateX">水平位移值</param>
		///<param name="translateY">垂直位移值</param>
		///<param name="graphProperties">图形参数数组</param>
		public ConstructionParameters(IWindow window, uint background, uint count, float range, float scaleX, float scaleLimitX, float scaleY, float scaleLimitY, uint translateX, uint translateY, Enum_RefreshMode refreshMode, GraphProperties[] graphProperties)
		{
			Oscilloscopes = window.Oscilloscopes;
			ControlHwnd = IntPtr.Zero;
			GraphProperties = graphProperties;
			Args = new ConstructionArgs { Background = background, Count = count, Range = range, ScaleX = scaleX, ScaleLimitX = scaleLimitX, ScaleY = scaleY, ScaleLimitY = scaleLimitY, TranslateX = translateX, TranslateY = translateY, RefreshMode = refreshMode, AxisColor = 0u, AxisLineWidth = 0u, AxisX_TicksCount = 0u, AxisY_TicksCount = 0u, AxisX_TickHeight = 0u, AxisY_TickWidth = 0u, AxisX_Height = 0u, AxisY_Width = 0u, AxisX_Placement = Enum_AxisPlacement_X.Bottom, AxisY_Placement = Enum_AxisPlacement_Y.Left };
		}

		///<summary>构造参数（多图形）</summary>
		///<param name="window">窗口</param>
		///<param name="background">背景色</param>
		///<param name="count">要呈现的数据量</param>
		///<param name="range">量程（例如：[-50,50]的量程为100）</param>
		///<param name="refreshMode">刷新模式</param>
		///<param name="scaleX">水平缩放率</param>
		///<param name="scaleLimitX">水平最大缩放率</param>
		///<param name="ScaleY">垂直缩放率</param>
		///<param name="ScaleLimitY">垂直最大缩放率</param>
		///<param name="translateX">水平位移值</param>
		///<param name="translateY">垂直位移值</param>
		///<param name="axisColor">坐标轴颜色</param>
		///<param name="axisLineWidth">坐标轴线宽</param>
		///<param name="axisX_TicksCount">横轴刻度的数量（不包含原点刻度，即总刻度数量为此设定值加1。此值为0时不绘制刻度。）</param>
		///<param name="axisY_TicksCount">纵轴刻度的数量（不包含原点刻度，即总刻度数量为此设定值加1。此值为0时不绘制刻度。）</param>
		///<param name="axisX_TickHeight">横轴刻度的高度</param>
		///<param name="axisY_TickWidth">纵轴刻度的宽度</param>
		///<param name="axisX_Height">横轴高度</param>
		///<param name="axisY_Width">纵轴宽度</param>
		///<param name="axisX_Placement">横轴位置</param>
		///<param name="axisY_Placement">纵轴位置</param>
		///<param name="graphProperties">图形参数数组</param>
		public ConstructionParameters(IWindow window, uint background, uint count, float range, float scaleX, float scaleLimitX, float scaleY, float scaleLimitY, uint translateX, uint translateY, Enum_RefreshMode refreshMode, uint axisColor, uint axisLineWidth, uint axisX_TicksCount, uint axisY_TicksCount, uint axisX_TickHeight, uint axisY_TickWidth, uint axisX_Height, uint axisY_Width, Enum_AxisPlacement_X axisX_Placement, Enum_AxisPlacement_Y axisY_Placement, GraphProperties[] graphProperties)
		{
			Oscilloscopes = window.Oscilloscopes;
			ControlHwnd = IntPtr.Zero;
			GraphProperties = graphProperties;
			Args = new ConstructionArgs { Background = background, Count = count, Range = range, ScaleX = scaleX, ScaleLimitX = scaleLimitX, ScaleY = scaleY, ScaleLimitY = scaleLimitY, TranslateX = translateX, TranslateY = translateY, RefreshMode = refreshMode, AxisColor = axisColor, AxisLineWidth = axisLineWidth, AxisX_TicksCount = axisX_TicksCount, AxisY_TicksCount = axisY_TicksCount, AxisX_TickHeight = axisX_TickHeight, AxisY_TickWidth = axisY_TickWidth, AxisX_Height = axisX_Height, AxisY_Width = axisY_Width, AxisX_Placement = axisX_Placement, AxisY_Placement = axisY_Placement };
		}

		///<summary>示波器组</summary>
		internal IntPtr Oscilloscopes { get; private set; }

		///<summary>绘图区句柄</summary>
		internal IntPtr ControlHwnd { get; set; }

		///<summary>图形参数数组</summary>
		public GraphProperties[] GraphProperties { get; private set; }

		///<summary>背景色</summary>
		public uint Background { get { return Args.Background; } }

		///<summary>要呈现的数据量</summary>
		public uint Count { get { return Args.Count; } }

		///<summary>量程（例如：[-50,50]的量程为100）</summary>
		public float Range { get { return Args.Range; } }

		///<summary>刷新模式</summary>
		public Enum_RefreshMode RefreshMode { get { return Args.RefreshMode; } }

		///<summary>坐标轴颜色</summary>
		public uint AxisColor { get { return Args.AxisColor; } }

		///<summary>坐标轴线宽</summary>
		public uint AxisLineWidth { get { return Args.AxisLineWidth; } }

		///<summary>横轴刻度的数量（不包含原点刻度，即总刻度数量为此设定值加1。此值为0时不绘制刻度。）</summary>
		public uint AxisX_TicksCount { get { return Args.AxisX_TicksCount; } }

		///<summary>纵轴刻度的数量（不包含原点刻度，即总刻度数量为此设定值加1。此值为0时不绘制刻度。）</summary>
		public uint AxisY_TicksCount { get { return Args.AxisY_TicksCount; } }

		///<summary>横轴刻度的高度</summary>
		public uint AxisX_TickHeight { get { return Args.AxisX_TickHeight; } }

		///<summary>纵轴刻度的宽度</summary>
		public uint AxisY_TickWidth { get { return Args.AxisY_TickWidth; } }

		///<summary>横轴高度</summary>
		public uint AxisX_Height { get { return Args.AxisX_Height; } }

		///<summary>纵轴宽度</summary>
		public uint AxisY_Width { get { return Args.AxisY_Width; } }

		///<summary>横轴位置</summary>
		public Enum_AxisPlacement_X AxisX_Placement { get { return Args.AxisX_Placement; } }

		///<summary>纵轴位置</summary>
		public Enum_AxisPlacement_Y AxisY_Placement { get { return Args.AxisY_Placement; } }

		///<summary>参数</summary>
		internal ConstructionArgs Args;

	}

}