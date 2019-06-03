using System.Windows;
using System.Windows.Media;
using DllInterface;

namespace Oscilloscope
{

	///<summary>示波器图形</summary>
	public class OscilloscopeGraph : DependencyObject, IDataReceiver
	{

		///<summary>是否是线条图形</summary>
		public static readonly DependencyProperty IsLineGraphProperty = DependencyProperty.Register("IsLineGraph", typeof(bool), typeof(OscilloscopeGraph), new FrameworkPropertyMetadata(false, FrameworkPropertyMetadataOptions.SubPropertiesDoNotAffectRender));
		///<summary>是否是线条图形</summary>
		public bool IsLineGraph
		{
			get { return (bool)GetValue(IsLineGraphProperty); }
			private set { SetValue(IsLineGraphProperty, value); }
		}

		///<summary>基线类型</summary>
		public static readonly DependencyProperty BaselineTypeIndexProperty = DependencyProperty.Register("BaselineTypeIndex", typeof(int), typeof(OscilloscopeGraph), new FrameworkPropertyMetadata(0, FrameworkPropertyMetadataOptions.BindsTwoWayByDefault | FrameworkPropertyMetadataOptions.SubPropertiesDoNotAffectRender, new PropertyChangedCallback((DependencyObject d, DependencyPropertyChangedEventArgs e) =>
		{
			if (e.NewValue != null)
			{
				var sender = d as OscilloscopeGraph;
				if (sender != null && sender.m_Core != null)
				{
					var oldValue = e.OldValue == null ? 0 : (int)e.OldValue;
					var newValue = (int)e.NewValue;
					if (newValue != oldValue) sender.m_Core.ChangeBaselineType(sender.m_Index, (Enum_BaselineType)newValue);
				}
			}
		})));
		///<summary>基线类型</summary>
		public int BaselineTypeIndex
		{
			get { return (int)GetValue(BaselineTypeIndexProperty); }
			set { SetValue(BaselineTypeIndexProperty, value); }
		}

		///<summary>基线类型</summary>
		public Enum_BaselineType BaselineType
		{
			get { return (Enum_BaselineType)(int)GetValue(BaselineTypeIndexProperty); }
			set { SetValue(BaselineTypeIndexProperty, (int)value); }
		}

		///<summary>图形样式</summary>
		public static readonly DependencyProperty GraphStyleIndexProperty = DependencyProperty.Register("GraphStyleIndex", typeof(int), typeof(OscilloscopeGraph), new FrameworkPropertyMetadata(0, FrameworkPropertyMetadataOptions.BindsTwoWayByDefault | FrameworkPropertyMetadataOptions.SubPropertiesDoNotAffectRender, new PropertyChangedCallback((DependencyObject d, DependencyPropertyChangedEventArgs e) =>
		{
			if (e.NewValue != null)
			{
				var sender = d as OscilloscopeGraph;
				if (sender != null)
				{
					var oldValue = e.OldValue == null ? 0 : (int)e.OldValue;
					var newValue = (int)e.NewValue;
					if (newValue != oldValue)
					{
						var style = (Enum_GraphStyle)newValue;
						if (sender.m_Core != null) sender.m_Core.ChangeGraphStyle(sender.m_Index, style);
						switch (style)
						{
							case Enum_GraphStyle.Bar:
								sender.IsLineGraph = false;
								break;
							case Enum_GraphStyle.Line:
								sender.IsLineGraph = true;
								break;
						}
					}
				}
			}
		})));
		///<summary>图形样式</summary>
		public int GraphStyleIndex
		{
			get { return (int)GetValue(GraphStyleIndexProperty); }
			set { SetValue(GraphStyleIndexProperty, value); }
		}

		///<summary>图形样式</summary>
		public Enum_GraphStyle GraphStyle
		{
			get { return (Enum_GraphStyle)(int)GetValue(GraphStyleIndexProperty); }
			set { SetValue(GraphStyleIndexProperty, (int)value); }
		}

