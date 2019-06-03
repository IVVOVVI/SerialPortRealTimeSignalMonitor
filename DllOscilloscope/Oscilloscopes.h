#pragma once

#include <vector>
#include <unordered_map>
#include "Common.h"

namespace DllOscilloscope
{

	///<summary>�����ò���</summary>
	struct ConstructionArgs;
	///<summary>ͼ�β���</summary>
	struct GraphProperties;
	///<summary>ʾ����</summary>
	class Oscilloscope;

	///<summary>ʾ������</summary>
	class Oscilloscopes final
	{

	public:

		///<summary>���</summary>
		///<param name="t_WindowHwnd">���ھ��</param>
		///<param name="t_Fps">֡�ʣ���Сֵ��30��</param>
		static Oscilloscopes* Add(HWND const t_WindowHwnd, const byte t_Fps = 60ui8) noexcept;

		///<summary>�Ƴ�</summary>
		///<param name="t_Instance">ʵ��</param>
		static void Remove(Oscilloscopes* const t_Instance) noexcept;

		///<summary>ʾ������</summary>
		~Oscilloscopes();

		///<summary>����ʾ����</summary>
		///<param name="t_ControlHwnd">��ͼ�����</param>
		///<param name="t_Args">�����ò���</param>
		///<param name="t_GraphProperties">ͼ�β�������</param>
		///<param name="t_Size">�����С</param>
		Oscilloscope* CreateOscilloscope(HWND const t_ControlHwnd, const ConstructionArgs* const t_Args, const GraphProperties* const t_GraphProperties, const UINT t_Size) noexcept;

		///<summary>ˢ��DPI</summary>
		void RefreshDPI() noexcept;

	private:

		///<summary>�ٽ���</summary>
		static Concurrency::critical_section s_CriticalSection;

		///<summary>ʵ��</summary>
		static std::unordered_map<HWND, std::unique_ptr<Oscilloscopes>> s_Instances;

		///<summary>D2D����</summary>
		static Microsoft::WRL::ComPtr<ID2D1Factory> s_D2dFactory;

		///<summary>D2D��������</summary>
		static int s_D2dFactoryCount;

		///<summary>��ʱ���ص�</summary>
		static VOID CALLBACK s_TimerCallback(HWND t_Hwnd, UINT t_Message, UINT_PTR t_Timer, DWORD t_Time) noexcept;

		///<summary>ʾ������</summary>
		///<param name="hwnd">���ھ��</param>
		///<param name="fps">֡�ʣ���Сֵ��30��</param>
		Oscilloscopes(HWND const t_Hwnd, const byte t_Fps);

		///<summary>�ٽ���</summary>
		Concurrency::critical_section m_CriticalSection;

		///<summary>ʾ��������</summary>
		std::vector<Oscilloscope*> m_Oscilloscopes;

		///<summary>���ھ��</summary>
		HWND const m_WindowHwnd;

		///<summary>��ʱ��ID</summary>
		const UINT_PTR m_TimerId;

		///<summary>��ʱ���ص�</summary>
		void m_TimerCallback();

	};

}