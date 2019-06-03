#pragma once

namespace DllOscilloscope
{

	///<summary>�����ò���</summary>
	struct ConstructionArgs
	{

		///<summary>����ɫ</summary>
		UINT32 Background;

		///<summary>Ҫ���ֵ�������</summary>
		UINT32 Count;

		///<summary>���̣����磺[-50,50]������Ϊ100��</summary>
		float Range;

		///<summary>ˮƽ������</summary>
		float ScaleX;

		///<summary>ˮƽ���������</summary>
		float ScaleLimitX;

		///<summary>��ֱ������</summary>
		float ScaleY;

		///<summary>��ֱ���������</summary>
		float ScaleLimitY;

		///<summary>ˮƽλ��ֵ</summary>
		UINT TranslateX;

		///<summary>��ֱλ��ֵ</summary>
		UINT TranslateY;

		///<summary>ˢ��ģʽ</summary>
		Enum_RefreshMode RefreshMode;

		///<summary>��������ɫ</summary>
		UINT32 AxisColor;

		///<summary>�������߿�</summary>
		UINT AxisLineWidth;

		///<summary>����̶ȵ�������������ԭ��̶ȣ����̶ܿ�����Ϊ���趨ֵ��1����ֵΪ0ʱ�����ƿ̶ȡ���</summary>
		UINT AxisX_TicksCount;

		///<summary>����̶ȵ�������������ԭ��̶ȣ����̶ܿ�����Ϊ���趨ֵ��1����ֵΪ0ʱ�����ƿ̶ȡ���</summary>
		UINT AxisY_TicksCount;

		///<summary>����̶ȵĸ߶�</summary>
		UINT AxisX_TickHeight;

		///<summary>����̶ȵĿ��</summary>
		UINT AxisY_TickWidth;

		///<summary>����߶�</summary>
		UINT AxisX_Height;

		///<summary>������</summary>
		UINT AxisY_Width;

		///<summary>����λ��</summary>
		Enum_AxisPlacement_X AxisX_Placement;

		///<summary>����λ��</summary>
		Enum_AxisPlacement_Y AxisY_Placement;

	};

}