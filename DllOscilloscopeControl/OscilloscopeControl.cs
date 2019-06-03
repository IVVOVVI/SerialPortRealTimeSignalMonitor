using System.Windows;
using System.Windows.Forms;
using System.Windows.Forms.Integration;
using System.Windows.Media;

namespace Oscilloscope
{

	///<summary>示波器控件</summary>
	public class OscilloscopeControl : WindowsFormsHost
	{

		///<summary>示波器控件</summary>
		static OscilloscopeControl()
		{
			DefaultStyleKeyProperty.OverrideMetadata(typeof(OscilloscopeControl), new FrameworkPropertyMetadata(typeof(OscilloscopeControl)));
			ForegroundProperty.OverrideMetadata(typeof(OscilloscopeControl), new FrameworkPropertyMetadata(null, FrameworkPropertyMetadataOptions.SubPropertiesDoNotAffectRender, new PropertyChangedCallback((DependencyObject d, DependencyPropertyChangedEventArgs e) =>
			{
				if (e.NewValue != null)
				{
					var sender = d as OscilloscopeControl;
					if (sender != null && sender.m_IsPropertyEnabled)
					{
						var temp = e.NewValue as SolidColorBrush;
						if (temp != null) sender.m_Core.ChangeForegroundColor(ColorHelper.ToColorValue(temp.Color));
					}
				}
			})));
			BackgroundProperty.OverrideMetadata(typeof(OscilloscopeControl), new FrameworkPropertyMetadata(null, FrameworkPropertyMetadataOptions.SubPropertiesDoNotAffectRender, new PropertyChangedCallback((DependencyObject d, DependencyPropertyChangedEventArgs e) =>
			{
				if (e.NewValue != null)
				{
					var sender = d as OscilloscopeControl;
					if (sender != null && sender.m_IsPropertyEnabled)
					{
						var temp = e.NewValue as SolidColorBrush;
						if (temp != null) sender.m_Core.ChangeBackgroundColor(ColorHelper.ToColorValue(temp.Color));
					}
				}
			})));
		}

		///<summary>坐标轴颜色</summary>
		public static readonly DependencyProperty AxisColorProperty = DependencyProperty.Register("AxisColor", typeof(SolidColorBrush), typeof(OscilloscopeControl), new FrameworkPropertyMetadata(null, FrameworkPropertyMetadataOptions.BindsTwoWayByDefault | FrameworkPropertyMetadataOptions.SubPropertiesDoNotAffectRender, new PropertyChangedCallback((DependencyObject d, DependencyPropertyChangedEventArgs e) =>
		{
			if (e.NewValue != null)
			{
				var sender = d as OscilloscopeControl;
				if (sender != null && sender.m_IsPropertyEnabled)
				{
					var temp = e.NewValue as SolidColorBrush;
					if (temp != null) sender.m_Core.ChangeAxisColor(ColorHelper.ToColorValue(temp.Color));
				}
			}
		})));
		///<summary>坐标轴颜色</summary>
		public SolidColorBrush AxisColor
		{
			get { return (SolidColorBrush)GetValue(AxisColorProperty); }
			set { SetValue(AxisColorProperty, value); }
		}

		///<summary>坐标轴线宽</summary>
		public static readonly DependencyProperty AxisLineWidthProperty = DependencyProperty.Register("AxisLineWidth", typeof(int), typeof(OscilloscopeControl), new FrameworkPropertyMetadata(1, FrameworkPropertyMetadataOptions.BindsTwoWayByDefault | FrameworkPropertyMetadataOptions.SubPropertiesDoNotAffectRender, new PropertyChangedCallback((DependencyObject d, DependencyPropertyChangedEventArgs e) =>
		{
			if (e.NewValue != null)
			{
				var sender = d as OscilloscopeControl;
				if (sender != null && sender.m_IsPropertyEnabled)
				{
					var temp = (int)e.NewValue;
					if (temp > 0) sender.m_Core.ChangeAxisLineWidth((uint)temp);
				}
			}
		})));
		///<summary>坐标轴线宽</summary>
		public int AxisLineWidth
		{
			get { return (int)GetValue(AxisLineWidthProperty); }
			set { SetValue(AxisLineWidthProperty, value); }
		}

