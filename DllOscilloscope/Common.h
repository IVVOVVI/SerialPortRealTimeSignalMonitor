#pragma once

#include <memory>
#include <atomic>
#include <ppl.h>
#include <wrl/client.h>
#include <d2d1.h>
#include "comdef.h"

///<summary>�����</summary>
#define CHECK_COM_THROW(hresult) if(FAILED(hresult)) throw hresult

///<summary>���������Ϣ</summary>
#define DEBUG_MESSAGE(err) OutputDebugStringW(_com_error(err).ErrorMessage());

namespace DllOscilloscope
{

	///<summary>�������ͣ�Bottom���������·�����ֻ��������ֵ�� | Center���������м䣬�������͸������ɡ� | Top���������Ϸ�����ֱ���ո���ֵ����</summary>
	enum class Enum_BaselineType : UINT32 { Bottom, Center, Top };

	///<summary>ͼ����ʽ</summary>
	enum class Enum_GraphStyle : UINT32 { Bar, Line };

	///<summary>ˢ��ģʽ</summary>
	enum class Enum_RefreshMode : UINT32 { Scanning, Scrolling };

	///<summary>����λ��</summary>
	enum class Enum_AxisPlacement_X : UINT32 { Bottom, Center, Top };

	///<summary>����λ��</summary>
	enum class Enum_AxisPlacement_Y : UINT32 { Left, Center, Right };

}