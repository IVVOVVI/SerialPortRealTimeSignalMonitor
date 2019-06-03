using System;

namespace Oscilloscope
{

	unsafe public static class Dll
	{

		private static readonly bool s_IsX64 = Environment.Is64BitProcess;

		///<summary>默认缩放率极限值</summary>
		public const uint DefaultScaleLimit = 10u;

		///<summary>创建示波器组</summary>
		///<param name="windowHwnd">窗口句柄</param>
		///<param name="fps">帧率（最小值：30）</param>
		public static IntPtr CreateOscilloscopes(IntPtr windowHwnd, byte fps)
		{
			if (s_IsX64)
			{
				return Dll64.CreateOscilloscopes(windowHwnd, fps);
			}
			else
			{
				return Dll86.CreateOscilloscopes(windowHwnd, fps);
			}
		}

		///<summary>关闭示波器组</summary>
		///<param name="ptr">由Initialize返回的指针</param>
		public static void CloseOscilloscopes(IntPtr oscilloscopes)
		{
			if (s_IsX64)
			{
				Dll64.CloseOscilloscopes(oscilloscopes);
			}
			else
			{
				Dll86.CloseOscilloscopes(oscilloscopes);
			}
		}

		///<summary>创建示波器（多图形）</summary>
		///<param name="arg">参数</param>
		public static IntPtr CreateOscilloscope(ref ConstructionParameters arg)
		{
			var length = (uint)arg.GraphProperties.Length;
			if (length == 0u) throw new ArgumentException();
			fixed (ConstructionArgs* args = &arg.Args)
			{
				fixed (GraphProperties* array = arg.GraphProperties)
				{
					if (s_IsX64)
					{
						return Dll64.CreateOscilloscope(arg.Oscilloscopes, arg.ControlHwnd, new IntPtr(args), new IntPtr(array), length);
					}
					else
					{
						return Dll86.CreateOscilloscope(arg.Oscilloscopes, arg.ControlHwnd, new IntPtr(args), new IntPtr(array), length);
					}
				}
			}
		}

		///<summary>刷新DPI</summary>
		///<param name="oscilloscopes">示波器组</param>
		public static void RefreshDPI(IntPtr oscilloscopes)
		{
			if (s_IsX64)
			{
				Dll64.RefreshDPI(oscilloscopes);
			}
			else
			{
				Dll86.RefreshDPI(oscilloscopes);
			}
		}

		///<summary>水平缩放率【单线程调用】</summary>
		///<param name="oscilloscope">示波器</param>
		public static float ScaleX(IntPtr oscilloscope)
		{
			if (s_IsX64)
			{
				return Dll64.ScaleX(oscilloscope);
			}
			else
			{
				return Dll86.ScaleX(oscilloscope);
			}
		}

		///<summary>水平缩放率极限值【单线程调用】</summary>
		///<param name="oscilloscope">示波器</param>
		public static float ScaleX_Limit(IntPtr oscilloscope)
		{
			if (s_IsX64)
			{
				return Dll64.ScaleX_Limit(oscilloscope);
			}
			else
			{
				return Dll86.ScaleX_Limit(oscilloscope);
			}
		}

		///<summary>水平位移【单线程调用】</summary>
		///<param name="oscilloscope">示波器</param>
		public static int TranslateX(IntPtr oscilloscope)
		{
			if (s_IsX64)
			{
				return Dll64.TranslateX(oscilloscope);
			}
			else
			{
				return Dll86.TranslateX(oscilloscope);
			}
		}

		///<summary>水平位移极限值【单线程调用】</summary>
		///<param name="oscilloscope">示波器</param>
		public static int TranslateX_Limit(IntPtr oscilloscope)
		{
			if (s_IsX64)
			{
				return Dll64.TranslateX_Limit(oscilloscope);
			}
			else
			{
				return Dll86.TranslateX_Limit(oscilloscope);
			}
		}

		///<summary>垂直缩放率【单线程调用】</summary>
		///<param name="oscilloscope">示波器</param>
		public static float ScaleY(IntPtr oscilloscope)
		{
			if (s_IsX64)
			{
				return Dll64.ScaleY(oscilloscope);
			}
			else
			{
				return Dll86.ScaleY(oscilloscope);
			}
		}

