using System;
using System.Windows.Forms;
using DllInterface;

namespace Oscilloscope
{

	///<summary>窗口</summary>
	public class Window : Form, IWindow
	{

		///<summary>示波器组</summary>
		public IntPtr Oscilloscopes { get; private set; }

		///<summary>关闭</summary>
		public IClose Closeable { get; set; } = null;

		///<summary>示波器窗口</summary>
		public Window()
		{
			Oscilloscopes = Dll.CreateOscilloscopes(Handle, FPS());
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