		///<summary>水平缩放率</summary>
		public static readonly DependencyProperty ScaleXProperty = DependencyProperty.Register("ScaleX", typeof(double), typeof(OscilloscopeControl), new FrameworkPropertyMetadata(1.0, FrameworkPropertyMetadataOptions.BindsTwoWayByDefault | FrameworkPropertyMetadataOptions.SubPropertiesDoNotAffectRender, new PropertyChangedCallback((DependencyObject d, DependencyPropertyChangedEventArgs e) =>
		{
			if (e.NewValue != null)
			{
				var sender = d as OscilloscopeControl;
				if (sender != null && sender.m_IsPropertyEnabled)
				{
					var oldValue = e.OldValue == null ? 1.0 : (double)e.OldValue;
					var newValue = (double)e.NewValue;
					if (newValue != oldValue) sender.m_Core.ChangeScaleX((float)(newValue - oldValue));
				}
			}
		})));
		///<summary>水平缩放率</summary>
		public double ScaleX
		{
			get { return (double)GetValue(ScaleXProperty); }
			set { SetValue(ScaleXProperty, value); }
		}

		///<summary>水平缩放率最大值</summary>
		public static readonly DependencyProperty MaxScaleXProperty = DependencyProperty.Register("MaxScaleX", typeof(double), typeof(OscilloscopeControl), new FrameworkPropertyMetadata((double)Dll.DefaultScaleLimit, FrameworkPropertyMetadataOptions.BindsTwoWayByDefault | FrameworkPropertyMetadataOptions.SubPropertiesDoNotAffectRender, new PropertyChangedCallback((DependencyObject d, DependencyPropertyChangedEventArgs e) =>
		{
			if (e.NewValue != null)
			{
				var sender = d as OscilloscopeControl;
				if (sender != null && sender.m_IsPropertyEnabled && sender.m_Core != null)
				{
					var oldValue = e.OldValue == null ? Dll.DefaultScaleLimit : (uint)((double)e.OldValue);
					var newValue = (uint)((double)e.NewValue);
					if (newValue != oldValue) sender.m_Core.ChangeScaleLimitX(newValue);
				}
			}
		})));
		///<summary>水平缩放率最大值</summary>
		public double MaxScaleX
		{
			get { return (double)GetValue(MaxScaleXProperty); }
			set { SetValue(MaxScaleXProperty, value); }
		}

		///<summary>水平缩放率最小值</summary>
		public static readonly DependencyProperty MinScaleXProperty = DependencyProperty.Register("MinScaleX", typeof(double), typeof(OscilloscopeControl), new FrameworkPropertyMetadata(1.0, FrameworkPropertyMetadataOptions.SubPropertiesDoNotAffectRender));
		///<summary>水平缩放率最小值</summary>
		public double MinScaleX
		{
			get { return 1.0; }
		}

		///<summary>垂直缩放率</summary>
		public static readonly DependencyProperty ScaleYProperty = DependencyProperty.Register("ScaleY", typeof(double), typeof(OscilloscopeControl), new FrameworkPropertyMetadata(1.0, FrameworkPropertyMetadataOptions.BindsTwoWayByDefault | FrameworkPropertyMetadataOptions.SubPropertiesDoNotAffectRender, new PropertyChangedCallback((DependencyObject d, DependencyPropertyChangedEventArgs e) =>
		{
			if (e.NewValue != null)
			{
				var sender = d as OscilloscopeControl;
				if (sender != null && sender.m_IsPropertyEnabled && sender.m_Core != null)
				{
					var oldValue = e.OldValue == null ? 1.0 : (double)e.OldValue;
					var newValue = (double)e.NewValue;
					if (newValue != oldValue) sender.m_Core.ChangeScaleY((float)(newValue - oldValue));
				}
			}
		})));
		///<summary>垂直缩放率</summary>
		public double ScaleY
		{
			get { return (double)GetValue(ScaleYProperty); }
			set { SetValue(ScaleYProperty, value); }
		}

