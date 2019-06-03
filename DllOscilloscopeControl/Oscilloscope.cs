using System;
using System.Windows.Forms;

namespace Oscilloscope
{

	///<summary>示波器控件</summary>
	public class Oscilloscope : Control
	{

		///<summary>属性</summary>
		public enum Property : uint { ScaleX, ScaleX_Limit, ScaleY, ScaleY_Limit, TranslateX, TranslateX_Limit, TranslateY, TranslateY_Limit };

		///<summary>属性改变委托</summary>
		public delegate void PropertyChangedEventHandler(Property property, float value);

		///<summary>属性改变事件</summary>
		public event PropertyChangedEventHandler PropertyChanged;

		///<summary>示波器</summary>
		private IntPtr m_Oscilloscope = IntPtr.Zero;

		///<summary>光标水平位置</summary>
		private int m_PointerX = 0;

		///<summary>光标垂直位置</summary>
		private int m_PointerY = 0;

		///<summary>是否未初始化</summary>
		private bool m_IsUninitialized = true;

		///<summary>示波器</summary>
		public Oscilloscope()
		{
			SetStyle(ControlStyles.UserPaint | ControlStyles.AllPaintingInWmPaint | ControlStyles.Opaque | ControlStyles.UserMouse, true);
			SetStyle(ControlStyles.DoubleBuffer | ControlStyles.OptimizedDoubleBuffer | ControlStyles.ResizeRedraw, false);
			UpdateStyles();
		}

		///<summary>初始化</summary>
		///<param name="arg">参数</param>
		public void Initialize(ref ConstructionParameters arg)
		{
			if (arg != null && m_Oscilloscope == IntPtr.Zero && arg.Oscilloscopes != IntPtr.Zero)
			{
				arg.ControlHwnd = Handle;
				BackColor = ColorHelper.FromArgb(arg.Background);
				m_Oscilloscope = Dll.CreateOscilloscope(ref arg);
				m_IsUninitialized = m_Oscilloscope == IntPtr.Zero;
				if ((!m_IsUninitialized) && PropertyChanged != null)
				{
					PropertyChanged(Property.TranslateX_Limit, Dll.TranslateX_Limit(m_Oscilloscope));
					PropertyChanged(Property.TranslateY_Limit, Dll.TranslateY_Limit(m_Oscilloscope));
					PropertyChanged(Property.TranslateX, Dll.TranslateX(m_Oscilloscope));
					PropertyChanged(Property.TranslateY, Dll.TranslateY(m_Oscilloscope));
					PropertyChanged(Property.ScaleX, Dll.ScaleX(m_Oscilloscope));
					PropertyChanged(Property.ScaleY, Dll.ScaleY(m_Oscilloscope));
					PropertyChanged(Property.ScaleX_Limit, Dll.ScaleX_Limit(m_Oscilloscope));
					PropertyChanged(Property.ScaleY_Limit, Dll.ScaleY_Limit(m_Oscilloscope));
				}
			}
		}

		///<summary>水平缩放率</summary>
		public float ScaleX
		{
			get
			{
				return m_Oscilloscope == IntPtr.Zero ? 1.0f : Dll.ScaleX(m_Oscilloscope);
			}
		}

		///<summary>水平缩放率极限值</summary>
		public float ScaleX_Limit
		{
			get
			{
				return m_Oscilloscope == IntPtr.Zero ? 1.0f : Dll.ScaleX_Limit(m_Oscilloscope);
			}
		}

		///<summary>水平位移</summary>
		public int TranslateX
		{
			get
			{
				return m_Oscilloscope == IntPtr.Zero ? 0 : Dll.TranslateX(m_Oscilloscope);
			}
		}

		///<summary>水平位移极限值</summary>
		public int TranslateX_Limit
		{
			get
			{
				return m_Oscilloscope == IntPtr.Zero ? 0 : Dll.TranslateX_Limit(m_Oscilloscope);
			}
		}

