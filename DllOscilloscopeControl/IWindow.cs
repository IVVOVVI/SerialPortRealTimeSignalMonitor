using System;

namespace Oscilloscope
{

	///<summary>窗口</summary>
	public interface IWindow
	{

		///<summary>示波器组</summary>
		IntPtr Oscilloscopes { get; }

	}

}