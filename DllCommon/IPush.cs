namespace DllInterface
{

	///<summary>数据存入</summary>
	public interface IPush<T>
	{

		///<summary>存入数据</summary>
		///<param name="value">值</param>
		void Push(T value);

		///<summary>安全存入数据（并行安全）</summary>
		///<param name="value">值</param>
		void SafePush(T value);

	}

}