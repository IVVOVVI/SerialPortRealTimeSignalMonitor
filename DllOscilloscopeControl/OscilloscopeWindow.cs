using System;
using System.Windows;
using System.Windows.Interop;
using DllInterface;

namespace Oscilloscope
{

	///<summary>示波器窗口</summary>
	public class OscilloscopeWindow : System.Windows.Window, IWindow
	{

		///<summary>示波器组</summary>
		public IntPtr Oscilloscopes { get; private set; }

		///<summary>关闭</summary>
		public IClose Closeable { get; set; } = null;

		///<summary>窗口</summary>
		private IntPtr m_Window = IntPtr.Zero;

		///<summary>示波器窗口</summary>
		public OscilloscopeWindow()
		{
			Oscilloscopes = IntPtr.Zero;
		}

		///<summary>当初始化完成时</summary>
		protected override void OnSourceInitialized(EventArgs e)
		{
			base.OnSourceInitialized(e);
			m_Window = (new WindowInteropHelper(this)).Handle;
			Oscilloscopes = Dll.CreateOscilloscopes(m_Window, FPS());
			Initialize();
		}

		///<summary>关闭后</summary>
		protected override void OnClosed(EventArgs e)
		{
			base.OnClosed(e);
			if (Closeable != null) Closeable.Close();
			Release();
			if (Oscilloscopes != IntPtr.Zero) Dll.CloseOscilloscopes(Oscilloscopes);
		}

		///<summary>DPI改变时</summary>
		protected override void OnDpiChanged(DpiScale oldValueDpi, DpiScale newDpi)
		{
			base.OnDpiChanged(oldValueDpi, newDpi);
			if (Oscilloscopes != IntPtr.Zero) Dll.RefreshDPI(Oscilloscopes);
		}

		///<summary>刷新率</summary>
		protected virtual byte FPS()
		{
			return 60;
		}

		///<summary>初始化（重写此函数以初始化窗口中的所有示波器相关控件）</summary>
		protected virtual void Initialize() { }

		///<summary>释放（重写此函数以关闭所有示波器相关控件）</summary>
		protected virtual void Release() { }

	}

}