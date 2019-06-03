#pragma once

#include <memory>
#include <atomic>
#include <ppl.h>
#include <wrl/client.h>
#include <d2d1.h>
#include "comdef.h"

///<summary>检查结果</summary>
#define CHECK_COM_THROW(hresult) if(FAILED(hresult)) throw hresult

///<summary>输出错误信息</summary>
#define DEBUG_MESSAGE(err) OutputDebugStringW(_com_error(err).ErrorMessage());

namespace DllOscilloscope
{

	///<summary>基线类型（Bottom：横轴在下方，即只接收正数值。 | Center：横轴在中间，即正数和负数均可。 | Top：横轴在上方，即直接收负数值。）</summary>
	enum class Enum_BaselineType : UINT32 { Bottom, Center, Top };

	///<summary>图形样式</summary>
	enum class Enum_GraphStyle : UINT32 { Bar, Line };

	///<summary>刷新模式</summary>
	enum class Enum_RefreshMode : UINT32 { Scanning, Scrolling };

	///<summary>横轴位置</summary>
	enum class Enum_AxisPlacement_X : UINT32 { Bottom, Center, Top };

	///<summary>纵轴位置</summary>
	enum class Enum_AxisPlacement_Y : UINT32 { Left, Center, Right };

}