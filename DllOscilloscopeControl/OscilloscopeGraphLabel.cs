using System.Windows;
using System.Windows.Controls.Primitives;

namespace Oscilloscope
{

	///<summary>示波器图形标签</summary>
	public class OscilloscopeGraphLabel : ToggleButton
	{

		///<summary>示波器图形标签</summary>
		static OscilloscopeGraphLabel()
		{
			DefaultStyleKeyProperty.OverrideMetadata(typeof(OscilloscopeGraphLabel), new FrameworkPropertyMetadata(typeof(OscilloscopeGraphLabel)));
		}

	}

}