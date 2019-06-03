using System;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Media;

namespace Oscilloscope
{

	///<summary>信号显示器</summary>
	public class SignalMonitor : Control
	{

		///<summary>标签</summary>
		public static readonly DependencyProperty LabelProperty = DependencyProperty.Register("Label", typeof(string), typeof(SignalMonitor), new FrameworkPropertyMetadata(null, FrameworkPropertyMetadataOptions.SubPropertiesDoNotAffectRender));
		///<summary>标签</summary>
		public string Label
		{
			get { return (string)GetValue(LabelProperty); }
			set { SetValue(LabelProperty, value); }
		}

		///<summary>标签是否可见</summary>
		public static readonly DependencyProperty LabelVisibilityProperty = DependencyProperty.Register("LabelVisibility", typeof(Visibility), typeof(SignalMonitor), new FrameworkPropertyMetadata(Visibility.Collapsed, FrameworkPropertyMetadataOptions.SubPropertiesDoNotAffectRender));
		///<summary>标签是否可见</summary>
		public Visibility LabelVisibility
		{
			get { return (Visibility)GetValue(SettingPanelVisibilityProperty); }
			set { SetValue(SettingPanelVisibilityProperty, value); }
		}

		///<summary>设置面板是否可见</summary>
		public static readonly DependencyProperty SettingPanelVisibilityProperty = DependencyProperty.Register("SettingPanelVisibility", typeof(Visibility), typeof(SignalMonitor), new FrameworkPropertyMetadata(Visibility.Visible, FrameworkPropertyMetadataOptions.SubPropertiesDoNotAffectRender));
		///<summary>设置面板是否可见</summary>
		public Visibility SettingPanelVisibility
		{
			get { return (Visibility)GetValue(SettingPanelVisibilityProperty); }
			set { SetValue(SettingPanelVisibilityProperty, value); }
		}

		///<summary>设置项【刷新模式】是否可见</summary>
		public static readonly DependencyProperty SettingRefreshModeVisibilityProperty = DependencyProperty.Register("SettingRefreshModeVisibility", typeof(Visibility), typeof(SignalMonitor), new FrameworkPropertyMetadata(Visibility.Visible, FrameworkPropertyMetadataOptions.SubPropertiesDoNotAffectRender));
		///<summary>设置项【刷新模式】是否可见</summary>
		public Visibility SettingRefreshModeVisibility
		{
			get { return (Visibility)GetValue(SettingRefreshModeVisibilityProperty); }
			set { SetValue(SettingRefreshModeVisibilityProperty, value); }
		}

		///<summary>内容</summary>
		public static readonly DependencyProperty ContentProperty = DependencyProperty.Register("Content", typeof(OscilloscopeControl), typeof(SignalMonitor), new FrameworkPropertyMetadata(null, FrameworkPropertyMetadataOptions.SubPropertiesDoNotAffectRender));
		///<summary>内容</summary>
		public OscilloscopeControl Content
		{
			get { return (OscilloscopeControl)GetValue(ContentProperty); }
			private set { SetValue(ContentProperty, value); }
		}

		///<summary>内核</summary>
		protected readonly OscilloscopeControl m_Core = new OscilloscopeControl();

		///<summary>是否冻结</summary>
		protected bool m_IsFrozen = false;

		///<summary>信号显示器</summary>
		protected SignalMonitor()
		{
			Content = m_Core;
		}

		///<summary>是否冻结</summary>
		public bool IsFrozen
		{
			get { return m_IsFrozen; }
		}

		///<summary>内容背景色</summary>
		public SolidColorBrush ContentBackground
		{
			get { return m_Core.Background as SolidColorBrush; }
			set { m_Core.Background = value; }
		}

		///<summary>坐标轴颜色</summary>
		public SolidColorBrush AxisColor
		{
			get { return m_Core.AxisColor; }
			set { m_Core.AxisColor = value; }
		}

		///<summary>坐标轴线宽</summary>
		public int AxisLineWidth
		{
			get { return m_Core.AxisLineWidth; }
			set { m_Core.AxisLineWidth = value; }
		}

		///<summary>水平缩放率</summary>
		public double ScaleX
		{
			get { return m_Core.ScaleX; }
			set { m_Core.ScaleX = value; }
		}

		///<summary>水平缩放率最大值</summary>
		public double MaxScaleX
		{
			get { return m_Core.MaxScaleX; }
			set { m_Core.MaxScaleX = value; }
		}

		///<summary>水平缩放率最小值</summary>
		public double MinScaleX
		{
			get { return m_Core.MinScaleX; }
		}

		///<summary>垂直缩放率</summary>
		public double ScaleY
		{
			get { return m_Core.ScaleY; }
			set { m_Core.ScaleY = value; }
		}

		///<summary>垂直缩放率最大值</summary>
		public double MaxScaleY
		{
			get { return m_Core.MaxScaleY; }
			set { m_Core.MaxScaleY = value; }
		}

		///<summary>垂直缩放率最小值</summary>
		public double MinScaleY
		{
			get { return m_Core.MinScaleY; }
		}

		///<summary>水平位移值</summary>
		public double TranslateX
		{
			get { return m_Core.TranslateX; }
			set { m_Core.TranslateX = value; }
		}

