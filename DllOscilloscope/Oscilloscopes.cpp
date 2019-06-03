#include "stdafx.h"
#include "Oscilloscopes.h"
#include "Oscilloscope.h"

#pragma comment(lib, "d2d1.lib")

using namespace std;
using namespace D2D1;
using namespace Concurrency;
using namespace Microsoft::WRL;
using namespace DllOscilloscope;

///<summary>�ٽ���</summary>
critical_section Oscilloscopes::s_CriticalSection;

///<summary>ʵ��</summary>
unordered_map<HWND, unique_ptr<Oscilloscopes>> Oscilloscopes::s_Instances;

///<summary>D2D����</summary>
ComPtr<ID2D1Factory> Oscilloscopes::s_D2dFactory = nullptr;

///<summary>D2D�������ü���</summary>
int Oscilloscopes::s_D2dFactoryCount = 0;

///<summary>��ӣ�����һ�������Ӧ�ö�Ӧ��ID��</summary>
Oscilloscopes* Oscilloscopes::Add(HWND const t_WindowHwnd, const byte t_Fps) noexcept
{
	if (t_WindowHwnd != nullptr)
	{
		critical_section::scoped_lock lock(s_CriticalSection);
		auto const t_Where = s_Instances.find(t_WindowHwnd);
		if (t_Where != s_Instances.end()) return t_Where->second.get();
		try
		{
			auto t_Instance = new Oscilloscopes(t_WindowHwnd, t_Fps);
			s_Instances.emplace(t_WindowHwnd, unique_ptr<Oscilloscopes>(t_Instance));
			if (s_D2dFactory == nullptr)
			{
				s_D2dFactoryCount = (SUCCEEDED(D2D1CreateFactory(D2D1_FACTORY_TYPE::D2D1_FACTORY_TYPE_MULTI_THREADED, s_D2dFactory.GetAddressOf())) && s_D2dFactory != nullptr) ? 1 : 0;
			}
			else
			{
				++s_D2dFactoryCount;
			}
			return t_Instance;
		}
		catch (...) {}
	}
	return nullptr;
}

///<summary>�Ƴ�</summary>
void Oscilloscopes::Remove(Oscilloscopes* const t_Instance) noexcept
{
	if (t_Instance != nullptr)
	{
		critical_section::scoped_lock lock(s_CriticalSection);
		try
		{
			s_Instances.erase(t_Instance->m_WindowHwnd);
			if (s_D2dFactory != nullptr)
			{
				--s_D2dFactoryCount;
				if (s_D2dFactoryCount <= 0) s_D2dFactory = nullptr;
			}
		}
		catch (...) {}
	}
}

///<summary>��ʱ���ص�</summary>
VOID CALLBACK Oscilloscopes::s_TimerCallback(HWND t_Hwnd, UINT t_Message, UINT_PTR t_Timer, DWORD t_Time) noexcept
{
	try
	{
		critical_section::scoped_lock lock(s_CriticalSection);
		s_Instances.at(t_Hwnd)->m_TimerCallback();
	}
	catch (...) {}
}

///<summary>ʾ������</summary>
Oscilloscopes::~Oscilloscopes()
{
	KillTimer(m_WindowHwnd, m_TimerId);
	for (auto const t_Oscilloscope : m_Oscilloscopes) delete t_Oscilloscope;
}

///<summary>ʾ������</summary>
Oscilloscopes::Oscilloscopes(HWND const t_Hwnd, const byte t_Fps) :
	m_WindowHwnd(t_Hwnd),
	m_TimerId(SetTimer(t_Hwnd, 0u, static_cast<UINT>(round(1000.0f / static_cast<float>(max(t_Fps, 30ui8)))), &s_TimerCallback))
{
	m_Oscilloscopes.reserve(10u);
}

///<summary>����ʾ����</summary>
Oscilloscope* Oscilloscopes::CreateOscilloscope(HWND const t_ControlHwnd, const ConstructionArgs* const t_Args, const GraphProperties* const t_GraphProperties, const UINT t_Size) noexcept
{
	try
	{
		critical_section::scoped_lock lock(m_CriticalSection);
		auto t_Instance = new Oscilloscope(s_D2dFactory.Get(), t_ControlHwnd, t_Args, t_GraphProperties, t_Size);
		m_Oscilloscopes.emplace_back(t_Instance);
		return t_Instance;
	}
#if _DEBUG
	catch (const HRESULT &err)
	{
		DEBUG_MESSAGE(err);
	}
#endif
	catch (...) {}
	return nullptr;
}

///<summary>ˢ��DPI</summary>
void Oscilloscopes::RefreshDPI() noexcept
{
	try
	{
		auto const t_Dpi = static_cast<float>(GetDpiForWindow(m_WindowHwnd));
		critical_section::scoped_lock lock(m_CriticalSection);
		for (auto const &t_Oscilloscope : m_Oscilloscopes) try
		{
			t_Oscilloscope->ChangeDPI(t_Dpi, t_Dpi);
		}
		catch (...) {}
	}
	catch (...) {}
}

///<summary>��ʱ���ص�</summary>
void Oscilloscopes::m_TimerCallback()
{
	if (IsIconic(m_WindowHwnd) == FALSE)
	{
		critical_section::scoped_lock lock(m_CriticalSection);
		for (auto const &t_Oscilloscope : m_Oscilloscopes) try
		{
			t_Oscilloscope->Render();
		}
		catch (...) {}
	}
}