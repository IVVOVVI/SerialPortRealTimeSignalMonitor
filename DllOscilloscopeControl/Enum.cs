namespace Oscilloscope
{

	///<summary>基线类型（Bottom：横轴在下方，即只接收正数值。 | Center：横轴在中间，即正数和负数均可。 | Top：横轴在上方，即直接收负数值。）</summary>
	public enum Enum_BaselineType : uint { Bottom, Center, Top };

	///<summary>图形样式</summary>
	public enum Enum_GraphStyle : uint { Bar, Line };

	///<summary>刷新模式</summary>
	public enum Enum_RefreshMode : uint { Scanning, Scrolling };

	///<summary>横轴位置</summary>
	public enum Enum_AxisPlacement_X : uint { Bottom, Center, Top };

	///<summary>纵轴位置</summary>
	public enum Enum_AxisPlacement_Y : uint { Left, Center, Right };

}