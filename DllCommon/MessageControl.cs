using System.Windows;
using System.Windows.Controls;

namespace Common
{

	///<summary>消息控件</summary>
	public class MessageControl : Control
	{

		///<summary>消息控件</summary>
		static MessageControl()
		{
			DefaultStyleKeyProperty.OverrideMetadata(typeof(MessageControl), new FrameworkPropertyMetadata(typeof(MessageControl)));
		}

		///<summary>文本</summary>
		public static readonly DependencyProperty TextProperty = DependencyProperty.Register("Text", typeof(string), typeof(MessageControl), new FrameworkPropertyMetadata(null, FrameworkPropertyMetadataOptions.SubPropertiesDoNotAffectRender));
		///<summary>文本</summary>
		public string Text
		{
			get { return (string)GetValue(TextProperty); }
			set { SetValue(TextProperty, value); }
		}

		///<summary>标签</summary>
		public static readonly DependencyProperty LabelProperty = DependencyProperty.Register("Label", typeof(string), typeof(MessageControl), new FrameworkPropertyMetadata(null, FrameworkPropertyMetadataOptions.SubPropertiesDoNotAffectRender));
		///<summary>标签</summary>
		public string Label
		{
			get { return (string)GetValue(LabelProperty); }
			set { SetValue(LabelProperty, value); }
		}

		///<summary>当加载模板后</summary>
		public override void OnApplyTemplate()
		{
			try
			{
				var button = GetTemplateChild("BtnClear") as Button;
				if (button != null) button.Click += OnButtonClearClick;
			}
			catch { }
		}

		///<summary>当点击清空按钮时</summary>
		private void OnButtonClearClick(object sender, RoutedEventArgs e)
		{
			Text = null;
		}

	}

}