		///<summary>垂直缩放率极限值【单线程调用】</summary>
		///<param name="oscilloscope">示波器</param>
		public static float ScaleY_Limit(IntPtr oscilloscope)
		{
			if (s_IsX64)
			{
				return Dll64.ScaleY_Limit(oscilloscope);
			}
			else
			{
				return Dll86.ScaleY_Limit(oscilloscope);
			}
		}

		///<summary>垂直位移【单线程调用】</summary>
		///<param name="oscilloscope">示波器</param>
		public static int TranslateY(IntPtr oscilloscope)
		{
			if (s_IsX64)
			{
				return Dll64.TranslateY(oscilloscope);
			}
			else
			{
				return Dll86.TranslateY(oscilloscope);
			}
		}

		///<summary>垂直位移极限值【单线程调用】</summary>
		///<param name="oscilloscope">示波器</param>
		public static int TranslateY_Limit(IntPtr oscilloscope)
		{
			if (s_IsX64)
			{
				return Dll64.TranslateY_Limit(oscilloscope);
			}
			else
			{
				return Dll86.TranslateY_Limit(oscilloscope);
			}
		}

		///<summary>改变缩放极限值【单线程调用】</summary>
		///<param name="oscilloscope">示波器</param>
		///<param name="value">值（>=1）</param>
		public static bool ChangeScaleLimit(IntPtr oscilloscope, uint value)
		{
			if (s_IsX64)
			{
				return Dll64.ChangeScaleLimit(oscilloscope, value);
			}
			else
			{
				return Dll86.ChangeScaleLimit(oscilloscope, value);
			}
		}

		///<summary>改变水平缩放极限值【单线程调用】</summary>
		///<param name="oscilloscope">示波器</param>
		///<param name="value">值（>=1）</param>
		public static bool ChangeScaleLimitX(IntPtr oscilloscope, uint value)
		{
			if (s_IsX64)
			{
				return Dll64.ChangeScaleLimitX(oscilloscope, value);
			}
			else
			{
				return Dll86.ChangeScaleLimitX(oscilloscope, value);
			}
		}

		///<summary>改变垂直缩放极限值【单线程调用】</summary>
		///<param name="oscilloscope">示波器</param>
		///<param name="value">值（>=1）</param>
		public static bool ChangeScaleLimitY(IntPtr oscilloscope, uint value)
		{
			if (s_IsX64)
			{
				return Dll64.ChangeScaleLimitY(oscilloscope, value);
			}
			else
			{
				return Dll86.ChangeScaleLimitY(oscilloscope, value);
			}
		}

		///<summary>改变缩放【单线程调用】</summary>
		///<param name="oscilloscope">示波器</param>
		///<param name="offset">差值</param>
		public static bool ChangeScale(IntPtr oscilloscope, float offset)
		{
			if (s_IsX64)
			{
				return Dll64.ChangeScale(oscilloscope, offset);
			}
			else
			{
				return Dll86.ChangeScale(oscilloscope, offset);
			}
		}

		///<summary>改变水平缩放【单线程调用】</summary>
		///<param name="oscilloscope">示波器</param>
		///<param name="offset">差值</param>
		public static bool ChangeScaleX(IntPtr oscilloscope, float offset)
		{
			if (s_IsX64)
			{
				return Dll64.ChangeScaleX(oscilloscope, offset);
			}
			else
			{
				return Dll86.ChangeScaleX(oscilloscope, offset);
			}
		}

		///<summary>改变垂直缩放【单线程调用】</summary>
		///<param name="oscilloscope">示波器</param>
		///<param name="offset">差值</param>
		public static bool ChangeScaleY(IntPtr oscilloscope, float offset)
		{
			if (s_IsX64)
			{
				return Dll64.ChangeScaleY(oscilloscope, offset);
			}
			else
			{
				return Dll86.ChangeScaleY(oscilloscope, offset);
			}
		}

		///<summary>改变平移【单线程调用】</summary>
		///<param name="oscilloscope">示波器</param>
		///<param name="offsetX">水平位移值</param>
		///<param name="offsetY">垂直位移值</param>
		public static bool ChangeTranslate(IntPtr oscilloscope, int offsetX, int offsetY)
		{
			if (s_IsX64)
			{
				return Dll64.ChangeTranslate(oscilloscope, offsetX, offsetY);
			}
			else
			{
				return Dll86.ChangeTranslate(oscilloscope, offsetX, offsetY);
			}
		}

