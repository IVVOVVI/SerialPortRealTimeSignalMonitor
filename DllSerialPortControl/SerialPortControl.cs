using System.IO.Ports;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Media;
using DllInterface;

namespace SerialPort
{

	///<summary>串口控件</summary>
	public class SerialPortControl : Control, ISerialPortDevice
	{

		///<summary>串口控件</summary>
		static SerialPortControl()
        {
            DefaultStyleKeyProperty.OverrideMetadata(typeof(SerialPortControl), new FrameworkPropertyMetadata(typeof(SerialPortControl)));
        }

		///<summary>资源</summary>
		public static readonly DependencyProperty ResourcesProperty = DependencyProperty.Register("Resources", typeof(ResourceDictionary), typeof(SerialPortControl), new FrameworkPropertyMetadata(null, FrameworkPropertyMetadataOptions.SubPropertiesDoNotAffectRender));
		///<summary>资源</summary>
		public new ResourceDictionary Resources
		{
			get { return (ResourceDictionary)GetValue(ResourcesProperty); }
			set { SetValue(ResourcesProperty, value); }
		}

		///<summary>标签颜色</summary>
		public static readonly DependencyProperty LabelColorProperty = DependencyProperty.Register("LabelColor", typeof(Brush), typeof(SerialPortControl), new FrameworkPropertyMetadata(null, FrameworkPropertyMetadataOptions.SubPropertiesDoNotAffectRender));
		///<summary>标签颜色</summary>
		public Brush LabelColor
		{
			get { return (Brush)GetValue(LabelColorProperty); }
			set { SetValue(LabelColorProperty, value); }
		}

		///<summary>端口名集合</summary>
		public static readonly DependencyProperty PortNamesProperty = DependencyProperty.Register("PortNames", typeof(string[]), typeof(SerialPortControl), new FrameworkPropertyMetadata(null, FrameworkPropertyMetadataOptions.SubPropertiesDoNotAffectRender));
		///<summary>端口名集合</summary>
		public string[] PortNames
		{
			get { return (string[])GetValue(PortNamesProperty); }
			private set { SetValue(PortNamesProperty, value); }
		}

		///<summary>消息</summary>
		public static readonly DependencyProperty MessageProperty = DependencyProperty.Register("Message", typeof(string), typeof(SerialPortControl), new FrameworkPropertyMetadata(null, FrameworkPropertyMetadataOptions.SubPropertiesDoNotAffectRender));
		///<summary>消息</summary>
		public string Message
		{
			get { return (string)GetValue(MessageProperty); }
			private set { SetValue(MessageProperty, value); }
		}

		///<summary>是否开启</summary>
		public static readonly DependencyProperty IsOnProperty = DependencyProperty.Register("IsOn", typeof(bool), typeof(SerialPortControl), new FrameworkPropertyMetadata(false, FrameworkPropertyMetadataOptions.BindsTwoWayByDefault | FrameworkPropertyMetadataOptions.SubPropertiesDoNotAffectRender, new PropertyChangedCallback((DependencyObject d, DependencyPropertyChangedEventArgs e) =>
		{
			if (e.NewValue != null)
			{
				var sender = d as SerialPortControl;
				if (sender != null && sender.m_IsPropertyEnabled && sender.m_Device != null)
				{
					var oldValue = e.OldValue == null ? false : (bool)e.OldValue;
					var newValue = (bool)e.NewValue;
					if (newValue != oldValue)
					{
						if (newValue)
						{
							sender.m_Device.Start();
						}
						else
						{
							sender.m_Device.Stop();
						}
					}
				}
			}
		})));
		///<summary>是否开启</summary>
		public bool IsOn
		{
			get { return (bool)GetValue(IsOnProperty); }
			set { SetValue(IsOnProperty, value); }
		}

		///<summary>数据处理程序</summary>
		public static readonly DependencyProperty HandlerProperty = DependencyProperty.Register("Handler", typeof(object), typeof(SerialPortControl), new FrameworkPropertyMetadata(null, FrameworkPropertyMetadataOptions.BindsTwoWayByDefault | FrameworkPropertyMetadataOptions.SubPropertiesDoNotAffectRender, new PropertyChangedCallback((DependencyObject d, DependencyPropertyChangedEventArgs e) =>
		{
			if (e.NewValue != null)
			{
				var sender = d as SerialPortControl;
				if (sender != null && sender.m_IsPropertyEnabled && sender.m_Device != null)
				{
					var oldValue = e.OldValue as IDataHandler;
					var newValue = e.NewValue as IDataHandler;
					if (newValue != oldValue) sender.m_Device.Handler = newValue;
				}
			}
		})));
		///<summary>数据处理程序</summary>
		public IDataHandler Handler
		{
			get { return GetValue(HandlerProperty) as IDataHandler; }
			set { SetValue(HandlerProperty, value); }
		}