		///<summary>垂直缩放率最大值</summary>
		public static readonly DependencyProperty MaxScaleYProperty = DependencyProperty.Register("MaxScaleY", typeof(double), typeof(OscilloscopeControl), new FrameworkPropertyMetadata((double)Dll.DefaultScaleLimit, FrameworkPropertyMetadataOptions.BindsTwoWayByDefault | FrameworkPropertyMetadataOptions.SubPropertiesDoNotAffectRender, new PropertyChangedCallback((DependencyObject d, DependencyPropertyChangedEventArgs e) =>
		{
			if (e.NewValue != null)
			{
				var sender = d as OscilloscopeControl;
				if (sender != null && sender.m_IsPropertyEnabled && sender.m_Core != null)
				{
					var oldValue = e.OldValue == null ? Dll.DefaultScaleLimit : (uint)((double)e.OldValue);
					var newValue = (uint)((double)e.NewValue);
					if (newValue != oldValue) sender.m_Core.ChangeScaleLimitY(newValue);
				}
			}
		})));
		///<summary>垂直缩放率最大值</summary>
		public double MaxScaleY
		{
			get { return (double)GetValue(MaxScaleYProperty); }
			set { SetValue(MaxScaleYProperty, value); }
		}

		///<summary>垂直缩放率最小值</summary>
		public static readonly DependencyProperty MinScaleYProperty = DependencyProperty.Register("MinScaleY", typeof(double), typeof(OscilloscopeControl), new FrameworkPropertyMetadata(1.0, FrameworkPropertyMetadataOptions.SubPropertiesDoNotAffectRender));
		///<summary>垂直缩放率最小值</summary>
		public double MinScaleY
		{
			get { return 1.0; }
		}

		///<summary>水平位移值</summary>
		public static readonly DependencyProperty TranslateXProperty = DependencyProperty.Register("TranslateX", typeof(double), typeof(OscilloscopeControl), new FrameworkPropertyMetadata(0.0, FrameworkPropertyMetadataOptions.BindsTwoWayByDefault | FrameworkPropertyMetadataOptions.SubPropertiesDoNotAffectRender, new PropertyChangedCallback((DependencyObject d, DependencyPropertyChangedEventArgs e) =>
		{
			if (e.NewValue != null)
			{
				var sender = d as OscilloscopeControl;
				if (sender != null && sender.m_IsPropertyEnabled && sender.m_Core != null)
				{
					var oldValue = e.OldValue == null ? 0 : (int)((double)e.OldValue);
					var newValue = (int)((double)e.NewValue);
					if (newValue != oldValue) sender.m_Core.ChangeTranslateX(newValue - oldValue);
				}
			}
		})));
		///<summary>水平位移值</summary>
		public double TranslateX
		{
			get { return (double)GetValue(TranslateXProperty); }
			set { SetValue(TranslateXProperty, value); }
		}

		///<summary>水平位移值最大值</summary>
		public static readonly DependencyProperty MaxTranslateXProperty = DependencyProperty.Register("MaxTranslateX", typeof(double), typeof(OscilloscopeControl), new FrameworkPropertyMetadata(0.0, FrameworkPropertyMetadataOptions.SubPropertiesDoNotAffectRender));
		///<summary>水平位移值最大值</summary>
		public double MaxTranslateX
		{
			get { return (double)GetValue(MaxTranslateXProperty); }
			private set { SetValue(MaxTranslateXProperty, value); }
		}

		///<summary>水平位移值最小值</summary>
		public static readonly DependencyProperty MinTranslateXProperty = DependencyProperty.Register("MinTranslateX", typeof(double), typeof(OscilloscopeControl), new FrameworkPropertyMetadata(0.0, FrameworkPropertyMetadataOptions.SubPropertiesDoNotAffectRender));
		///<summary>水平位移值最小值</summary>
		public double MinTranslateX
		{
			get { return 0.0; }
		}