		///<summary>改变水平平移【单线程调用】</summary>
		///<param name="oscilloscope">示波器</param>
		///<param name="offset">位移值</param>
		public static bool ChangeTranslateX(IntPtr oscilloscope, int offset)
		{
			if (s_IsX64)
			{
				return Dll64.ChangeTranslateX(oscilloscope, offset);
			}
			else
			{
				return Dll86.ChangeTranslateX(oscilloscope, offset);
			}
		}

		///<summary>改变垂直平移【单线程调用】</summary>
		///<param name="oscilloscope">示波器</param>
		///<param name="offset">位移值</param>
		public static bool ChangeTranslateY(IntPtr oscilloscope, int offset)
		{
			if (s_IsX64)
			{
				return Dll64.ChangeTranslateY(oscilloscope, offset);
			}
			else
			{
				return Dll86.ChangeTranslateY(oscilloscope, offset);
			}
		}

		///<summary>改变示波器刷新模式【单线程调用】</summary>
		///<param name="oscilloscope">示波器</param>
		///<param name="refreshMode">刷新模式</param>
		public static void ChangeRefreshMode(IntPtr oscilloscope, Enum_RefreshMode refreshMode)
		{
			if (s_IsX64)
			{
				Dll64.ChangeRefreshMode(oscilloscope, (uint)refreshMode);
			}
			else
			{
				Dll86.ChangeRefreshMode(oscilloscope, (uint)refreshMode);
			}
		}

		///<summary>改变基线类型【单线程调用】</summary>
		///<param name="oscilloscope">示波器</param>
		///<param name="graphIndex">图形序号</param>
		///<param name="baselineType">基线类型</param>
		public static void ChangeBaselineType(IntPtr oscilloscope, uint graphIndex, Enum_BaselineType baselineType)
		{
			if (s_IsX64)
			{
				Dll64.ChangeBaselineType(oscilloscope, graphIndex, (uint)baselineType);
			}
			else
			{
				Dll86.ChangeBaselineType(oscilloscope, graphIndex, (uint)baselineType);
			}
		}

		///<summary>改变示波器图形样式【单线程调用】</summary>
		///<param name="oscilloscope">示波器</param>
		///<param name="graphStyle">图形样式数组</param>
		public static void ChangeGraphStyle(IntPtr oscilloscope, Enum_GraphStyle[] graphStyle)
		{
			fixed (Enum_GraphStyle* array = graphStyle)
			{
				if (s_IsX64)
				{
					Dll64.ChangeGraphStyles(oscilloscope, new IntPtr(array));
				}
				else
				{
					Dll86.ChangeGraphStyles(oscilloscope, new IntPtr(array));
				}
			}
		}

		///<summary>改变示波器图形样式【单线程调用】</summary>
		///<param name="oscilloscope">示波器</param>
		///<param name="graphIndex">图形序号</param>
		///<param name="graphStyle">图形样式</param>
		public static void ChangeGraphStyle(IntPtr oscilloscope, uint graphIndex, Enum_GraphStyle graphStyle)
		{
			if (s_IsX64)
			{
				Dll64.ChangeGraphStyle(oscilloscope, graphIndex, (uint)graphStyle);
			}
			else
			{
				Dll86.ChangeGraphStyle(oscilloscope, graphIndex, (uint)graphStyle);
			}
		}

		///<summary>改变坐标轴线宽【单线程调用】</summary>
		///<param name="oscilloscope">示波器</param>
		///<param name="axisLineWidth">坐标轴线宽</param>
		public static void ChangeAxisLineWidth(IntPtr oscilloscope, uint axisLineWidth)
		{
			if (s_IsX64)
			{
				Dll64.ChangeAxisLineWidth(oscilloscope, axisLineWidth);
			}
			else
			{
				Dll86.ChangeAxisLineWidth(oscilloscope, axisLineWidth);
			}
		}

		///<summary>改变坐标轴颜色【单线程调用】</summary>
		///<param name="oscilloscope">示波器</param>
		///<param name="axisColor">坐标轴颜色</param>
		public static void ChangeAxisColor(IntPtr oscilloscope, uint axisColor)
		{
			if (s_IsX64)
			{
				Dll64.ChangeAxisColor(oscilloscope, axisColor);
			}
			else
			{
				Dll86.ChangeAxisColor(oscilloscope, axisColor);
			}
		}

