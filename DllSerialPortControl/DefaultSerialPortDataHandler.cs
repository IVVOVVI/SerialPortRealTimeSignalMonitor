using System;
using DllInterface;

namespace SerialPort
{

	///<summary>默认串口数据处理程序</summary>
	public class DefaultSerialPortDataHandler : IDataHandler
	{

		///<summary>数据接收器</summary>
		private readonly IDataReceiver[] m_Receivers;

		///<summary>默认串口数据处理程序</summary>
		///<param name="receiver">数据接收器</param>
		public DefaultSerialPortDataHandler(IDataReceiver receiver)
		{
			if (receiver == null) throw new ArgumentNullException();
			m_Receivers = new IDataReceiver[] { receiver };
		}

		///<summary>默认串口数据处理程序</summary>
		///<param name="receivers">数据接收器</param>
		public DefaultSerialPortDataHandler(IDataReceiver[] receivers)
		{
			if (receivers == null || receivers.Length <= 0) throw new ArgumentNullException();
			m_Receivers = receivers;
		}

		///<summary>安全存入数据</summary>
		///<param name="value">值</param>
		public void SafePush(byte value)
		{
			foreach (var receiver in m_Receivers) receiver.SafePush(value);
		}

		///<summary>存入数据</summary>
		///<param name="value">值</param>
		public void Push(byte value)
		{
			foreach (var receiver in m_Receivers) receiver.Push(value);
		}

		///<summary>安全清空</summary>
		public void SafeClear()
		{
			foreach (var receiver in m_Receivers) receiver.SafeClear();
		}

		///<summary>清空</summary>
		public void Clear()
		{
			foreach (var receiver in m_Receivers) receiver.Clear();
		}

		///<summary>关闭</summary>
		public void Close()
		{
			foreach (var receiver in m_Receivers) receiver.Close();
		}

	}

}