		///<summary>水平位移值最大值</summary>
		public double MaxTranslateX
		{
			get { return m_Core.MaxTranslateX; }
		}

		///<summary>水平位移值最小值</summary>
		public double MinTranslateX
		{
			get { return m_Core.MinTranslateX; }
		}

		///<summary>垂直位移值</summary>
		public double TranslateY
		{
			get { return m_Core.TranslateY; }
			set { m_Core.TranslateY = value; }
		}

		///<summary>垂直位移值最大值</summary>
		public double MaxTranslateY
		{
			get { return m_Core.MaxTranslateY; }
		}

		///<summary>垂直位移值最小值</summary>
		public double MinTranslateY
		{
			get { return m_Core.MinTranslateY; }
		}

		///<summary>刷新模式（false - 扫描 | true - 滚动）</summary>
		public Enum_RefreshMode RefreshMode
		{
			get { return m_Core.RefreshMode ? Enum_RefreshMode.Scrolling : Enum_RefreshMode.Scanning; }
			set { m_Core.RefreshMode = value == Enum_RefreshMode.Scrolling; }
		}

		///<summary>数据量</summary>
		private int m_DataLength = 0;
		///<summary>数据量</summary>
		public int DataLength
		{
			get { return m_DataLength; }
			set
			{
				if (IsFrozen) throw new InvalidOperationException();
				m_DataLength = value;
			}
		}

		///<summary>量程</summary>
		private double m_Range = 0;
		///<summary>量程（例如：[-50,50]的量程为100）</summary>
		public double Range
		{
			get { return m_Range; }
			set
			{
				if (IsFrozen) throw new InvalidOperationException();
				m_Range = value;
			}
		}

		///<summary>横轴刻度的数量</summary>
		private int m_AxisTicksCountX = 0;
		///<summary>横轴刻度的数量（不包含原点刻度，即总刻度数量为此设定值加1。此值为0时不绘制刻度。）</summary>
		public int AxisTicksCountX
		{
			get { return m_AxisTicksCountX; }
			set
			{
				if (IsFrozen) throw new InvalidOperationException();
				m_AxisTicksCountX = value;
			}
		}

		///<summary>横轴刻度的尺寸</summary>
		private int m_AxisTickSizeX = 0;
		///<summary>横轴刻度的尺寸</summary>
		public int AxisTickSizeX
		{
			get { return m_AxisTickSizeX; }
			set
			{
				if (IsFrozen) throw new InvalidOperationException();
				m_AxisTickSizeX = value;
			}
		}

		///<summary>横轴尺寸</summary>
		private int m_AxisSizeX = 0;
		///<summary>横轴尺寸（此数值如果小于刻度的高度，则将绘制在图形上面，如大于则差值为与图形之间的边距。）</summary>
		public int AxisSizeX
		{
			get { return m_AxisSizeX; }
			set
			{
				if (IsFrozen) throw new InvalidOperationException();
				m_AxisSizeX = value;
			}
		}

		///<summary>横轴位置</summary>
		private Enum_AxisPlacement_X m_AxisPlacementX = Enum_AxisPlacement_X.Bottom;
		///<summary>横轴位置</summary>
		public Enum_AxisPlacement_X AxisPlacementX
		{
			get { return m_AxisPlacementX; }
			set
			{
				if (IsFrozen) throw new InvalidOperationException();
				m_AxisPlacementX = value;
			}
		}

		///<summary>纵轴刻度的数量</summary>
		private int m_AxisTicksCountY = 0;
		///<summary>纵轴刻度的数量（不包含原点刻度，即总刻度数量为此设定值加1。此值为0时不绘制刻度。）</summary>
		public int AxisTicksCountY
		{
			get { return m_AxisTicksCountY; }
			set
			{
				if (IsFrozen) throw new InvalidOperationException();
				m_AxisTicksCountY = value;
			}
		}

		///<summary>纵轴刻度的尺寸</summary>
		private int m_AxisTickSizeY = 0;
		///<summary>纵轴刻度的尺寸</summary>
		public int AxisTickSizeY
		{
			get { return m_AxisTickSizeY; }
			set
			{
				if (IsFrozen) throw new InvalidOperationException();
				m_AxisTickSizeY = value;
			}
		}

		///<summary>纵轴尺寸</summary>
		private int m_AxisSizeY = 0;
		///<summary>纵轴尺寸（此数值如果小于刻度的高度，则将绘制在图形上面，如大于则差值为与图形之间的边距。）</summary>
		public int AxisSizeY
		{
			get { return m_AxisSizeY; }
			set
			{
				if (IsFrozen) throw new InvalidOperationException();
				m_AxisSizeY = value;
			}
		}

		///<summary>纵轴位置</summary>
		private Enum_AxisPlacement_Y m_AxisPlacementY = Enum_AxisPlacement_Y.Left;
		///<summary>纵轴位置</summary>
		public Enum_AxisPlacement_Y AxisPlacementY
		{
			get { return m_AxisPlacementY; }
			set
			{
				if (IsFrozen) throw new InvalidOperationException();
				m_AxisPlacementY = value;
			}
		}

	}

}