		///<summary>供串口模拟器使用的数据产生器</summary>
		public static readonly DependencyProperty SimulatorDataGeneratorProperty = DependencyProperty.Register("SimulatorDataGenerator", typeof(object), typeof(SerialPortControl), new FrameworkPropertyMetadata(null, FrameworkPropertyMetadataOptions.BindsTwoWayByDefault | FrameworkPropertyMetadataOptions.SubPropertiesDoNotAffectRender, new PropertyChangedCallback((DependencyObject d, DependencyPropertyChangedEventArgs e) =>
		{
			if (e.NewValue != null)
			{
				var sender = d as SerialPortControl;
				if (sender != null && sender.m_IsPropertyEnabled && sender.m_Device != null)
				{
					var oldValue = e.OldValue as IDataGenerator<byte>;
					var newValue = e.NewValue as IDataGenerator<byte>;
					if (newValue != oldValue) sender.m_Device.SimulatorDataGenerator = newValue;
				}
			}
		})));
		///<summary>供串口模拟器使用的数据产生器</summary>
		public IDataGenerator<byte> SimulatorDataGenerator
		{
			get { return GetValue(SimulatorDataGeneratorProperty) as IDataGenerator<byte>; }
			set { SetValue(SimulatorDataGeneratorProperty, value); }
		}

		///<summary>是否使用模拟器</summary>
		public static readonly DependencyProperty IsSimulatorProperty = DependencyProperty.Register("IsSimulator", typeof(bool), typeof(SerialPortControl), new FrameworkPropertyMetadata(false, FrameworkPropertyMetadataOptions.BindsTwoWayByDefault | FrameworkPropertyMetadataOptions.SubPropertiesDoNotAffectRender, new PropertyChangedCallback((DependencyObject d, DependencyPropertyChangedEventArgs e) =>
		{
			if (e.NewValue != null)
			{
				var sender = d as SerialPortControl;
				if (sender != null && sender.m_IsPropertyEnabled && sender.m_Device != null)
				{
					var oldValue = e.OldValue == null ? false : (bool)e.OldValue;
					var newValue = (bool)e.NewValue;
					if (newValue != oldValue) sender.m_Device.IsSimulator = newValue;
				}
			}
		})));
		///<summary>是否使用模拟器</summary>
		public bool IsSimulator
		{
			get { return (bool)GetValue(IsSimulatorProperty); }
			set { SetValue(IsSimulatorProperty, value); }
		}

		///<summary>端口序号</summary>
		public static readonly DependencyProperty PortIndexProperty = DependencyProperty.Register("PortIndex", typeof(int), typeof(SerialPortControl), new FrameworkPropertyMetadata(-1, FrameworkPropertyMetadataOptions.BindsTwoWayByDefault | FrameworkPropertyMetadataOptions.SubPropertiesDoNotAffectRender, new PropertyChangedCallback((DependencyObject d, DependencyPropertyChangedEventArgs e) =>
		{
			if (e.NewValue != null)
			{
				var sender = d as SerialPortControl;
				if (sender != null && sender.m_IsPropertyEnabled && sender.m_Device != null)
				{
					var oldValue = e.OldValue == null ? -1 : (int)e.OldValue;
					var newValue = (int)e.NewValue;
					if (newValue != oldValue) sender.m_Device.PortID = newValue;
				}
			}
		})));
		///<summary>端口序号</summary>
		public int PortIndex
		{
			get { return (int)GetValue(PortIndexProperty); }
			set { SetValue(PortIndexProperty, value); }
		}

		///<summary>波特率文本</summary>
		public static readonly DependencyProperty BaudRateStringProperty = DependencyProperty.Register("BaudRateString", typeof(string), typeof(SerialPortControl), new FrameworkPropertyMetadata(null, FrameworkPropertyMetadataOptions.BindsTwoWayByDefault | FrameworkPropertyMetadataOptions.SubPropertiesDoNotAffectRender, new PropertyChangedCallback((DependencyObject d, DependencyPropertyChangedEventArgs e) =>
		{
			if (e.OldValue != null)
			{
				var newValue = e.NewValue as string;
				if (newValue != null)
				{
					var sender = d as SerialPortControl;
					if (sender != null)
					{
						var oldValue = e.OldValue as string;
						int value;
						if (newValue != oldValue && int.TryParse(newValue, out value))
						{
							sender.BaudRate = value;
							return;
						}
					}
				}
				d.SetValue(e.Property, e.OldValue);
			}
		})));
		///<summary>波特率文本</summary>
		public string BaudRateString
		{
			get { return (string)GetValue(BaudRateStringProperty); }
			set { SetValue(BaudRateStringProperty, value); }
		}

