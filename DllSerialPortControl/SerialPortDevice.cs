using System;
using System.IO.Ports;
using System.Threading;
using DllInterface;

namespace SerialPort
{

	///<summary>端口名集合改变回调</summary>
	public delegate void PortNamesChangedEventHandler(string[] e);

	///<summary>串口设备</summary>
	class SerialPortDevice : ISerialPortDevice, IDataSource
	{

		///<summary>端口名集合改变【注意：此事件可能从辅助线程触发】</summary>
		public static event PortNamesChangedEventHandler PortNamesChanged;

		///<summary>端口名集合</summary>
		public static string[] PortNames { get; private set; } = null;

		///<summary>获取端口名</summary>
		///<param name="index">端口序号</param>
		public static string GetPortName(int index)
		{
			return (index >= 0 && PortNames != null && index < PortNames.Length) ? PortNames[index] : null;
		}

		///<summary>刷新端口名集合</summary>
		public static void RefreshPortNames()
		{
			try
			{
				var names = System.IO.Ports.SerialPort.GetPortNames();
				if (ComparerPortNames(names))
				{
					PortNames = names;
					PortNamesChanged?.Invoke(names);
				}
			}
			catch { }
		}

		///<summary>比较端口名集合</summary>
		///<param name="names">端口名集合</param>
		private static bool ComparerPortNames(string[] names)
		{
			if (names == null) return PortNames != null;
			if (PortNames == null) return true;
			var length = names.Length;
			if (length != PortNames.Length) return true;
			for (var index = 0; index != length; ++index) if (names[index] != PortNames[index]) return true;
			return false;
		}

		///<summary>创建串口设备</summary>
		///<param name="handler">数据处理程序</param>
		///<param name="baudRate">波特率</param>
		///<param name="parity">奇偶校验类型</param>
		///<param name="stopBits">停止位</param>
		///<param name="dataBits">数据长度</param>
		///<param name="simulatorDataGenerator">供串口模拟器使用的数据产生器</param>
		public static SerialPortDevice Create(IDataHandler handler, int baudRate = 19200, Parity parity = Parity.Odd, StopBits stopBits = StopBits.One, int dataBits = 8, IDataGenerator<byte> simulatorDataGenerator = null)
		{
			return new SerialPortDevice(handler, baudRate, parity, stopBits, dataBits, simulatorDataGenerator);
		}

		///<summary>创建使用默认处理程序的串口设备</summary>
		///<param name="receivers">数据接收器集合</param>
		///<param name="baudRate">波特率</param>
		///<param name="parity">奇偶校验类型</param>
		///<param name="stopBits">停止位</param>
		///<param name="dataBits">数据长度</param>
		public static SerialPortDevice CreateDafault(IDataReceiver[] receivers, int baudRate = 19200, Parity parity = Parity.Odd, StopBits stopBits = StopBits.One, int dataBits = 8)
		{
			return new SerialPortDevice(new DefaultSerialPortDataHandler(receivers), baudRate, parity, stopBits, dataBits, new DefaultSerialPortDataGenerator());
		}

		///<summary>串口设备</summary>
		///<param name="handler">数据处理程序</param>
		///<param name="baudRate">波特率</param>
		///<param name="parity">奇偶校验类型</param>
		///<param name="stopBits">停止位</param>
		///<param name="dataBits">数据长度</param>
		///<param name="simulatorDataGenerator">供串口模拟器使用的数据产生器</param>
		private SerialPortDevice(IDataHandler handler, int baudRate, Parity parity, StopBits stopBits, int dataBits, IDataGenerator<byte> simulatorDataGenerator)
		{
			m_Handler = handler;
			m_SimulatorDataGenerator = simulatorDataGenerator;
			m_SerialPort = new System.IO.Ports.SerialPort();
			m_SerialPort.ReadTimeout = System.IO.Ports.SerialPort.InfiniteTimeout; //禁止超时
			BaudRate = baudRate;
			Parity = parity;
			StopBits = stopBits;
			DataBits = dataBits;
		}

		///<summary>错误类型</summary>
		public enum ErrorType { StartFailed, NoError }

		///<summary>错误</summary>
		public event EventHandler<ErrorType> Error;

		///<summary>状态改变（true - 开始 | false - 停止）【注意：此事件可能从辅助线程触发】</summary>
		public event EventHandler<bool> StateChanged;

		///<summary>串口</summary>
		System.IO.Ports.SerialPort ISerialPortDevice.SerialPort { get { return m_SerialPort; } }

		///<summary>数据处理程序</summary>
		private readonly System.IO.Ports.SerialPort m_SerialPort;

		///<summary>串口模拟器</summary>
		private SerialPortSimulator m_SerialPortSimulator = null;

		///<summary>数据处理程序</summary>
		private IDataHandler m_Handler = null;

		///<summary>供串口模拟器使用的数据产生器</summary>
		private IDataGenerator<byte> m_SimulatorDataGenerator = null;

		///<summary>是否使用模拟器</summary>
		private bool m_IsSimulator = false;

		///<summary>端口序号</summary>
		private int m_PortID = -1;

		///<summary>窗口读取线程</summary>
		private Thread m_ReadThread = null;

		///<summary>是否已停止</summary>
		private bool m_IsStopped = true;

		///<summary>数据处理程序</summary>
		public IDataHandler Handler
		{
			get { return m_Handler; }
			set { if (m_Handler != value) ChangeProperty(() => { m_Handler = value; }); }
		}

