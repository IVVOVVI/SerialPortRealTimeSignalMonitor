using System;
using System.Runtime.InteropServices;

namespace Oscilloscope
{

	static class Dll86
	{

		private const string DllFileName = @"x86/DllOscilloscope.dll";

		///<summary>创建示波器组</summary>
		///<param name="windowHwnd">窗口句柄</param>
		///<param name="fps">帧率（最小值：30）</param>
		[DllImport(DllFileName)]
		internal extern static IntPtr CreateOscilloscopes(IntPtr windowHwnd, byte fps);

		///<summary>关闭示波器组</summary>
		///<param name="ptr">由Initialize返回的指针</param>
		[DllImport(DllFileName)]
		internal extern static void CloseOscilloscopes(IntPtr oscilloscopes);

		///<summary>创建示波器（多图形）</summary>
		///<param name="oscilloscopes">示波器组</param>
		///<param name="controlHwnd">绘图区句柄</param>
		///<param name="args">参数</param>
		///<param name="graphProperties">图形参数数组</param>
		///<param name="size">数组大小</param>
		[DllImport(DllFileName)]
		internal extern static IntPtr CreateOscilloscope(IntPtr oscilloscopes, IntPtr controlHwnd, IntPtr args, IntPtr graphProperties, uint size);

		///<summary>刷新DPI</summary>
		///<param name="oscilloscopes">示波器组</param>
		[DllImport(DllFileName)]
		internal extern static void RefreshDPI(IntPtr oscilloscopes);

		///<summary>水平缩放率【单线程调用】</summary>
		///<param name="oscilloscope">示波器</param>
		[DllImport(DllFileName)]
		internal extern static float ScaleX(IntPtr oscilloscope);

		///<summary>水平缩放率极限值【单线程调用】</summary>
		///<param name="oscilloscope">示波器</param>
		[DllImport(DllFileName)]
		internal extern static float ScaleX_Limit(IntPtr oscilloscope);

		///<summary>水平位移【单线程调用】</summary>
		///<param name="oscilloscope">示波器</param>
		[DllImport(DllFileName)]
		internal extern static int TranslateX(IntPtr oscilloscope);

		///<summary>水平位移极限值【单线程调用】</summary>
		///<param name="oscilloscope">示波器</param>
		[DllImport(DllFileName)]
		internal extern static int TranslateX_Limit(IntPtr oscilloscope);

		///<summary>垂直缩放率【单线程调用】</summary>
		///<param name="oscilloscope">示波器</param>
		[DllImport(DllFileName)]
		internal extern static float ScaleY(IntPtr oscilloscope);

		///<summary>垂直缩放率极限值【单线程调用】</summary>
		///<param name="oscilloscope">示波器</param>
		[DllImport(DllFileName)]
		internal extern static float ScaleY_Limit(IntPtr oscilloscope);

		///<summary>垂直位移【单线程调用】</summary>
		///<param name="oscilloscope">示波器</param>
		[DllImport(DllFileName)]
		internal extern static int TranslateY(IntPtr oscilloscope);

		///<summary>垂直位移极限值【单线程调用】</summary>
		///<param name="oscilloscope">示波器</param>
		[DllImport(DllFileName)]
		internal extern static int TranslateY_Limit(IntPtr oscilloscope);

		///<summary>改变缩放极限值【单线程调用】</summary>
		///<param name="oscilloscope">示波器</param>
		///<param name="value">值（>=1）</param>
		[DllImport(DllFileName)]
		internal extern static bool ChangeScaleLimit(IntPtr oscilloscope, uint value);

		///<summary>改变水平缩放极限值【单线程调用】</summary>
		///<param name="oscilloscope">示波器</param>
		///<param name="value">值（>=1）</param>
		[DllImport(DllFileName)]
		internal extern static bool ChangeScaleLimitX(IntPtr oscilloscope, uint value);

		///<summary>改变垂直缩放极限值【单线程调用】</summary>
		///<param name="oscilloscope">示波器</param>
		///<param name="value">值（>=1）</param>
		[DllImport(DllFileName)]
		internal extern static bool ChangeScaleLimitY(IntPtr oscilloscope, uint value);

		///<summary>改变缩放【单线程调用】</summary>
		///<param name="oscilloscope">示波器</param>
		///<param name="offset">差值</param>
		[DllImport(DllFileName)]
		internal extern static bool ChangeScale(IntPtr oscilloscope, float offset);