		///<summary>波特率</summary>
		public static readonly DependencyProperty BaudRateProperty = DependencyProperty.Register("BaudRate", typeof(int), typeof(SerialPortControl), new FrameworkPropertyMetadata(19200, FrameworkPropertyMetadataOptions.BindsTwoWayByDefault | FrameworkPropertyMetadataOptions.SubPropertiesDoNotAffectRender, new PropertyChangedCallback((DependencyObject d, DependencyPropertyChangedEventArgs e) =>
		{
			if (e.NewValue != null)
			{
				var sender = d as SerialPortControl;
				if (sender != null)
				{
					var oldValue = e.OldValue == null ? -1 : (int)e.OldValue;
					var newValue = (int)e.NewValue;
					if (newValue != oldValue)
					{
						if (sender.m_IsPropertyEnabled && sender.m_Device != null) sender.m_Device.BaudRate = newValue;
						sender.BaudRateString = newValue.ToString();
					}
				}
			}
		})));
		///<summary>波特率</summary>
		public int BaudRate
		{
			get { return (int)GetValue(BaudRateProperty); }
			set { SetValue(BaudRateProperty, value); }
		}

		///<summary>奇偶校验类型</summary>
		public static readonly DependencyProperty ParityProperty = DependencyProperty.Register("Parity", typeof(int), typeof(SerialPortControl), new FrameworkPropertyMetadata(1, FrameworkPropertyMetadataOptions.BindsTwoWayByDefault | FrameworkPropertyMetadataOptions.SubPropertiesDoNotAffectRender, new PropertyChangedCallback((DependencyObject d, DependencyPropertyChangedEventArgs e) =>
		{
			if (e.NewValue != null)
			{
				var sender = d as SerialPortControl;
				if (sender != null && sender.m_IsPropertyEnabled && sender.m_Device != null)
				{
					var oldValue = e.OldValue == null ? 1 : (int)e.OldValue;
					var newValue = (int)e.NewValue;
					if (newValue != oldValue) sender.m_Device.Parity = (Parity)newValue;
				}
			}
		})));
		///<summary>奇偶校验类型</summary>
		public int Parity
		{
			get { return (int)GetValue(ParityProperty); }
			set { SetValue(ParityProperty, value); }
		}

		///<summary>停止位</summary>
		public static readonly DependencyProperty StopBitsProperty = DependencyProperty.Register("StopBits", typeof(int), typeof(SerialPortControl), new FrameworkPropertyMetadata(1, FrameworkPropertyMetadataOptions.BindsTwoWayByDefault | FrameworkPropertyMetadataOptions.SubPropertiesDoNotAffectRender, new PropertyChangedCallback((DependencyObject d, DependencyPropertyChangedEventArgs e) =>
		{
			if (e.NewValue != null)
			{
				var sender = d as SerialPortControl;
				if (sender != null && sender.m_IsPropertyEnabled && sender.m_Device != null)
				{
					var oldValue = e.OldValue == null ? 1 : (int)e.OldValue;
					var newValue = (int)e.NewValue;
					if (newValue != oldValue) sender.m_Device.StopBits = (StopBits)newValue;
				}
			}
		})));
		///<summary>停止位</summary>
		public int StopBits
		{
			get { return (int)GetValue(StopBitsProperty); }
			set { SetValue(StopBitsProperty, value); }
		}

		///<summary>数据长度</summary>
		public static readonly DependencyProperty DataBitsProperty = DependencyProperty.Register("DataBits", typeof(int), typeof(SerialPortControl), new FrameworkPropertyMetadata(0, FrameworkPropertyMetadataOptions.BindsTwoWayByDefault | FrameworkPropertyMetadataOptions.SubPropertiesDoNotAffectRender, new PropertyChangedCallback((DependencyObject d, DependencyPropertyChangedEventArgs e) =>
		{
			if (e.NewValue != null)
			{
				var sender = d as SerialPortControl;
				if (sender != null && sender.m_IsPropertyEnabled && sender.m_Device != null)
				{
					var oldValue = e.OldValue == null ? 0 : (int)e.OldValue;
					var newValue = (int)e.NewValue;
					if (newValue != oldValue) sender.m_Device.DataBits = ConvertDataBits(newValue);
				}
			}
		})));
		///<summary>数据长度</summary>
		public int DataBits
		{
			get { return (int)GetValue(DataBitsProperty); }
			set { SetValue(DataBitsProperty, value); }
		}

