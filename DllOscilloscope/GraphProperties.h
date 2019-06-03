#pragma once

#include "Common.h"

namespace DllOscilloscope
{

	///<summary>ͼ�β���</summary>
	struct GraphProperties
	{

		///<summary>��������</summary>
		Enum_BaselineType BaselineType;

		///<summary>ͼ����ʽ</summary>
		Enum_GraphStyle GraphStyle;

		///<summary>ǰ��ɫ</summary>
		UINT32 Foreground;

		///<summary>�߿�ֻ��������ͼ����Ч��</summary>
		UINT32 LineWidth;

		///<summary>����ƫ��ֵ</summary>
		int Offset;

		///<summary>�Զ����㣨����Ƿ���ֵ��ʹ�ô˹��ܣ�������ӽ��������ֵ�������ϴ�ֵ����</summary>
		float AutoTranslate;

		///<summary>�Ƿ�����</summary>
		bool IsEnabled;

	};

}