		///<summary>垂直缩放率</summary>
		public float ScaleY
		{
			get
			{
				return m_Oscilloscope == IntPtr.Zero ? 1.0f : Dll.ScaleY(m_Oscilloscope);
			}
		}

		///<summary>垂直缩放率极限值</summary>
		public float ScaleY_Limit
		{
			get
			{
				return m_Oscilloscope == IntPtr.Zero ? 1.0f : Dll.ScaleY_Limit(m_Oscilloscope);
			}
		}

		///<summary>垂直位移</summary>
		public int TranslateY
		{
			get
			{
				return m_Oscilloscope == IntPtr.Zero ? 0 : Dll.TranslateY(m_Oscilloscope);
			}
		}

		///<summary>垂直位移极限值</summary>
		public int TranslateY_Limit
		{
			get
			{
				return m_Oscilloscope == IntPtr.Zero ? 0 : Dll.TranslateY_Limit(m_Oscilloscope);
			}
		}

		///<summary>改变缩放极限值</summary>
		///<param name="value">值（>=1）</param>
		public void ChangeScaleLimit(uint value)
		{
			if (m_Oscilloscope != IntPtr.Zero && Dll.ChangeScaleLimit(m_Oscilloscope, value) && PropertyChanged != null)
			{
				PropertyChanged(Property.TranslateX_Limit, Dll.TranslateX_Limit(m_Oscilloscope));
				PropertyChanged(Property.TranslateY_Limit, Dll.TranslateY_Limit(m_Oscilloscope));
				PropertyChanged(Property.TranslateX, Dll.TranslateX(m_Oscilloscope));
				PropertyChanged(Property.TranslateY, Dll.TranslateY(m_Oscilloscope));
				PropertyChanged(Property.ScaleX, Dll.ScaleX(m_Oscilloscope));
				PropertyChanged(Property.ScaleY, Dll.ScaleY(m_Oscilloscope));
				PropertyChanged(Property.ScaleX_Limit, Dll.ScaleX_Limit(m_Oscilloscope));
				PropertyChanged(Property.ScaleY_Limit, Dll.ScaleY_Limit(m_Oscilloscope));
			}
		}

		///<summary>改变水平缩放极限值</summary>
		///<param name="value">值（>=1）</param>
		public void ChangeScaleLimitX(uint value)
		{
			if (m_Oscilloscope != IntPtr.Zero && Dll.ChangeScaleLimitX(m_Oscilloscope, value) && PropertyChanged != null)
			{
				PropertyChanged(Property.TranslateX_Limit, Dll.TranslateX_Limit(m_Oscilloscope));
				PropertyChanged(Property.TranslateX, Dll.TranslateX(m_Oscilloscope));
				PropertyChanged(Property.ScaleX, Dll.ScaleX(m_Oscilloscope));
				PropertyChanged(Property.ScaleX_Limit, Dll.ScaleX_Limit(m_Oscilloscope));
			}
		}

		///<summary>改变垂直缩放极限值</summary>
		///<param name="value">值（>=1）</param>
		public void ChangeScaleLimitY(uint value)
		{
			if (m_Oscilloscope != IntPtr.Zero && Dll.ChangeScaleLimitY(m_Oscilloscope, value) && PropertyChanged != null)
			{
				PropertyChanged(Property.TranslateY_Limit, Dll.TranslateY_Limit(m_Oscilloscope));
				PropertyChanged(Property.TranslateY, Dll.TranslateY(m_Oscilloscope));
				PropertyChanged(Property.ScaleY, Dll.ScaleY(m_Oscilloscope));
				PropertyChanged(Property.ScaleY_Limit, Dll.ScaleY_Limit(m_Oscilloscope));
			}
		}