		///<summary>前景色</summary>
		public static readonly DependencyProperty ForegroundProperty = DependencyProperty.Register("Foreground", typeof(SolidColorBrush), typeof(OscilloscopeGraph), new FrameworkPropertyMetadata(null, FrameworkPropertyMetadataOptions.BindsTwoWayByDefault | FrameworkPropertyMetadataOptions.SubPropertiesDoNotAffectRender, new PropertyChangedCallback((DependencyObject d, DependencyPropertyChangedEventArgs e) =>
		{
			if (e.NewValue != null)
			{
				var temp = e.NewValue as SolidColorBrush;
				if (temp != null)
				{
					var sender = d as OscilloscopeGraph;
					if (sender != null && sender.m_Core != null) sender.m_Core.ChangeForegroundColor(sender.m_Index, ColorHelper.ToColorValue(temp.Color));
				}
			}
		})));
		///<summary>前景色</summary>
		public SolidColorBrush Foreground
		{
			get { return (SolidColorBrush)GetValue(ForegroundProperty); }
			set { SetValue(ForegroundProperty, value); }
		}

		///<summary>线宽</summary>
		public static readonly DependencyProperty LineWidthProperty = DependencyProperty.Register("LineWidth", typeof(int), typeof(OscilloscopeGraph), new FrameworkPropertyMetadata(1, FrameworkPropertyMetadataOptions.BindsTwoWayByDefault | FrameworkPropertyMetadataOptions.SubPropertiesDoNotAffectRender, new PropertyChangedCallback((DependencyObject d, DependencyPropertyChangedEventArgs e) =>
		{
			if (e.NewValue != null)
			{
				var sender = d as OscilloscopeGraph;
				if (sender != null && sender.m_Core != null)
				{
					var oldValue = e.OldValue == null ? 1 : (int)e.OldValue;
					var newValue = (int)e.NewValue;
					if (newValue != oldValue) sender.m_Core.ChangeLineWidth(sender.m_Index, (byte)newValue);
				}
			}
		})));
		///<summary>线宽</summary>
		public int LineWidth
		{
			get { return (int)GetValue(LineWidthProperty); }
			set { SetValue(LineWidthProperty, value); }
		}

		///<summary>基线偏移值</summary>
		public static readonly DependencyProperty OffsetProperty = DependencyProperty.Register("Offset", typeof(int), typeof(OscilloscopeGraph), new FrameworkPropertyMetadata(0, FrameworkPropertyMetadataOptions.BindsTwoWayByDefault | FrameworkPropertyMetadataOptions.SubPropertiesDoNotAffectRender, new PropertyChangedCallback((DependencyObject d, DependencyPropertyChangedEventArgs e) =>
		{
			if (e.NewValue != null)
			{
				var sender = d as OscilloscopeGraph;
				if (sender != null && sender.m_Core != null)
				{
					var oldValue = e.OldValue == null ? 0.0 : (int)e.OldValue;
					var newValue = (int)e.NewValue;
					if (newValue != oldValue) sender.m_Core.ChangeOffset(sender.m_Index, newValue);
				}
			}
		})));
		///<summary>基线偏移值</summary>
		public int Offset
		{
			get { return (int)GetValue(OffsetProperty); }
			set { SetValue(OffsetProperty, value); }
		}

		///<summary>是否自动归零</summary>
		public static readonly DependencyProperty IsAutoTranslateProperty = DependencyProperty.Register("IsAutoTranslate", typeof(bool), typeof(OscilloscopeGraph), new FrameworkPropertyMetadata(false, FrameworkPropertyMetadataOptions.BindsTwoWayByDefault | FrameworkPropertyMetadataOptions.SubPropertiesDoNotAffectRender, new PropertyChangedCallback((DependencyObject d, DependencyPropertyChangedEventArgs e) =>
		{
			if (e.NewValue != null)
			{
				var sender = d as OscilloscopeGraph;
				if (sender != null && sender.m_Core != null)
				{
					var oldValue = e.OldValue == null ? false : (bool)e.OldValue;
					var newValue = (bool)e.NewValue;
					if (newValue != oldValue)
					{
						if (newValue)
						{
							sender.m_Core.ChangeAutoTranslate(sender.m_Index, sender.m_AutoTranslateOffset);
						}
						else
						{
							sender.m_Core.ChangeIsAutoTranslate(false);
						}
					}
				}
			}
		})));
		///<summary>是否自动归零</summary>
		public bool IsAutoTranslate
		{
			get { return (bool)GetValue(IsAutoTranslateProperty); }
			set { SetValue(IsAutoTranslateProperty, value); }
		}

