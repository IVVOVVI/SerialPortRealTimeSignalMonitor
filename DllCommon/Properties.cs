using System.Windows;
using System.Windows.Media;

namespace Common
{

	///<summary>颜色属性</summary>
	public class Properties : DependencyObject
	{

		///<summary>聚焦时的颜色</summary>
		public static readonly DependencyProperty FocusColorProperty = DependencyProperty.Register("FocusColor", typeof(Brush), typeof(Properties), new FrameworkPropertyMetadata(null, FrameworkPropertyMetadataOptions.SubPropertiesDoNotAffectRender));
		///<summary>聚焦时的颜色</summary>
		public static Brush GetFocusColor(DependencyObject obj)
		{
			return (Brush)obj.GetValue(FocusColorProperty);
		}
		///<summary>聚焦时的颜色</summary>
		public static void SetFocusColor(DependencyObject obj, Brush value)
		{
			obj.SetValue(FocusColorProperty, value);
		}

		///<summary>按下时的颜色</summary>
		public static readonly DependencyProperty PressedColorProperty = DependencyProperty.Register("PressedColor", typeof(Brush), typeof(Properties), new FrameworkPropertyMetadata(null, FrameworkPropertyMetadataOptions.SubPropertiesDoNotAffectRender));
		///<summary>按下时的颜色</summary>
		public static Brush GetPressedColor(DependencyObject obj)
		{
			return (Brush)obj.GetValue(FocusColorProperty);
		}
		///<summary>按下时的颜色</summary>
		public static void SetPressedColor(DependencyObject obj, Brush value)
		{
			obj.SetValue(FocusColorProperty, value);
		}

		///<summary>选中时的颜色</summary>
		public static readonly DependencyProperty CheckedColorProperty = DependencyProperty.Register("CheckedColor", typeof(Brush), typeof(Properties), new FrameworkPropertyMetadata(null, FrameworkPropertyMetadataOptions.SubPropertiesDoNotAffectRender));
		///<summary>选中时的颜色</summary>
		public static Brush GetCheckedColor(DependencyObject obj)
		{
			return (Brush)obj.GetValue(FocusColorProperty);
		}
		///<summary>选中时的颜色</summary>
		public static void SetCheckedColor(DependencyObject obj, Brush value)
		{
			obj.SetValue(FocusColorProperty, value);
		}

		///<summary>圆角</summary>
		public static readonly DependencyProperty CornerRadiusProperty = DependencyProperty.Register("CornerRadius", typeof(CornerRadius), typeof(Properties), new FrameworkPropertyMetadata(new CornerRadius(0.0), FrameworkPropertyMetadataOptions.SubPropertiesDoNotAffectRender));
		///<summary>圆角</summary>
		public static CornerRadius GetCornerRadius(DependencyObject obj)
		{
			return (CornerRadius)obj.GetValue(CornerRadiusProperty);
		}
		///<summary>圆角</summary>
		public static void SetCornerRadius(DependencyObject obj, CornerRadius value)
		{
			obj.SetValue(CornerRadiusProperty, value);
		}

		///<summary>文本</summary>
		public static readonly DependencyProperty TextProperty = DependencyProperty.Register("Text", typeof(string), typeof(Properties), new FrameworkPropertyMetadata(null, FrameworkPropertyMetadataOptions.SubPropertiesDoNotAffectRender));
		///<summary>文本</summary>
		public static string GetText(DependencyObject obj)
		{
			return (string)obj.GetValue(CornerRadiusProperty);
		}
		///<summary>文本</summary>
		public static void SetText(DependencyObject obj, string value)
		{
			obj.SetValue(CornerRadiusProperty, value);
		}

	}

}