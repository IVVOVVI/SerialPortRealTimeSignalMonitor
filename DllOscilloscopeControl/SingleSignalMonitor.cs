using System;
using System.Windows;
using System.Windows.Media;
using DllInterface;

namespace Oscilloscope
{

	///<summary>单信号显示器</summary>
	public class SingleSignalMonitor : SignalMonitor, IDataReceiver
	{

		///<summary>单信号显示器</summary>
		static SingleSignalMonitor()
		{
			DefaultStyleKeyProperty.OverrideMetadata(typeof(SingleSignalMonitor), new FrameworkPropertyMetadata(typeof(SingleSignalMonitor)));
		}

		///<summary>设置项【图形样式】是否可见</summary>
		public static readonly DependencyProperty SettingGraphStyleVisibilityProperty = DependencyProperty.Register("SettingGraphStyleVisibility", typeof(Visibility), typeof(SingleSignalMonitor), new FrameworkPropertyMetadata(Visibility.Visible, FrameworkPropertyMetadataOptions.SubPropertiesDoNotAffectRender));
		///<summary>设置项【图形样式】是否可见</summary>
		public Visibility SettingGraphStyleVisibility
		{
			get { return (Visibility)GetValue(SettingGraphStyleVisibilityProperty); }
			set { SetValue(SettingGraphStyleVisibilityProperty, value); }
		}

		///<summary>设置项【是否启用】是否可见</summary>
		public static readonly DependencyProperty SettingIsGraphEnabledVisibilityProperty = DependencyProperty.Register("SettingIsGraphEnabledVisibility", typeof(Visibility), typeof(SingleSignalMonitor), new FrameworkPropertyMetadata(Visibility.Visible, FrameworkPropertyMetadataOptions.SubPropertiesDoNotAffectRender));
		///<summary>设置项【是否启用】是否可见</summary>
		public Visibility SettingIsGraphEnabledVisibility
		{
			get { return (Visibility)GetValue(SettingIsGraphEnabledVisibilityProperty); }
			set { SetValue(SettingIsGraphEnabledVisibilityProperty, value); }
		}

		///<summary>设置项【是否自动归零】是否可见</summary>
		public static readonly DependencyProperty SettingIsAutoTranslateVisibilityProperty = DependencyProperty.Register("SettingIsAutoTranslateVisibility", typeof(Visibility), typeof(SingleSignalMonitor), new FrameworkPropertyMetadata(Visibility.Collapsed, FrameworkPropertyMetadataOptions.SubPropertiesDoNotAffectRender));
		///<summary>设置项【是否自动归零】是否可见</summary>
		public Visibility SettingIsAutoTranslateVisibility
		{
			get { return (Visibility)GetValue(SettingIsAutoTranslateVisibilityProperty); }
			set { SetValue(SettingIsAutoTranslateVisibilityProperty, value); }
		}

		///<summary>设置项【基线类型】是否可见</summary>
		public static readonly DependencyProperty SettingBaselineTypeVisibilityProperty = DependencyProperty.Register("SettingBaselineTypeVisibility", typeof(Visibility), typeof(SingleSignalMonitor), new FrameworkPropertyMetadata(Visibility.Collapsed, FrameworkPropertyMetadataOptions.SubPropertiesDoNotAffectRender));
		///<summary>设置项【基线类型】是否可见</summary>
		public Visibility SettingBaselineTypeVisibility
		{
			get { return (Visibility)GetValue(SettingBaselineTypeVisibilityProperty); }
			set { SetValue(SettingBaselineTypeVisibilityProperty, value); }
		}

		///<summary>示波器图形</summary>
		public static readonly DependencyProperty GraphProperty = DependencyProperty.Register("Graph", typeof(OscilloscopeGraph), typeof(SingleSignalMonitor), new FrameworkPropertyMetadata(null, FrameworkPropertyMetadataOptions.SubPropertiesDoNotAffectRender));
		///<summary>示波器图形</summary>
		public OscilloscopeGraph Graph
		{
			get { return (OscilloscopeGraph)GetValue(GraphProperty); }
			private set { SetValue(GraphProperty, value); }
		}

