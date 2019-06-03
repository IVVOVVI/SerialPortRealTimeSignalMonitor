using System;

namespace DllInterface
{

	///<summary>数据源</summary>
	public interface IDataSource : IClose
	{

		///<summary>开始</summary>
		void Start();

		///<summary>停止</summary>
		void Stop();

		///<summary>状态改变（true - 开始 | false - 停止）【注意：此事件可能从辅助线程触发】</summary>
		event EventHandler<bool> StateChanged;

	}

}