		///<summary>垂直位移值</summary>
		public static readonly DependencyProperty TranslateYProperty = DependencyProperty.Register("TranslateY", typeof(double), typeof(OscilloscopeControl), new FrameworkPropertyMetadata(0.0, FrameworkPropertyMetadataOptions.BindsTwoWayByDefault | FrameworkPropertyMetadataOptions.SubPropertiesDoNotAffectRender, new PropertyChangedCallback((DependencyObject d, DependencyPropertyChangedEventArgs e) =>
		{
			if (e.NewValue != null)
			{
				var sender = d as OscilloscopeControl;
				if (sender != null && sender.m_IsPropertyEnabled && sender.m_Core != null)
				{
					var oldValue = e.OldValue == null ? 0 : (int)((double)e.OldValue);
					var newValue = (int)((double)e.NewValue);
					if (newValue != oldValue) sender.m_Core.ChangeTranslateY(newValue - oldValue);
				}
			}
		})));
		///<summary>垂直位移值</summary>
		public double TranslateY
		{
			get { return (double)GetValue(TranslateYProperty); }
			set { SetValue(TranslateYProperty, value); }
		}

		///<summary>垂直位移值最大值</summary>
		public static readonly DependencyProperty MaxTranslateYProperty = DependencyProperty.Register("MaxTranslateY", typeof(double), typeof(OscilloscopeControl), new FrameworkPropertyMetadata(0.0, FrameworkPropertyMetadataOptions.SubPropertiesDoNotAffectRender));
		///<summary>垂直位移值最大值</summary>
		public double MaxTranslateY
		{
			get { return (double)GetValue(MaxTranslateYProperty); }
			private set { SetValue(MaxTranslateYProperty, value); }
		}

		///<summary>垂直位移值最小值</summary>
		public static readonly DependencyProperty MinTranslateYProperty = DependencyProperty.Register("MinTranslateY", typeof(double), typeof(OscilloscopeControl), new FrameworkPropertyMetadata(0.0, FrameworkPropertyMetadataOptions.SubPropertiesDoNotAffectRender));
		///<summary>垂直位移值最小值</summary>
		public double MinTranslateY
		{
			get { return 0.0; }
		}
		
		///<summary>刷新模式（false - 扫描 | true - 滚动）</summary>
		public static readonly DependencyProperty RefreshModeProperty = DependencyProperty.Register("RefreshMode", typeof(bool), typeof(OscilloscopeControl), new FrameworkPropertyMetadata(false, FrameworkPropertyMetadataOptions.SubPropertiesDoNotAffectRender, new PropertyChangedCallback((DependencyObject d, DependencyPropertyChangedEventArgs e) =>
		{
			if (e.NewValue != null)
			{
				var sender = d as OscilloscopeControl;
				if (sender != null && sender.m_Core != null) sender.m_Core.ChangeRefreshMode((bool)e.NewValue ? Enum_RefreshMode.Scrolling : Enum_RefreshMode.Scanning);
			}
		})));
		///<summary>刷新模式（false - 扫描 | true - 滚动）</summary>
		public bool RefreshMode
		{
			get { return (bool)GetValue(RefreshModeProperty); }
			set { SetValue(RefreshModeProperty, value); }
		}

		///<summary>是否未初始化</summary>
		private bool m_IsUninitialized = true;

		///<summary>属性是否可用</summary>
		private bool m_IsPropertyEnabled = false;

		///<summary>内核</summary>
		private readonly Oscilloscope m_Core;

		///<summary>示波器控件</summary>
		public OscilloscopeControl()
		{
			m_Core = new Oscilloscope();
			base.Child = m_Core;
			m_Core.PropertyChanged += m_OnPropertyChanged;
		}

		///<summary>初始化</summary>
		///<param name="arg">参数</param>
		public void Initialize(ref ConstructionParameters arg)
		{
			if (arg != null && m_IsUninitialized)
			{
				m_IsUninitialized = false;
				m_Core.Initialize(ref arg);
				var brush = Background as SolidColorBrush;
				var color = ColorHelper.ToColor(arg.Background);
				if (brush == null || brush.Color != color) Background = new SolidColorBrush(color);
				RefreshMode = arg.RefreshMode == Enum_RefreshMode.Scrolling;
				m_IsPropertyEnabled = true;
			}
		}

		///<summary>改变基线类型</summary>
		///<param name="baselineType">基线类型</param>
		public void ChangeBaselineType(Enum_BaselineType baselineType)
		{
			m_Core.ChangeBaselineType(baselineType);
		}

