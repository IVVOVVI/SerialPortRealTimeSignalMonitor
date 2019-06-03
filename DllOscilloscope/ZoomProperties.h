#pragma once

namespace DllOscilloscope
{
	
	///<summary>���Ų���</summary>
	struct ZoomParameters
	{

		///<summary>���Ų���</summary>
		ZoomParameters() :
			Scale(1.0f),
			Scale_Limit(10.0f),
			Translate(0),
			Translate_Limit(0)
		{}

		///<summary>������</summary>
		float Scale;

		///<summary>�����ʼ���ֵ</summary>
		float Scale_Limit;

		///<summary>λ��</summary>
		UINT Translate;

		///<summary>λ�Ƽ���ֵ</summary>
		UINT Translate_Limit;

	};

	///<summary>���Ų���</summary>
	struct ZoomProperties
	{

		///<summary>ˮƽ</summary>
		ZoomParameters X;

		///<summary>��ֱ</summary>
		ZoomParameters Y;

	};

}