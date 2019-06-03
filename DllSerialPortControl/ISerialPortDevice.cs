namespace SerialPort
{

	///<summary>串口设备</summary>
	public interface ISerialPortDevice
	{

		///<summary>串口</summary>
		System.IO.Ports.SerialPort SerialPort { get; }

	}

}