		///<summary>改变缩放</summary>
		///<param name="offset">差值</param>
		public void ChangeScale(float offset)
		{
			if (m_Oscilloscope != IntPtr.Zero && Dll.ChangeScale(m_Oscilloscope, offset) && PropertyChanged != null)
			{
				PropertyChanged(Property.TranslateX_Limit, Dll.TranslateX_Limit(m_Oscilloscope));
				PropertyChanged(Property.TranslateY_Limit, Dll.TranslateY_Limit(m_Oscilloscope));
				PropertyChanged(Property.TranslateX, Dll.TranslateX(m_Oscilloscope));
				PropertyChanged(Property.TranslateY, Dll.TranslateY(m_Oscilloscope));
				PropertyChanged(Property.ScaleX, Dll.ScaleX(m_Oscilloscope));
				PropertyChanged(Property.ScaleY, Dll.ScaleY(m_Oscilloscope));
			}
		}

		///<summary>改变水平缩放</summary>
		///<param name="offset">差值</param>
		public void ChangeScaleX(float offset)
		{
			if (m_Oscilloscope != IntPtr.Zero && Dll.ChangeScaleX(m_Oscilloscope, offset) && PropertyChanged != null)
			{
				PropertyChanged(Property.TranslateX_Limit, Dll.TranslateX_Limit(m_Oscilloscope));
				PropertyChanged(Property.TranslateX, Dll.TranslateX(m_Oscilloscope));
				PropertyChanged(Property.ScaleX, Dll.ScaleX(m_Oscilloscope));
			}
		}


		///<summary>改变垂直缩放</summary>
		///<param name="offset">差值</param>
		public void ChangeScaleY(float offset)
		{
			if (m_Oscilloscope != IntPtr.Zero && Dll.ChangeScaleY(m_Oscilloscope, offset) && PropertyChanged != null)
			{
				PropertyChanged(Property.TranslateY_Limit, Dll.TranslateY_Limit(m_Oscilloscope));
				PropertyChanged(Property.TranslateY, Dll.TranslateY(m_Oscilloscope));
				PropertyChanged(Property.ScaleY, Dll.ScaleY(m_Oscilloscope));
			}
		}

		///<summary>改变平移</summary>
		///<param name="offsetX">水平位移值</param>
		///<param name="offsetY">垂直位移值</param>
		public void ChangeTranslate(int offsetX, int offsetY)
		{
			if (m_Oscilloscope != IntPtr.Zero && Dll.ChangeTranslate(m_Oscilloscope, offsetX, offsetY) && PropertyChanged != null)
			{
				PropertyChanged(Property.TranslateX, Dll.TranslateX(m_Oscilloscope));
				PropertyChanged(Property.TranslateY, Dll.TranslateY(m_Oscilloscope));
			}
		}

		///<summary>改变水平平移</summary>
		///<param name="offset">位移值</param>
		public void ChangeTranslateX(int offset)
		{
			if (m_Oscilloscope != IntPtr.Zero && Dll.ChangeTranslateX(m_Oscilloscope, offset) && PropertyChanged != null)
			{
				PropertyChanged(Property.TranslateX, Dll.TranslateX(m_Oscilloscope));
			}
		}

		///<summary>改变垂直平移</summary>
		///<param name="offset">位移值</param>
		public void ChangeTranslateY(int offset)
		{
			if (m_Oscilloscope != IntPtr.Zero && Dll.ChangeTranslateY(m_Oscilloscope, offset) && PropertyChanged != null)
			{
				PropertyChanged(Property.TranslateY, Dll.TranslateY(m_Oscilloscope));
			}
		}

		///<summary>改变示波器刷新模式</summary>
		///<param name="refreshMode">刷新模式</param>
		public void ChangeRefreshMode(Enum_RefreshMode refreshMode)
		{
			if (m_Oscilloscope != IntPtr.Zero) Dll.ChangeRefreshMode(m_Oscilloscope, refreshMode);
		}

