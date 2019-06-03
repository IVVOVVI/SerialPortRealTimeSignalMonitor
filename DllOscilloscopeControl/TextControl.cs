using System;
using System.Text;
using System.Windows;
using System.Windows.Controls;
using DllInterface;

namespace Oscilloscope
{

	///<summary>文本控件</summary>
	public class TextControl : Control, IDataReceiver
	{

		///<summary>文本控件</summary>
		static TextControl()
		{
			DefaultStyleKeyProperty.OverrideMetadata(typeof(TextControl), new FrameworkPropertyMetadata(typeof(TextControl)));
		}

		///<summary>文本</summary>
		public static readonly DependencyProperty TextProperty = DependencyProperty.Register("Text", typeof(string), typeof(TextControl), new FrameworkPropertyMetadata(null, FrameworkPropertyMetadataOptions.SubPropertiesDoNotAffectRender));
		///<summary>文本</summary>
		public string Text
		{
			get { return (string)GetValue(TextProperty); }
			private set { SetValue(TextProperty, value); }
		}

		///<summary>标签</summary>
		public static readonly DependencyProperty LabelProperty = DependencyProperty.Register("Label", typeof(string), typeof(TextControl), new FrameworkPropertyMetadata(null, FrameworkPropertyMetadataOptions.SubPropertiesDoNotAffectRender));
		///<summary>标签</summary>
		public string Label
		{
			get { return (string)GetValue(LabelProperty); }
			set { SetValue(LabelProperty, value); }
		}

		///<summary>前缀</summary>
		public string Prefix { get; set; } = null;

		///<summary>后缀</summary>
		public string Suffix { get; set; } = null;

		///<summary>字符串</summary>
		private readonly StringBuilder m_StringBuilder = new StringBuilder();

		///<summary>小数位数</summary>
		private int Digits { get; set; } = 3;

		///<summary>滚动区</summary>
		private ScrollViewer m_ScrollViewer = null;

		///<summary>安全替换全部数据（并行安全）</summary>
		///<param name="value">值</param>
		public void SafeReplace(float[] value)
		{
			Replace(value);
		}

		///<summary>替换全部数据</summary>
		///<param name="value">值</param>
		public void Replace(float[] value)
		{
			string text;
			lock (m_StringBuilder)
			{
				m_StringBuilder.Clear();
				if (Prefix != null)
				{
					if (Suffix != null)
					{
						foreach (var temp in value)
						{
							m_StringBuilder.Append(Prefix);
							m_StringBuilder.Append(Math.Round(temp, Digits).ToString());
							m_StringBuilder.AppendLine(Suffix);
						}
					}
					else
					{
						foreach (var temp in value)
						{
							m_StringBuilder.Append(Prefix);
							m_StringBuilder.AppendLine(Math.Round(temp, Digits).ToString());
						}
					}
				}
				else
				{
					if (Suffix != null)
					{
						foreach (var temp in value)
						{
							m_StringBuilder.Append(Math.Round(temp, Digits).ToString());
							m_StringBuilder.AppendLine(Suffix);
						}
					}
					else
					{
						foreach (var temp in value) m_StringBuilder.AppendLine(Math.Round(temp, Digits).ToString());
					}
				}
				text = m_StringBuilder.ToString();
			}
			Dispatcher.Invoke(() =>
			{
				Text = text;
				if (m_ScrollViewer != null) m_ScrollViewer.ScrollToEnd();
			});
		}

		///<summary>安全存入数据（并行安全）</summary>
		///<param name="value">值</param>
		public void SafePush(float value)
		{
			Push(value);
		}

		///<summary>存入数据</summary>
		///<param name="value">值</param>
		public void Push(float value)
		{
			string text;
			lock (m_StringBuilder)
			{
				if (Prefix != null) m_StringBuilder.Append(Prefix);
				m_StringBuilder.Append(Math.Round(value, Digits).ToString());
				if (Suffix != null) m_StringBuilder.Append(Suffix);
				m_StringBuilder.AppendLine();
				text = m_StringBuilder.ToString();
			}
			Dispatcher.Invoke(() =>
			{
				Text = text;
				if (m_ScrollViewer != null) m_ScrollViewer.ScrollToEnd();
			});
		}

		///<summary>安全清空</summary>
		public void SafeClear()
		{
			Clear();
		}

		///<summary>清空</summary>
		public void Clear()
		{
			lock (m_StringBuilder)
			{
				m_StringBuilder.Clear();
			}
			Dispatcher.Invoke(() => { Text = null; });
		}

		///<summary>当加载模板后</summary>
		public override void OnApplyTemplate()
		{
			try
			{
				m_ScrollViewer = GetTemplateChild("ScrollViewer") as ScrollViewer;
				if (m_ScrollViewer != null) m_ScrollViewer.ScrollToEnd();
				var button = GetTemplateChild("BtnClear") as Button;
				if (button != null) button.Click += OnButtonClearClick;
			}
			catch { }
		}

		///<summary>当点击清空按钮时</summary>
		private void OnButtonClearClick(object sender, RoutedEventArgs e)
		{
			Clear();
		}

		///<summary>关闭</summary>
		void IClose.Close() { }

	}

}