		///<summary>自动归零偏移值（如果是非数值则不使用此功能，否则将最接近横轴的数值归零后加上此值。）</summary>
		private float m_AutoTranslateOffset = float.NaN;
		///<summary>自动归零偏移值（如果是非数值则不使用此功能，否则将最接近横轴的数值归零后加上此值。）</summary>
		public double AutoTranslateOffset
		{
			get { return m_AutoTranslateOffset; }
			set
			{
				m_AutoTranslateOffset = (float)value;
				IsAutoTranslate = float.IsNaN(m_AutoTranslateOffset) || float.IsPositiveInfinity(m_AutoTranslateOffset) || float.IsNegativeInfinity(m_AutoTranslateOffset);
			}
		}

		///<summary>图形是否可用</summary>
		public static readonly DependencyProperty IsGraphEnabledProperty = DependencyProperty.Register("IsGraphEnabled", typeof(bool), typeof(OscilloscopeGraph), new FrameworkPropertyMetadata(true, FrameworkPropertyMetadataOptions.BindsTwoWayByDefault | FrameworkPropertyMetadataOptions.SubPropertiesDoNotAffectRender, new PropertyChangedCallback((DependencyObject d, DependencyPropertyChangedEventArgs e) =>
		{
			if (e.NewValue != null)
			{
				var sender = d as OscilloscopeGraph;
				if (sender != null && sender.m_Core != null)
				{
					var oldValue = e.OldValue == null ? true : (bool)e.OldValue;
					var newValue = (bool)e.NewValue;
					if (newValue != oldValue) sender.m_Core.ChangeIsEnabled(sender.m_Index, newValue);
				}
			}
		})));
		///<summary>图形是否可用</summary>
		public bool IsGraphEnabled
		{
			get { return (bool)GetValue(IsGraphEnabledProperty); }
			set { SetValue(IsGraphEnabledProperty, value); }
		}

		///<summary>索引</summary>
		private uint m_Index;

		///<summary>内核</summary>
		protected OscilloscopeControl m_Core = null;

		///<summary>初始化</summary>
		///<param name="core">内核</param>
		///<param name="index">索引</param>
		internal void Initialize(OscilloscopeControl core, uint index)
		{
			if (m_Core == null)
			{
				m_Core = core;
				m_Index = index;
			}
		}

		///<summary>刷新颜色</summary>
		public void RefreshColors()
		{
			if (m_Core != null)
			{
				var foreground = Foreground as SolidColorBrush;
				if (foreground != null) m_Core.ChangeForegroundColor(m_Index, ColorHelper.ToColorValue(foreground.Color));
			}
		}

		///<summary>安全存入数据（并行安全）</summary>
		///<param name="value">值</param>
		public void SafePush(float value)
		{
			if (m_Core != null) m_Core.SafePush(m_Index, value);
		}

		///<summary>存入数据</summary>
		///<param name="value">值</param>
		public void Push(float value)
		{
			if (m_Core != null) m_Core.Push(m_Index, value);
		}

		///<summary>安全替换全部数据（并行安全）</summary>
		///<param name="value">值</param>
		public void SafeReplace(float[] value)
		{
			if (m_Core != null) m_Core.SafeReplace(m_Index, value);
		}

		///<summary>替换全部数据</summary>
		///<param name="value">值</param>
		public void Replace(float[] value)
		{
			if (m_Core != null) m_Core.Replace(m_Index, value);
		}

		///<summary>安全清空（并行安全）</summary>
		public void SafeClear()
		{
			if (m_Core != null) m_Core.SafeClear();
		}

		///<summary>清空</summary>
		public void Clear()
		{
			if (m_Core != null) m_Core.Clear();
		}

		///<summary>关闭</summary>
		void IClose.Close() { }

	}

}

