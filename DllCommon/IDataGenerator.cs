namespace DllInterface
{

	///<summary>数据产生器</summary>
	public interface IDataGenerator<T>
	{

		///<summary>刷新率（最大值60，且不能小于等于0。）</summary>
		double RefreshRate();

		///<summary>每次的数据量</summary>
		uint DataLengthPerTime();

		///<summary>创建数据</summary>
		T CreateData();

	}

}