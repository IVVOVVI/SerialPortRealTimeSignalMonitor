#pragma once

namespace DllOscilloscope
{

	///<summary>获取透明度通道</summary>
	///<param name="t_Color">颜色（ARGB）</param>
	float GetAlphaChannel(const UINT32 t_Color)
	{
		return static_cast<float>((t_Color & 0xFF000000) >> 24u) / 255.f;
	}

}