		///<summary>改变基线类型</summary>
		///<param name="index">图形序号</param>
		///<param name="baselineType">基线类型</param>
		public void ChangeBaselineType(uint index, Enum_BaselineType baselineType)
		{
			m_Core.ChangeBaselineType(index, baselineType);
		}

		///<summary>改变示波器图形样式</summary>
		///<param name="graphStyle">图形样式数组</param>
		public void ChangeGraphStyle(Enum_GraphStyle[] graphStyle)
		{
			m_Core.ChangeGraphStyle(graphStyle);
		}

		///<summary>改变示波器图形样式</summary>
		///<param name="index">图形序号</param>
		///<param name="graphStyle">图形样式</param>
		public void ChangeGraphStyle(uint index, Enum_GraphStyle graphStyle)
		{
			m_Core.ChangeGraphStyle(index, graphStyle);
		}

		///<summary>改变示波器图形样式</summary>
		///<param name="graphStyle">图形样式</param>
		public void ChangeGraphStyle(Enum_GraphStyle graphStyle)
		{
			m_Core.ChangeGraphStyle(graphStyle);
		}

		///<summary>改变示波器前景色</summary>
		///<param name="foreground">前景色数组</param>
		public void ChangeForegroundColor(uint[] foreground)
		{
			m_Core.ChangeForegroundColor(foreground);
		}

		///<summary>改变示波器前景色</summary>
		///<param name="index">图形序号</param>
		///<param name="foreground">前景色</param>
		public void ChangeForegroundColor(uint index, uint foreground)
		{
			m_Core.ChangeForegroundColor(index, foreground);
		}

		///<summary>改变示波器前景色</summary>
		///<param name="foreground">前景色</param>
		public void ChangeForegroundColor(uint foreground)
		{
			m_Core.ChangeForegroundColor(foreground);
		}

		///<summary>改变线宽</summary>
		///<param name="index">图形序号</param>
		///<param name="lineWidth">线宽</param>
		public void ChangeLineWidth(uint index, byte lineWidth)
		{
			m_Core.ChangeLineWidth(index, lineWidth);
		}

		///<summary>改变线宽</summary>
		///<param name="lineWidth">线宽</param>
		public void ChangeLineWidth(byte lineWidth)
		{
			m_Core.ChangeLineWidth(lineWidth);
		}

		///<summary>改变基线偏移值</summary>
		///<param name="index">图形序号</param>
		///<param name="offset">基线偏移值</param>
		public void ChangeOffset(uint index, int offset)
		{
			m_Core.ChangeOffset(index, offset);
		}

		///<summary>改变基线偏移值</summary>
		///<param name="lineWidth">基线偏移值</param>
		public void ChangeOffset(int offset)
		{
			m_Core.ChangeOffset(offset);
		}

		///<summary>改变是否自动归零</summary>
		///<param name="index">图形序号</param>
		///<param name="isAutoTranslate">是否自动归零</param>
		public void ChangeIsAutoTranslate(uint index, bool isAutoTranslate)
		{
			m_Core.ChangeIsAutoTranslate(index, isAutoTranslate);
		}

		///<summary>改变是否自动归零</summary>
		///<param name="isAutoTranslate">是否自动归零</param>
		public void ChangeIsAutoTranslate(bool isAutoTranslate)
		{
			m_Core.ChangeIsAutoTranslate(isAutoTranslate);
		}

		///<summary>改变自动归零</summary>
		///<param name="index">图形序号</param>
		///<param name="autoTranslateOffset">自动归零偏移值（如果是非数值则不使用此功能，否则将最接近横轴的数值归零后加上此值。）</param>
		public void ChangeAutoTranslate(uint index, float autoTranslateOffset)
		{
			m_Core.ChangeAutoTranslate(index, autoTranslateOffset);
		}

		///<summary>改变自动归零</summary>
		///<param name="autoTranslateOffset">自动归零偏移值（如果是非数值则不使用此功能，否则将最接近横轴的数值归零后加上此值。）</param>
		public void ChangeAutoTranslate(float autoTranslateOffset)
		{
			m_Core.ChangeAutoTranslate(autoTranslateOffset);
		}