		///<summary>改变基线类型</summary>
		///<param name="baselineType">基线类型</param>
		public void ChangeBaselineType(Enum_BaselineType baselineType)
		{
			if (m_Oscilloscope != IntPtr.Zero) Dll.ChangeBaselineType(m_Oscilloscope, 0u, baselineType);
		}

		///<summary>改变基线类型</summary>
		///<param name="index">图形序号</param>
		///<param name="baselineType">基线类型</param>
		public void ChangeBaselineType(uint index, Enum_BaselineType baselineType)
		{
			if (m_Oscilloscope != IntPtr.Zero) Dll.ChangeBaselineType(m_Oscilloscope, index, baselineType);
		}

		///<summary>改变示波器图形样式</summary>
		///<param name="graphStyle">图形样式数组</param>
		public void ChangeGraphStyle(Enum_GraphStyle[] graphStyle)
		{
			if (m_Oscilloscope != IntPtr.Zero) Dll.ChangeGraphStyle(m_Oscilloscope, graphStyle);
		}

		///<summary>改变示波器图形样式</summary>
		///<param name="index">图形序号</param>
		///<param name="graphStyle">图形样式</param>
		public void ChangeGraphStyle(uint index, Enum_GraphStyle graphStyle)
		{
			if (m_Oscilloscope != IntPtr.Zero) Dll.ChangeGraphStyle(m_Oscilloscope, index, graphStyle);
		}

		///<summary>改变示波器图形样式</summary>
		///<param name="graphStyle">图形样式</param>
		public void ChangeGraphStyle(Enum_GraphStyle graphStyle)
		{
			if (m_Oscilloscope != IntPtr.Zero) Dll.ChangeGraphStyle(m_Oscilloscope, 0u, graphStyle);
		}

		///<summary>改变坐标轴线宽</summary>
		///<param name="axisLineWidth">坐标轴线宽</param>
		public void ChangeAxisLineWidth(uint axisLineWidth)
		{
			if (m_Oscilloscope != IntPtr.Zero) Dll.ChangeAxisLineWidth(m_Oscilloscope, axisLineWidth);
		}

		///<summary>改变坐标轴颜色</summary>
		///<param name="axisColor">坐标轴颜色</param>
		public void ChangeAxisColor(uint axisColor)
		{
			if (m_Oscilloscope != IntPtr.Zero) Dll.ChangeAxisColor(m_Oscilloscope, axisColor);
		}

		///<summary>改变示波器背景色</summary>
		///<param name="background">背景色</param>
		public void ChangeBackgroundColor(uint background)
		{
			BackColor = ColorHelper.FromArgb(background);
			if (m_Oscilloscope != IntPtr.Zero) Dll.ChangeBackgroundColor(m_Oscilloscope, background);
		}

		///<summary>改变示波器前景色</summary>
		///<param name="foreground">前景色数组</param>
		public void ChangeForegroundColor(uint[] foreground)
		{
			if (m_Oscilloscope != IntPtr.Zero) Dll.ChangeForegroundColor(m_Oscilloscope, foreground);
		}

		///<summary>改变示波器前景色</summary>
		///<param name="index">图形序号</param>
		///<param name="foreground">前景色</param>
		public void ChangeForegroundColor(uint index, uint foreground)
		{
			if (m_Oscilloscope != IntPtr.Zero) Dll.ChangeForegroundColor(m_Oscilloscope, index, foreground);
		}

		///<summary>改变示波器前景色</summary>
		///<param name="foreground">前景色</param>
		public void ChangeForegroundColor(uint foreground)
		{
			if (m_Oscilloscope != IntPtr.Zero) Dll.ChangeForegroundColor(m_Oscilloscope, 0u, foreground);
		}

		///<summary>改变线宽</summary>
		///<param name="index">图形序号</param>
		///<param name="lineWidth">线宽</param>
		public void ChangeLineWidth(uint index, byte lineWidth)
		{
			if (m_Oscilloscope != IntPtr.Zero) Dll.ChangeLineWidth(m_Oscilloscope, index, lineWidth);
		}

