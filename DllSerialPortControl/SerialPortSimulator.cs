using System;
using System.Threading;
using DllInterface;

namespace SerialPort
{

	///<summary>串口模拟器</summary>
	sealed class SerialPortSimulator
	{

		///<summary>数据产生器</summary>
		private readonly IDataGenerator<byte> m_Generator;

		///<summary>数据处理程序</summary>
		private readonly IDataHandler m_Handler;

		///<summary>计时器</summary>
		private readonly Timer m_Timer;

		///<summary>串口数据模拟器</summary>
		internal SerialPortSimulator(IDataGenerator<byte> generator, IDataHandler handler)
		{
			m_Generator = generator;
			m_Handler = handler;
			m_Timer = new Timer(new TimerCallback(Push), null, 0, (int)(Math.Round(1000.0 / generator.RefreshRate())));
		}

		///<summary>释放</summary>
		internal void Dispose()
		{
			if (m_Timer != null) m_Timer.Dispose();
		}

		///<summary>存入数据</summary>
		private void Push(object obj)
		{
			try
			{
				var times = m_Generator.DataLengthPerTime();
				for (var index = 0u; index != times; ++index) m_Handler.Push(m_Generator.CreateData());
			}
			catch { }
		}

	}

}