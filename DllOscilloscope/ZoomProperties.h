#pragma once

namespace DllOscilloscope
{
	
	///<summary>缩放参数</summary>
	struct ZoomParameters
	{

		///<summary>缩放参数</summary>
		ZoomParameters() :
			Scale(1.0f),
			Scale_Limit(10.0f),
			Translate(0),
			Translate_Limit(0)
		{}

		///<summary>缩放率</summary>
		float Scale;

		///<summary>缩放率极限值</summary>
		float Scale_Limit;

		///<summary>位移</summary>
		UINT Translate;

		///<summary>位移极限值</summary>
		UINT Translate_Limit;

	};

	///<summary>缩放参数</summary>
	struct ZoomProperties
	{

		///<summary>水平</summary>
		ZoomParameters X;

		///<summary>垂直</summary>
		ZoomParameters Y;

	};

}