		///<summary>改变线宽</summary>
		///<param name="lineWidth">线宽</param>
		public void ChangeLineWidth(byte lineWidth)
		{
			if (m_Oscilloscope != IntPtr.Zero) Dll.ChangeLineWidth(m_Oscilloscope, 0u, lineWidth);
		}

		///<summary>改变基线偏移值</summary>
		///<param name="index">图形序号</param>
		///<param name="offset">基线偏移值</param>
		public void ChangeOffset(uint index, int offset)
		{
			if (m_Oscilloscope != IntPtr.Zero) Dll.ChangeOffset(m_Oscilloscope, index, offset);
		}

		///<summary>改变基线偏移值</summary>
		///<param name="lineWidth">基线偏移值</param>
		public void ChangeOffset(int offset)
		{
			if (m_Oscilloscope != IntPtr.Zero) Dll.ChangeOffset(m_Oscilloscope, 0u, offset);
		}

		///<summary>改变是否自动归零</summary>
		///<param name="index">图形序号</param>
		///<param name="isAutoTranslate">是否自动归零</param>
		public void ChangeIsAutoTranslate(uint index, bool isAutoTranslate)
		{
			if (m_Oscilloscope != IntPtr.Zero) Dll.ChangeIsAutoTranslate(m_Oscilloscope, index, isAutoTranslate);
		}

		///<summary>改变是否自动归零</summary>
		///<param name="isAutoTranslate">是否自动归零</param>
		public void ChangeIsAutoTranslate(bool isAutoTranslate)
		{
			if (m_Oscilloscope != IntPtr.Zero) Dll.ChangeIsAutoTranslate(m_Oscilloscope, 0u, isAutoTranslate);
		}

		///<summary>改变自动归零</summary>
		///<param name="index">图形序号</param>
		///<param name="autoTranslateOffset">自动归零偏移值（如果是非数值则不使用此功能，否则将最接近横轴的数值归零后加上此值。）</param>
		public void ChangeAutoTranslate(uint index, float autoTranslateOffset)
		{
			if (m_Oscilloscope != IntPtr.Zero) Dll.ChangeAutoTranslate(m_Oscilloscope, index, autoTranslateOffset);
		}

		///<summary>改变自动归零</summary>
		///<param name="autoTranslateOffset">自动归零偏移值（如果是非数值则不使用此功能，否则将最接近横轴的数值归零后加上此值。）</param>
		public void ChangeAutoTranslate(float autoTranslateOffset)
		{
			if (m_Oscilloscope != IntPtr.Zero) Dll.ChangeAutoTranslate(m_Oscilloscope, 0u, autoTranslateOffset);
		}

		///<summary>是否可用</summary>
		///<param name="index">图形序号</param>
		///<param name="isEnabled">是否可用</param>
		public void ChangeIsEnabled(uint index, bool isEnabled)
		{
			if (m_Oscilloscope != IntPtr.Zero) Dll.ChangeIsEnabled(m_Oscilloscope, index, isEnabled);
		}

		///<summary>是否可用</summary>
		///<param name="isEnabled">是否可用</param>
		public void ChangeIsEnabled(bool isEnabled)
		{
			if (m_Oscilloscope != IntPtr.Zero) Dll.ChangeIsEnabled(m_Oscilloscope, 0u, isEnabled);
		}

		///<summary>安全存入数据（并行安全）</summary>
		///<param name="index">图形序号</param>
		///<param name="value">值</param>
		public void SafePush(uint index, float value)
		{
			if (m_Oscilloscope != IntPtr.Zero) Dll.SafePush(m_Oscilloscope, index, value);
		}

		///<summary>存入数据</summary>
		///<param name="index">图形序号</param>
		///<param name="value">值</param>
		public void Push(uint index, float value)
		{
			if (m_Oscilloscope != IntPtr.Zero) Dll.Push(m_Oscilloscope, index, value);
		}

