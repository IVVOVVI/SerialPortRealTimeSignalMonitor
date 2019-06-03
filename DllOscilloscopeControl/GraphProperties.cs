using System.Runtime.InteropServices;

namespace Oscilloscope
{

	///<summary>图形参数</summary>
	[StructLayoutAttribute(LayoutKind.Sequential)]
	public struct GraphProperties
	{

		///<summary>基线类型</summary>
		public Enum_BaselineType BaselineType;

		///<summary>图形样式</summary>
		public Enum_GraphStyle GraphStyle;

		///<summary>前景色</summary>
		public uint Foreground;

		///<summary>线宽（只有线条类图形有效）</summary>
		public uint LineWidth;

		///<summary>基线偏移值</summary>
		public int Offset;

		///<summary>自动归零（如果是非数值则不使用此功能，否则将最接近横轴的数值归零后加上此值。）</summary>
		public float AutoTranslate;

		///<summary>是否启用</summary>
		public bool IsEnabled;

	}

}