		///<summary>改变示波器背景色【单线程调用】</summary>
		///<param name="oscilloscope">示波器</param>
		///<param name="background">背景色</param>
		public static void ChangeBackgroundColor(IntPtr oscilloscope, uint background)
		{
			if (s_IsX64)
			{
				Dll64.ChangeBackgroundColor(oscilloscope, background);
			}
			else
			{
				Dll86.ChangeBackgroundColor(oscilloscope, background);
			}
		}

		///<summary>改变示波器前景色【单线程调用】</summary>
		///<param name="oscilloscope">示波器</param>
		///<param name="foreground">前景色数组</param>
		public static void ChangeForegroundColor(IntPtr oscilloscope, uint[] foreground)
		{
			fixed (uint* array = foreground)
			{
				if (s_IsX64)
				{
					Dll64.ChangeForegroundColors(oscilloscope, new IntPtr(array));
				}
				else
				{
					Dll86.ChangeForegroundColors(oscilloscope, new IntPtr(array));
				}
			}
		}

		///<summary>改变示波器前景色【单线程调用】</summary>
		///<param name="oscilloscope">示波器</param>
		///<param name="graphIndex">图形序号</param>
		///<param name="foreground">前景色</param>
		public static void ChangeForegroundColor(IntPtr oscilloscope, uint graphIndex, uint foreground)
		{
			if (s_IsX64)
			{
				Dll64.ChangeForegroundColor(oscilloscope, graphIndex, foreground);
			}
			else
			{
				Dll86.ChangeForegroundColor(oscilloscope, graphIndex, foreground);
			}
		}

		///<summary>改变线宽【单线程调用】</summary>
		///<param name="oscilloscope">示波器</param>
		///<param name="graphIndex">图形序号</param>
		///<param name="lineWidth">线宽</param>
		public static void ChangeLineWidth(IntPtr oscilloscope, uint graphIndex, byte lineWidth)
		{
			if (s_IsX64)
			{
				Dll64.ChangeLineWidth(oscilloscope, graphIndex, lineWidth);
			}
			else
			{
				Dll86.ChangeLineWidth(oscilloscope, graphIndex, lineWidth);
			}
		}

		///<summary>改变基线偏移值【单线程调用】</summary>
		///<param name="oscilloscope">示波器</param>
		///<param name="graphIndex">图形序号</param>
		///<param name="offset">基线偏移值</param>
		public static void ChangeOffset(IntPtr oscilloscope, uint graphIndex, int offset)
		{
			if (s_IsX64)
			{
				Dll64.ChangeOffset(oscilloscope, graphIndex, offset);
			}
			else
			{
				Dll86.ChangeOffset(oscilloscope, graphIndex, offset);
			}
		}

		///<summary>改变是否自动归零【单线程调用】</summary>
		///<param name="oscilloscope">示波器</param>
		///<param name="graphIndex">图形序号</param>
		///<param name="isAutoTranslate">是否自动归零</param>
		public static void ChangeIsAutoTranslate(IntPtr oscilloscope, uint graphIndex, bool isAutoTranslate)
		{
			if (s_IsX64)
			{
				Dll64.ChangeIsAutoTranslate(oscilloscope, graphIndex, isAutoTranslate);
			}
			else
			{
				Dll86.ChangeIsAutoTranslate(oscilloscope, graphIndex, isAutoTranslate);
			}
		}

		///<summary>改变自动归零【单线程调用】</summary>
		///<param name="oscilloscope">示波器</param>
		///<param name="graphIndex">图形序号</param>
		///<param name="autoTranslateOffset">自动归零偏移值（如果是非数值则不使用此功能，否则将最接近横轴的数值归零后加上此值。）</param>
		public static void ChangeAutoTranslate(IntPtr oscilloscope, uint graphIndex, float autoTranslateOffset)
		{
			if (s_IsX64)
			{
				Dll64.ChangeAutoTranslate(oscilloscope, graphIndex, autoTranslateOffset);
			}
			else
			{
				Dll86.ChangeAutoTranslate(oscilloscope, graphIndex, autoTranslateOffset);
			}
		}

