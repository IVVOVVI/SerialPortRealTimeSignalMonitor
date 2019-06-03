using System;
using System.Windows;

namespace Oscilloscope
{

	///<summary>多信号显示器</summary>
	public class MultipleSignalsMonitor : SignalMonitor
	{

		///<summary>多信号显示器</summary>
		static MultipleSignalsMonitor()
		{
			DefaultStyleKeyProperty.OverrideMetadata(typeof(MultipleSignalsMonitor), new FrameworkPropertyMetadata(typeof(MultipleSignalsMonitor)));
		}

		///<summary>标签</summary>
		public static readonly DependencyProperty LabelsProperty = DependencyProperty.Register("Labels", typeof(UIElement), typeof(MultipleSignalsMonitor), new FrameworkPropertyMetadata(null, FrameworkPropertyMetadataOptions.SubPropertiesDoNotAffectRender));
		///<summary>标签</summary>
		public UIElement Labels
		{
			get { return (UIElement)GetValue(LabelsProperty); }
			set { SetValue(LabelsProperty, value); }
		}

		///<summary>示波器图形集合</summary>
		public OscilloscopeGraphCollection Graphs { get; } = new OscilloscopeGraphCollection();

		///<summary>初始化</summary>
		///<param name="window">窗口</param>
		public void Initialize(IWindow window)
		{
			if (Graphs.IsReadOnly) throw new InvalidOperationException();
			Graphs.IsReadOnly = true;
			var count = Graphs.Count;
			if (count > 0)
			{
				var graphs = new GraphProperties[count];
				var index = 0;
				foreach (var graph in Graphs)
				{
					graph.Initialize(m_Core, (uint)index);
					graphs[index] = new GraphProperties { BaselineType = graph.BaselineType, GraphStyle = graph.GraphStyle, Foreground = ColorHelper.GetColor(graph.Foreground, 0xFF000000u), LineWidth = (uint)graph.LineWidth, Offset = graph.Offset, AutoTranslate = (float)graph.AutoTranslateOffset, IsEnabled = graph.IsGraphEnabled };
					++index;
				}
				var arg = new ConstructionParameters(window, ColorHelper.GetColor(ContentBackground, 0xFFFFFFFFu), (uint)DataLength, (float)Range, (float)ScaleX, (float)MaxScaleX, (float)ScaleY, (float)MaxScaleY, (uint)TranslateX, (uint)TranslateY, RefreshMode, ColorHelper.GetColor(AxisColor, 0xFF000000u), (uint)AxisLineWidth, (uint)AxisTicksCountX, (uint)AxisTicksCountY, (uint)AxisTickSizeX, (uint)AxisTickSizeY, (uint)AxisSizeX, (uint)AxisSizeY, AxisPlacementX, AxisPlacementY, graphs);
				m_Core.Initialize(ref arg);
			}
		}

	}

}