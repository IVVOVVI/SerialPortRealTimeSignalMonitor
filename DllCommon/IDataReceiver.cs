namespace DllInterface
{

	///<summary>数据接收器</summary>
	public interface IDataReceiver : IPush<float>, IReplace<float>, IClear, IClose { }

}