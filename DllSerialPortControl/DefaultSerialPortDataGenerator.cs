using DllInterface;

namespace SerialPort
{

	///<summary>默认串口数据产生器</summary>
	public class DefaultSerialPortDataGenerator : IDataGenerator<byte>
	{

		///<summary>刷新率（最大值60，且不能小于等于0。）</summary>
		public double RefreshRate()
		{
			return 60.0;
		}

		///<summary>每次的数据量</summary>
		public uint DataLengthPerTime()
		{
			return 150u;
		}

		///<summary>创建数据</summary>
		public byte CreateData()
		{
			return m_Value += 7;
		}

		///<summary>数据</summary>
		private byte m_Value;

	}

}