		///<summary>示波器图形</summary>
		private readonly OscilloscopeGraph m_Graph = new OscilloscopeGraph();

		///<summary>单信号显示器</summary>
		public SingleSignalMonitor()
		{
			Graph = m_Graph;
		}

		///<summary>图形前景色</summary>
		public SolidColorBrush GraphForeground
		{
			get { return m_Graph.Foreground; }
			set { m_Graph.Foreground = value; }
		}

		///<summary>基线类型</summary>
		public Enum_BaselineType BaselineType
		{
			get { return m_Graph.BaselineType; }
			set { m_Graph.BaselineType = value; }
		}

		///<summary>图形样式</summary>
		public Enum_GraphStyle GraphStyle
		{
			get { return m_Graph.GraphStyle; }
			set { m_Graph.GraphStyle = value; }
		}

		///<summary>线宽</summary>
		public int LineWidth
		{
			get { return m_Graph.LineWidth; }
			set { m_Graph.LineWidth = value; }
		}

		///<summary>基线偏移值</summary>
		public int Offset
		{
			get { return m_Graph.Offset; }
			set { m_Graph.Offset = value; }
		}

		///<summary>是否自动归零</summary>
		public bool IsAutoTranslate
		{
			get { return m_Graph.IsAutoTranslate; }
			set { m_Graph.IsAutoTranslate = value; }
		}

		///<summary>自动归零偏移值（如果是非数值则不使用此功能，否则将最接近横轴的数值归零后加上此值。）</summary>
		public double AutoTranslateOffset
		{
			get { return m_Graph.AutoTranslateOffset; }
			set { m_Graph.AutoTranslateOffset = value; }
		}

		///<summary>图形是否可用</summary>
		public bool IsGraphEnabled
		{
			get { return m_Graph.IsGraphEnabled; }
			set { m_Graph.IsGraphEnabled = value; }
		}

		///<summary>刷新颜色</summary>
		public void RefreshColors()
		{
			m_Graph.RefreshColors();
		}

		///<summary>安全存入数据（并行安全）</summary>
		///<param name="value">值</param>
		public void SafePush(float value)
		{
			m_Graph.SafePush(value);
		}

		///<summary>存入数据</summary>
		///<param name="value">值</param>
		public void Push(float value)
		{
			m_Graph.Push(value);
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

		///<summary>初始化</summary>
		///<param name="window">窗口</param>
		public void Initialize(IWindow window)
		{
			if (IsFrozen) throw new InvalidOperationException();
			m_IsFrozen = true;
			m_Graph.Initialize(m_Core, 0u);
			var arg = new ConstructionParameters(window, ColorHelper.GetColor(ContentBackground, 0xFFFFFFFFu), (uint)DataLength, (float)Range, (float)ScaleX, (float)MaxScaleX, (float)ScaleY, (float)MaxScaleY, (uint)TranslateX, (uint)TranslateY, RefreshMode, ColorHelper.GetColor(AxisColor, 0xFF000000u), (uint)AxisLineWidth, (uint)AxisTicksCountX, (uint)AxisTicksCountY, (uint)AxisTickSizeX, (uint)AxisTickSizeY, (uint)AxisSizeX, (uint)AxisSizeY, AxisPlacementX, AxisPlacementY, new GraphProperties[] { new GraphProperties { BaselineType = BaselineType, GraphStyle = GraphStyle, Foreground = ColorHelper.GetColor(GraphForeground, 0xFF000000u), LineWidth = (uint)LineWidth, Offset = Offset, AutoTranslate = (float)AutoTranslateOffset, IsEnabled = IsGraphEnabled } });
			m_Core.Initialize(ref arg);
		}

		///<summary>关闭</summary>
		void IClose.Close() { }

	}

}