		///<summary>安全存入数据（并行安全）</summary>
		///<param name="value">值</param>
		public void SafePush(float value)
		{
			if (m_Oscilloscope != IntPtr.Zero) Dll.SafePush(m_Oscilloscope, 0u, value);
		}

		///<summary>存入数据</summary>
		///<param name="value">值</param>
		public void Push(float value)
		{
			if (m_Oscilloscope != IntPtr.Zero) Dll.Push(m_Oscilloscope, 0u, value);
		}

		///<summary>安全替换全部数据（并行安全）</summary>
		///<param name="index">图形序号</param>
		///<param name="value">值</param>
		public void SafeReplace(uint index, float[] value)
		{
			if (m_Oscilloscope != IntPtr.Zero) Dll.SafeReplace(m_Oscilloscope, index, value);
		}

		///<summary>替换全部数据</summary>
		///<param name="index">图形序号</param>
		///<param name="value">值</param>
		public void Replace(uint index, float[] value)
		{
			if (m_Oscilloscope != IntPtr.Zero) Dll.Replace(m_Oscilloscope, index, value);
		}

		///<summary>安全替换全部数据（并行安全）</summary>
		///<param name="value">值</param>
		public void SafeReplace(float[] value)
		{
			if (m_Oscilloscope != IntPtr.Zero) Dll.SafeReplace(m_Oscilloscope, 0u, value);
		}

		///<summary>替换全部数据</summary>
		///<param name="value">值</param>
		public void Replace(float[] value)
		{
			if (m_Oscilloscope != IntPtr.Zero) Dll.Replace(m_Oscilloscope, 0u, value);
		}

		///<summary>安全清空（并行安全）</summary>
		public void SafeClear()
		{
			if (m_Oscilloscope != IntPtr.Zero) Dll.SafeClear(m_Oscilloscope);
		}

		///<summary>清空</summary>
		public void Clear()
		{
			if (m_Oscilloscope != IntPtr.Zero) Dll.Clear(m_Oscilloscope);
		}

		///<summary>重绘</summary>
		protected override void OnPaint(PaintEventArgs e)
		{
			if (m_IsUninitialized)
			{
				e.Graphics.Clear(BackColor);
			}
			else
			{
				Dll.Refresh(m_Oscilloscope);
			}
		}

		///<summary>尺寸改变</summary>
		protected override void OnSizeChanged(EventArgs e)
		{
			base.OnSizeChanged(e);
			if (m_Oscilloscope != IntPtr.Zero)
			{
				try
				{
					Dll.Resize(m_Oscilloscope);
				}
				catch
				{
					m_Oscilloscope = IntPtr.Zero;
				}
			}
		}

		///<summary>当鼠标按下时</summary>
		protected override void OnMouseDown(MouseEventArgs e)
		{
			m_PointerX = e.X;
			m_PointerY = e.Y;
		}

		///<summary>当鼠标移动时</summary>
		protected override void OnMouseMove(MouseEventArgs e)
		{
			if (e.Button != MouseButtons.None)
			{
				var x = e.X;
				var y = e.Y;
				var offsetX = m_PointerX - x;
				var offsetY = m_PointerY - y;
				m_PointerX = x;
				m_PointerY = y;
				ChangeTranslate(offsetX, offsetY);
			}
		}

		///<summary>当滚轮改变时</summary>
		protected override void OnMouseWheel(MouseEventArgs e)
		{
			var delta = e.Delta > 0 ? 0.1f : -0.1f;
			var mouseButtons = MouseButtons;
			if ((mouseButtons & MouseButtons.Right) == MouseButtons.Right)
			{
				ChangeScaleX(delta);
			}
			else if (((mouseButtons & MouseButtons.Left) == MouseButtons.Left) || ((mouseButtons & MouseButtons.Middle) == MouseButtons.Middle))
			{
				ChangeScaleY(delta);
			}
			else
			{
				ChangeScale(delta);
			}
		}

	}

}