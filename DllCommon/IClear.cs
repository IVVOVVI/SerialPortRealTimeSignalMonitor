namespace DllInterface
{

	///<summary>数据清空</summary>
	public interface IClear
	{

		///<summary>清空</summary>
		void Clear();

		///<summary>安全清空（并行安全）</summary>
		void SafeClear();

	}

}