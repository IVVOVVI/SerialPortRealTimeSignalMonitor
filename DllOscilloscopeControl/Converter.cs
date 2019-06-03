using System;
using System.Globalization;
using System.Windows.Data;

namespace Oscilloscope
{

	[ValueConversion(typeof(double), typeof(string))]
	public sealed class ZoomValueConverter : IValueConverter
	{

		public object Convert(object value, Type targetType, object parameter, CultureInfo culture)
		{
			try
			{
				if (value != null) return (Math.Round((double)value, 2)).ToString() + "×";
			}
			catch { }
			return "-";
		}

		public object ConvertBack(object value, Type targetType, object parameter, CultureInfo culture)
		{
			try
			{
				var temp = (string)value;
				double zoom;
				if (temp != null && double.TryParse(temp, out zoom)) return zoom;
			}
			catch { }
			return 1.0;
		}

	}

}