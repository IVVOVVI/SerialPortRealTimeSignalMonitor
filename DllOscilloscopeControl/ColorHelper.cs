namespace Oscilloscope
{

	///<summary>颜色辅助类</summary>
	public static class ColorHelper
	{

		///<summary>创建颜色</summary>
		///<param name="color">ARGB</param>
		public static System.Windows.Media.Color ToColor(uint color)
		{
			return System.Windows.Media.Color.FromArgb((byte)((color >> 24) & 0xFFu), (byte)((color >> 16) & 0xFFu), (byte)((color >> 8) & 0xFFu), (byte)(color & 0xFFu));
		}

		///<summary>创建颜色</summary>
		///<param name="color">ARGB</param>
		public static uint ToColorValue(System.Windows.Media.Color color)
		{
			return (uint)(color.A << 24) + (uint)(color.R << 16) + (uint)(color.G << 8) + color.B;
		}

		///<summary>创建颜色</summary>
		///<param name="color">ARGB</param>
		public static System.Drawing.Color FromArgb(uint color)
		{
			return System.Drawing.Color.FromArgb((int)color);
		}

		///<summary>创建颜色</summary>
		///<param name="brush">笔刷</param>
		///<param name="defaultColor">默认颜色</param>
		public static uint GetColor(System.Windows.Media.Brush brush, uint defaultColor)
		{
			var color = brush as System.Windows.Media.SolidColorBrush;
			return color != null ? ToColorValue(color.Color) : defaultColor;
		}

		///<summary>创建颜色</summary>
		///<param name="brush">笔刷</param>
		///<param name="defaultColor">默认颜色</param>
		public static uint GetColor(System.Windows.Media.SolidColorBrush brush, uint defaultColor)
		{
			return brush != null ? ToColorValue(brush.Color) : defaultColor;
		}

	}

}