		///<summary>转换DataBits</summary>
		private static int ConvertDataBits(int value)
		{
			return (value < 5 || value > 8) ? 8 : (8 - value);
		}

		///<summary>属性是否可用</summary>
		private bool m_IsPropertyEnabled = false;

		///<summary>串口设备</summary>
		private SerialPortDevice m_Device = null;

		///<summary>串口控件</summary>
		public SerialPortControl()
		{
			BaudRateString = BaudRate.ToString();
		}

		///<summary>初始化串口设备</summary>
		public IClose InitializeDevice()
		{
			m_Device = SerialPortDevice.Create(Handler, BaudRate, (Parity)Parity, (StopBits)StopBits, ConvertDataBits(DataBits), SimulatorDataGenerator);
			Initialize();
			return m_Device;
		}

		///<summary>初始化使用默认处理程序的串口设备</summary>
		///<param name="receivers">数据接收器集合</param>
		public IClose InitializeDafaultDevice(IDataReceiver[] receivers)
		{
			m_Device = SerialPortDevice.CreateDafault(receivers, BaudRate, (Parity)Parity, (StopBits)StopBits, ConvertDataBits(DataBits));
			Handler = m_Device.Handler;
			SimulatorDataGenerator = m_Device.SimulatorDataGenerator;
			Initialize();
			return m_Device;
		}

		///<summary>初始化</summary>
		private void Initialize()
		{
			SerialPortDevice.RefreshPortNames();
			SerialPortDevice.PortNamesChanged += OnPortNamesChanged;
			m_Device.StateChanged += OnStateChanged;
			m_Device.Error += OnError;
			PortNames = SerialPortDevice.PortNames;
			m_IsPropertyEnabled = true;
			var length = PortNames.Length;
			if (length > 0)
			{
				if (PortIndex < 0 || PortIndex >= PortNames.Length) PortIndex = 0;
			}
			else
			{
				if (PortIndex >= 0) PortIndex = -1;
				IsSimulator = true;
			}
		}

		///<summary>当端口名集合改变时</summary>
		private void OnPortNamesChanged(string[] e)
		{
			Dispatcher.Invoke(() =>
			{
				var isOn = IsOn;
				var index = PortIndex;
				PortIndex = -1;
				PortNames = e;
				if (e.Length > 0)
				{
					if (index >= 0 && index < e.Length)
					{
						PortIndex = index;
					}
					else
					{
						PortIndex = 0;
					}
				}
				else if (!IsSimulator)
				{
					IsSimulator = true;
				}
				if (isOn) IsOn = true;
			});
		}

		///<summary>当状态改变时</summary>
		private void OnStateChanged(object sender, bool e)
		{
			Dispatcher.Invoke(() =>
			{
				if (IsOn != e)
				{
					m_IsPropertyEnabled = false;
					IsOn = e;
					m_IsPropertyEnabled = true;
				}
			});
		}

		///<summary>当收到错误指示时</summary>
		private void OnError(object sender, SerialPortDevice.ErrorType e)
		{
			switch (e)
			{
				case SerialPortDevice.ErrorType.StartFailed:
					Message = Resources == null ? null : Resources.FindName("Text_StartFailed") as string;
					break;
				case SerialPortDevice.ErrorType.NoError:
					Message = null;
					break;
			}
		}

		///<summary>当加载模板后</summary>
		public override void OnApplyTemplate()
		{
			try
			{
				var button = GetTemplateChild("BtnRefresh") as Button;
				if (button != null) button.Click += OnButtonRefreshClick;
			}
			catch { }
		}

		///<summary>当点击刷新按钮时</summary>
		private void OnButtonRefreshClick(object sender, RoutedEventArgs e)
		{
			SerialPortDevice.RefreshPortNames();
		}

		///<summary>串口</summary>
		System.IO.Ports.SerialPort ISerialPortDevice.SerialPort
		{
			get { return m_Device == null ? null : ((ISerialPortDevice)m_Device).SerialPort; }
		}

	}
}