		///<summary>改变水平缩放【单线程调用】</summary>
		///<param name="oscilloscope">示波器</param>
		///<param name="offset">差值</param>
		[DllImport(DllFileName)]
		internal extern static bool ChangeScaleX(IntPtr oscilloscope, float offset);

		///<summary>改变垂直缩放【单线程调用】</summary>
		///<param name="oscilloscope">示波器</param>
		///<param name="offset">差值</param>
		[DllImport(DllFileName)]
		internal extern static bool ChangeScaleY(IntPtr oscilloscope, float offset);

		///<summary>改变平移【单线程调用】</summary>
		///<param name="oscilloscope">示波器</param>
		///<param name="offsetX">水平位移值</param>
		///<param name="offsetY">垂直位移值</param>
		[DllImport(DllFileName)]
		internal extern static bool ChangeTranslate(IntPtr oscilloscope, int offsetX, int offsetY);

		///<summary>改变水平平移【单线程调用】</summary>
		///<param name="oscilloscope">示波器</param>
		///<param name="offset">位移值</param>
		[DllImport(DllFileName)]
		internal extern static bool ChangeTranslateX(IntPtr oscilloscope, int offset);

		///<summary>改变垂直平移【单线程调用】</summary>
		///<param name="oscilloscope">示波器</param>
		///<param name="offset">位移值</param>
		[DllImport(DllFileName)]
		internal extern static bool ChangeTranslateY(IntPtr oscilloscope, int offset);

		///<summary>改变示波器刷新模式【单线程调用】</summary>
		///<param name="oscilloscope">示波器</param>
		///<param name="refreshMode">刷新模式</param>
		[DllImport(DllFileName)]
		internal extern static void ChangeRefreshMode(IntPtr oscilloscope, uint refreshMode);

		///<summary>改变基线类型【单线程调用】</summary>
		///<param name="oscilloscope">示波器</param>
		///<param name="graphIndex">图形序号</param>
		///<param name="baselineType">基线类型</param>
		[DllImport(DllFileName)]
		internal extern static void ChangeBaselineType(IntPtr oscilloscope, uint graphIndex, uint baselineType);

		///<summary>改变示波器图形样式【单线程调用】</summary>
		///<param name="oscilloscope">示波器</param>
		///<param name="graphStyle">图形样式数组</param>
		[DllImport(DllFileName)]
		internal extern static void ChangeGraphStyles(IntPtr oscilloscope, IntPtr graphStyle);

		///<summary>改变示波器图形样式【单线程调用】</summary>
		///<param name="oscilloscope">示波器</param>
		///<param name="graphIndex">图形序号</param>
		///<param name="graphStyle">图形样式</param>
		[DllImport(DllFileName)]
		internal extern static void ChangeGraphStyle(IntPtr oscilloscope, uint graphIndex, uint graphStyle);

		///<summary>改变坐标轴线宽【单线程调用】</summary>
		///<param name="oscilloscope">示波器</param>
		///<param name="axisLineWidth">坐标轴线宽</param>
		[DllImport(DllFileName)]
		internal extern static void ChangeAxisLineWidth(IntPtr oscilloscope, uint axisLineWidth);

		///<summary>改变坐标轴颜色【单线程调用】</summary>
		///<param name="oscilloscope">示波器</param>
		///<param name="axisColor">坐标轴颜色</param>
		[DllImport(DllFileName)]
		internal extern static void ChangeAxisColor(IntPtr oscilloscope, uint axisColor);

		///<summary>改变示波器背景色【单线程调用】</summary>
		///<param name="oscilloscope">示波器</param>
		///<param name="background">背景色</param>
		[DllImport(DllFileName)]
		internal extern static void ChangeBackgroundColor(IntPtr oscilloscope, uint background);

		///<summary>改变示波器前景色【单线程调用】</summary>
		///<param name="oscilloscope">示波器</param>
		///<param name="foreground">前景色数组</param>
		[DllImport(DllFileName)]
		internal extern static void ChangeForegroundColors(IntPtr oscilloscope, IntPtr foreground);

