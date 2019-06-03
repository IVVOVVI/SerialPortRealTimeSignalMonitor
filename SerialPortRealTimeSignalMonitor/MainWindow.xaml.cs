using DllInterface;
using Oscilloscope;
using Statistics;

namespace SerialPortRealTimeSignalMonitor
{

	///<summary>主窗口</summary>
	public partial class MainWindow : OscilloscopeWindow
	{

		///<summary>主窗口</summary>
		public MainWindow()
		{
			InitializeComponent();
		}

		///<summary>初始化</summary>
		protected override void Initialize()
		{
			SignalMonitor.Initialize(this);
			StatisticsMonitor.Initialize(this);
			FFT.Initialize(this);
			Closeable = ComControl.InitializeDafaultDevice(new IDataReceiver[] { SignalMonitor, new DefaultStatisticsModule(MaxValue, MinValue, AverageValue, SD, true, FFT) });
		}

	}

}