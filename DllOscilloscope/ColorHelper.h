#pragma once

namespace DllOscilloscope
{

	///<summary>��ȡ͸����ͨ��</summary>
	///<param name="t_Color">��ɫ��ARGB��</param>
	float GetAlphaChannel(const UINT32 t_Color)
	{
		return static_cast<float>((t_Color & 0xFF000000) >> 24u) / 255.f;
	}

}