		///<summary>改变示波器前景色【单线程调用】</summary>
		///<param name="oscilloscope">示波器</param>
		///<param name="graphIndex">图形序号</param>
		///<param name="foreground">前景色</param>
		[DllImport(DllFileName)]
		internal extern static void ChangeForegroundColor(IntPtr oscilloscope, uint graphIndex, uint foreground);

		///<summary>改变线宽【单线程调用】</summary>
		///<param name="oscilloscope">示波器</param>
		///<param name="graphIndex">图形序号</param>
		///<param name="lineWidth">线宽</param>
		[DllImport(DllFileName)]
		internal extern static void ChangeLineWidth(IntPtr oscilloscope, uint graphIndex, byte lineWidth);

		///<summary>改变基线偏移值【单线程调用】</summary>
		///<param name="oscilloscope">示波器</param>
		///<param name="graphIndex">图形序号</param>
		///<param name="offset">基线偏移值</param>
		[DllImport(DllFileName)]
		internal extern static void ChangeOffset(IntPtr oscilloscope, uint graphIndex, int offset);

		///<summary>改变是否自动归零【单线程调用】</summary>
		///<param name="oscilloscope">示波器</param>
		///<param name="graphIndex">图形序号</param>
		///<param name="isAutoTranslate">是否自动归零</param>
		[DllImport(DllFileName)]
		internal extern static void ChangeIsAutoTranslate(IntPtr oscilloscope, uint graphIndex, bool isAutoTranslate);

		///<summary>改变自动归零【单线程调用】</summary>
		///<param name="oscilloscope">示波器</param>
		///<param name="graphIndex">图形序号</param>
		///<param name="autoTranslateOffset">自动归零偏移值（如果是非数值则不使用此功能，否则将最接近横轴的数值归零后加上此值。）</param>
		[DllImport(DllFileName)]
		internal extern static void ChangeAutoTranslate(IntPtr oscilloscope, uint graphIndex, float autoTranslateOffset);

		///<summary>示波器图形是否可用【单线程调用】</summary>
		///<param name="oscilloscope">示波器</param>
		///<param name="graphIndex">图形序号</param>
		///<param name="isEnabled">是否可用</param>
		[DllImport(DllFileName)]
		internal extern static void ChangeIsEnabled(IntPtr oscilloscope, uint graphIndex, bool isEnabled);

		///<summary>安全存入数据（并行安全）</summary>
		///<param name="oscilloscope">示波器</param>
		///<param name="graphIndex">图形序号</param>
		///<param name="value">值</param>
		[DllImport(DllFileName)]
		internal extern static void SafePush(IntPtr oscilloscope, uint graphIndex, float value);

		///<summary>存入数据</summary>
		///<param name="oscilloscope">示波器</param>
		///<param name="graphIndex">图形序号</param>
		///<param name="value">值</param>
		[DllImport(DllFileName)]
		internal extern static void Push(IntPtr oscilloscope, uint graphIndex, float value);

		///<summary>安全替换全部数据（并行安全）</summary>
		///<param name="oscilloscope">示波器</param>
		///<param name="graphIndex">图形序号</param>
		///<param name="value">值</param>
		///<param name="length">值数量</param>
		[DllImport(DllFileName)]
		internal extern static void SafeReplace(IntPtr oscilloscope, uint graphIndex, IntPtr value, uint length);

		///<summary>替换全部数据</summary>
		///<param name="oscilloscope">示波器</param>
		///<param name="graphIndex">图形序号</param>
		///<param name="value">值</param>
		///<param name="length">值数量</param>
		[DllImport(DllFileName)]
		internal extern static void Replace(IntPtr oscilloscope, uint graphIndex, IntPtr value, uint length);

		///<summary>安全清空（并行安全）</summary>
		///<param name="oscilloscope">示波器</param>
		[DllImport(DllFileName)]
		internal extern static void SafeClear(IntPtr oscilloscope);

		///<summary>清空</summary>
		///<param name="oscilloscope">示波器</param>
		[DllImport(DllFileName)]
		internal extern static void Clear(IntPtr oscilloscope);

		///<summary>改变示波器尺寸</summary>
		///<param name="oscilloscope">示波器</param>
		[DllImport(DllFileName)]
		internal extern static void Resize(IntPtr oscilloscope);

		///<summary>刷新示波器</summary>
		///<param name="oscilloscope">示波器</param>
		[DllImport(DllFileName)]
		internal extern static void Refresh(IntPtr oscilloscope);

	}

}