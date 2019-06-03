namespace DllInterface
{

	///<summary>数据替换</summary>
	public interface IReplace<T>
	{

		///<summary>替换全部数据</summary>
		///<param name="value">值</param>
		void Replace(T[] value);

		///<summary>安全替换全部数据（并行安全）</summary>
		///<param name="value">值</param>
		void SafeReplace(T[] value);

	}

}