		///<summary>图形是否可用</summary>
		///<param name="index">图形序号</param>
		///<param name="isEnabled">是否可用</param>
		public void ChangeIsEnabled(uint index, bool isEnabled)
		{
			m_Core.ChangeIsEnabled(index, isEnabled);
		}

		///<summary>图形是否可用</summary>
		///<param name="isEnabled">是否可用</param>
		public void ChangeIsEnabled(bool isEnabled)
		{
			m_Core.ChangeIsEnabled(isEnabled);
		}

		///<summary>安全存入数据（并行安全）</summary>
		///<param name="index">图形序号</param>
		///<param name="value">值</param>
		public void SafePush(uint index, float value)
		{
			m_Core.SafePush(index, value);
		}

		///<summary>存入数据</summary>
		///<param name="index">图形序号</param>
		///<param name="value">值</param>
		public void Push(uint index, float value)
		{
			m_Core.Push(index, value);
		}

		///<summary>安全存入数据（并行安全）</summary>
		///<param name="value">值</param>
		public void SafePush(float value)
		{
			m_Core.SafePush(value);
		}

		///<summary>存入数据</summary>
		///<param name="value">值</param>
		public void Push(float value)
		{
			m_Core.Push(value);
		}

		///<summary>安全替换全部数据（并行安全）</summary>
		///<param name="index">图形序号</param>
		///<param name="value">值</param>
		public void SafeReplace(uint index, float[] value)
		{
			m_Core.SafeReplace(index, value);
		}

		///<summary>替换全部数据</summary>
		///<param name="index">图形序号</param>
		///<param name="value">值</param>
		public void Replace(uint index, float[] value)
		{
			m_Core.Replace(index, value);
		}

		///<summary>安全替换全部数据（并行安全）</summary>
		///<param name="value">值</param>
		public void SafeReplace(float[] value)
		{
			m_Core.SafeReplace(value);
		}

		///<summary>替换全部数据</summary>
		///<param name="value">值</param>
		public void Replace(float[] value)
		{
			m_Core.Replace(value);
		}

		///<summary>安全清空（并行安全）</summary>
		public void SafeClear()
		{
			m_Core.SafeClear();
		}

		///<summary>清空</summary>
		public void Clear()
		{
			m_Core.Clear();
		}

		///<summary>刷新颜色</summary>
		public void RefreshColors()
		{
			var temp = Background as SolidColorBrush;
			if (temp != null) m_Core.ChangeBackgroundColor(ColorHelper.ToColorValue(temp.Color));
			temp = AxisColor;
			if (temp != null) m_Core.ChangeAxisColor(ColorHelper.ToColorValue(temp.Color));
		}

		///<summary>当属性改变时</summary>
		private void m_OnPropertyChanged(Oscilloscope.Property property, float arg)
		{
			var value = (double)arg;
			m_IsPropertyEnabled = false;
			switch (property)
			{
				case Oscilloscope.Property.ScaleX:
					if (value != ScaleX) ScaleX = value;
					break;
				case Oscilloscope.Property.ScaleX_Limit:
					if (value != MaxScaleX) MaxScaleX = value;
					break;
				case Oscilloscope.Property.ScaleY:
					if (value != ScaleY) ScaleY = value;
					break;
				case Oscilloscope.Property.ScaleY_Limit:
					if (value != MaxScaleY) MaxScaleY = value;
					break;
				case Oscilloscope.Property.TranslateX:
					if (value != TranslateX) TranslateX = value;
					break;
				case Oscilloscope.Property.TranslateX_Limit:
					if (value != MaxTranslateX) MaxTranslateX = value;
					break;
				case Oscilloscope.Property.TranslateY:
					if (value != TranslateY) TranslateY = value;
					break;
				case Oscilloscope.Property.TranslateY_Limit:
					if (value != MaxTranslateY) MaxTranslateY = value;
					break;
				default:
					break;
			}
			m_IsPropertyEnabled = true;
		}

		///<summary>内核</summary>
		private new Control Child
		{
			get
			{
				return base.Child;
			}
		}

	}

}