		///<summary>供串口模拟器使用的数据产生器</summary>
		public IDataGenerator<byte> SimulatorDataGenerator
		{
			get { return m_SimulatorDataGenerator; }
			set { if (m_SimulatorDataGenerator != value) ChangeProperty(() => { m_SimulatorDataGenerator = value; }); }
		}

		///<summary>是否使用模拟器</summary>
		public bool IsSimulator
		{
			get { return m_IsSimulator; }
			set { if (m_IsSimulator != value) ChangeProperty(() => { m_IsSimulator = value; }); }
		}

		///<summary>端口序号</summary>
		public int PortID
		{
			get { return m_PortID; }
			set { if (m_PortID != value) ChangeSerialPortProperty(() => { m_PortID = value; }); }
		}

		///<summary>波特率</summary>
		public int BaudRate
		{
			get { return m_SerialPort.BaudRate; }
			set { if (m_SerialPort.BaudRate != value) ChangeSerialPortProperty(() => { m_SerialPort.BaudRate = value; }); }
		}

		///<summary>奇偶校验类型</summary>
		public Parity Parity
		{
			get { return m_SerialPort.Parity; }
			set { if (m_SerialPort.Parity != value) ChangeSerialPortProperty(() => { m_SerialPort.Parity = value; }); }
		}

		///<summary>停止位</summary>
		public StopBits StopBits
		{
			get { return m_SerialPort.StopBits; }
			set { if (m_SerialPort.StopBits != value) ChangeSerialPortProperty(() => { m_SerialPort.StopBits = value; }); }
		}

		///<summary>数据长度</summary>
		public int DataBits
		{
			get { return m_SerialPort.DataBits; }
			set { if (value >= 5 && value <= 8 && m_SerialPort.DataBits != value) ChangeSerialPortProperty(() => { m_SerialPort.DataBits = value; }); }
		}

		///<summary>开始</summary>
		public void Start()
		{
			lock (this)
			{
				if (m_IsStopped)
				{
					if (m_Handler != null)
					{
						if (m_IsSimulator)
						{
							if (m_SimulatorDataGenerator != null)
							{
								m_SerialPortSimulator = new SerialPortSimulator(m_SimulatorDataGenerator, m_Handler);
								m_IsStopped = false;
								TriggerErrorEvent(ErrorType.NoError);
								ChangeState(true);
								return;
							}
						}
						else
						{
							if (PortID < 0)
							{
								ChangeState(false);
								return;
							}
							string name;
							name = GetPortName(PortID);
							if (name != null)
							{
								try
								{
									m_SerialPort.PortName = PortNames[PortID];
									m_SerialPort.Open();
									m_ReadThread = new Thread(Read);
									m_ReadThread.IsBackground = true;
									m_ReadThread.Start();
									m_IsStopped = false;
									TriggerErrorEvent(ErrorType.NoError);
									ChangeState(true);
									return;
								}
								catch { }
							}
						}
					}
					TriggerErrorEvent(ErrorType.StartFailed);
					ChangeState(false);
				}
			}
		}

		///<summary>停止</summary>
		public void Stop()
		{
			Stop(true);
		}

		///<summary>关闭</summary>
		public void Close()
		{
			Stop(false);
			if (m_Handler != null) m_Handler.Close();
		}

		///<summary>停止</summary>
		///<param name="isRefresh">是否刷新</param>
		private void Stop(bool isRefresh)
		{
			lock (this)
			{
				if (!m_IsStopped)
				{
					m_IsStopped = true;
					try
					{
						if (m_IsSimulator)
						{
							if (m_SerialPortSimulator != null)
							{
								m_SerialPortSimulator.Dispose();
								m_SerialPortSimulator = null;
							}
						}
						else
						{
							m_SerialPort.Close();
						}
					}
					catch { }
					if (isRefresh)
					{
						try
						{
							m_Handler.Clear();
						}
						catch { }
						ChangeState(false);
					}
				}
			}
		}

		///<summary>改变属性</summary>
		///<param name="setProperty">属性设置函数</param>
		private void ChangeProperty(Action setPropertyFun)
		{
			var isOpen = !m_IsStopped;
			if (isOpen) Stop();
			setPropertyFun();
			if (isOpen) Start();
		}

		///<summary>改变串口属性</summary>
		///<param name="setProperty">属性设置函数</param>
		private void ChangeSerialPortProperty(Action setPropertyFun)
		{
			var isOpen = m_SerialPort.IsOpen;
			if (isOpen) Stop();
			try
			{
				setPropertyFun();
			}
			catch { }
			if (isOpen) Start();
		}

		///<summary>改变状态</summary>
		///<param name="isStarted">是否已启动</param>
		private void ChangeState(bool isStarted)
		{
			try
			{
				StateChanged?.Invoke(this, isStarted);
			}
			catch { }
		}

		///<summary>触发错误事件</summary>
		///<param name="error">错误类型</param>
		private void TriggerErrorEvent(ErrorType error)
		{
			try
			{
				Error?.Invoke(this, error);
			}
			catch { }
		}

		///<summary>读取</summary>
		private void Read()
		{
			try
			{
				int temp;
				do
				{
					temp = m_SerialPort.ReadByte();
					if (temp >= 0) m_Handler.Push((byte)temp);
				}
				while (true);
			}
			catch
			{
				Stop();
			}
		}

	}

}