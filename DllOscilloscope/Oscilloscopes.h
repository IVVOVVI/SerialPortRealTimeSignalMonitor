#pragma once

#include <vector>
#include <unordered_map>
#include "Common.h"

namespace DllOscilloscope
{

	///<summary>构造用参数</summary>
	struct ConstructionArgs;
	///<summary>图形参数</summary>
	struct GraphProperties;
	///<summary>示波器</summary>
	class Oscilloscope;

	///<summary>示波器组</summary>
	class Oscilloscopes final
	{

	public:

		///<summary>添加</summary>
		///<param name="t_WindowHwnd">窗口句柄</param>
		///<param name="t_Fps">帧率（最小值：30）</param>
		static Oscilloscopes* Add(HWND const t_WindowHwnd, const byte t_Fps = 60ui8) noexcept;

		///<summary>移除</summary>
		///<param name="t_Instance">实例</param>
		static void Remove(Oscilloscopes* const t_Instance) noexcept;

		///<summary>示波器组</summary>
		~Oscilloscopes();

		///<summary>创建示波器</summary>
		///<param name="t_ControlHwnd">绘图区句柄</param>
		///<param name="t_Args">构造用参数</param>
		///<param name="t_GraphProperties">图形参数数组</param>
		///<param name="t_Size">数组大小</param>
		Oscilloscope* CreateOscilloscope(HWND const t_ControlHwnd, const ConstructionArgs* const t_Args, const GraphProperties* const t_GraphProperties, const UINT t_Size) noexcept;

		///<summary>刷新DPI</summary>
		void RefreshDPI() noexcept;

	private:

		///<summary>临界区</summary>
		static Concurrency::critical_section s_CriticalSection;

		///<summary>实例</summary>
		static std::unordered_map<HWND, std::unique_ptr<Oscilloscopes>> s_Instances;

		///<summary>D2D工厂</summary>
		static Microsoft::WRL::ComPtr<ID2D1Factory> s_D2dFactory;

		///<summary>D2D工厂计数</summary>
		static int s_D2dFactoryCount;

		///<summary>计时器回调</summary>
		static VOID CALLBACK s_TimerCallback(HWND t_Hwnd, UINT t_Message, UINT_PTR t_Timer, DWORD t_Time) noexcept;

		///<summary>示波器组</summary>
		///<param name="hwnd">窗口句柄</param>
		///<param name="fps">帧率（最小值：30）</param>
		Oscilloscopes(HWND const t_Hwnd, const byte t_Fps);

		///<summary>临界区</summary>
		Concurrency::critical_section m_CriticalSection;

		///<summary>示波器集合</summary>
		std::vector<Oscilloscope*> m_Oscilloscopes;

		///<summary>窗口句柄</summary>
		HWND const m_WindowHwnd;

		///<summary>计时器ID</summary>
		const UINT_PTR m_TimerId;

		///<summary>计时器回调</summary>
		void m_TimerCallback();

	};

}