		///<summary>示波器图形是否可用【单线程调用】</summary>
		///<param name="oscilloscope">示波器</param>
		///<param name="graphIndex">图形序号</param>
		///<param name="isEnabled">是否可用</param>
		public static void ChangeIsEnabled(IntPtr oscilloscope, uint graphIndex, bool isEnabled)
		{
			if (s_IsX64)
			{
				Dll64.ChangeIsEnabled(oscilloscope, graphIndex, isEnabled);
			}
			else
			{
				Dll86.ChangeIsEnabled(oscilloscope, graphIndex, isEnabled);
			}
		}

		///<summary>安全存入数据（并行安全）</summary>
		///<param name="oscilloscope">示波器</param>
		///<param name="graphIndex">图形序号</param>
		///<param name="value">值</param>
		public static void SafePush(IntPtr oscilloscope, uint graphIndex, float value)
		{
			if (s_IsX64)
			{
				Dll64.SafePush(oscilloscope, graphIndex, value);
			}
			else
			{
				Dll86.SafePush(oscilloscope, graphIndex, value);
			}
		}

		///<summary>存入数据</summary>
		///<param name="oscilloscope">示波器</param>
		///<param name="graphIndex">图形序号</param>
		///<param name="value">值</param>
		public static void Push(IntPtr oscilloscope, uint graphIndex, float value)
		{
			if (s_IsX64)
			{
				Dll64.Push(oscilloscope, graphIndex, value);
			}
			else
			{
				Dll86.Push(oscilloscope, graphIndex, value);
			}
		}

		///<summary>安全替换全部数据（并行安全）</summary>
		///<param name="oscilloscope">示波器</param>
		///<param name="graphIndex">图形序号</param>
		///<param name="value">值</param>
		public static void SafeReplace(IntPtr oscilloscope, uint graphIndex, float[] value)
		{
			var length = (uint)value.Length;
			if (length == 0u) throw new ArgumentException();
			fixed (float* array = value)
			{
				if (s_IsX64)
				{
					Dll64.SafeReplace(oscilloscope, graphIndex, new IntPtr(array), length);
				}
				else
				{
					Dll86.SafeReplace(oscilloscope, graphIndex, new IntPtr(array), length);
				}
			}
		}

		///<summary>替换全部数据</summary>
		///<param name="oscilloscope">示波器</param>
		///<param name="graphIndex">图形序号</param>
		///<param name="value">值</param>
		public static void Replace(IntPtr oscilloscope, uint graphIndex, float[] value)
		{
			var length = (uint)value.Length;
			if (length == 0u) throw new ArgumentException();
			fixed (float* array = value)
			{
				if (s_IsX64)
				{
					Dll64.Replace(oscilloscope, graphIndex, new IntPtr(array), length);
				}
				else
				{
					Dll86.Replace(oscilloscope, graphIndex, new IntPtr(array), length);
				}
			}
		}

		///<summary>安全清空（并行安全）</summary>
		///<param name="oscilloscope">示波器</param>
		public static void SafeClear(IntPtr oscilloscope)
		{
			if (s_IsX64)
			{
				Dll64.SafeClear(oscilloscope);
			}
			else
			{
				Dll86.SafeClear(oscilloscope);
			}
		}

		///<summary>清空</summary>
		///<param name="oscilloscope">示波器</param>
		public static void Clear(IntPtr oscilloscope)
		{
			if (s_IsX64)
			{
				Dll64.Clear(oscilloscope);
			}
			else
			{
				Dll86.Clear(oscilloscope);
			}
		}

		///<summary>改变示波器尺寸</summary>
		///<param name="oscilloscope">示波器</param>
		public static void Resize(IntPtr oscilloscope)
		{
			if (s_IsX64)
			{
				Dll64.Resize(oscilloscope);
			}
			else
			{
				Dll86.Resize(oscilloscope);
			}
		}

		///<summary>刷新示波器</summary>
		///<param name="oscilloscope">示波器</param>
		public static void Refresh(IntPtr oscilloscope)
		{
			if (s_IsX64)
			{
				Dll64.Refresh(oscilloscope);
			}
			else
			{
				Dll86.Refresh(oscilloscope);
			}
		}

	}

}