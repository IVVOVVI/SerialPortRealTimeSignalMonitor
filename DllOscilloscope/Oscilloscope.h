#pragma once

#include "Common.h"

namespace DllOscilloscope
{

	///<summary>����</summary>
	class Cache;
	///<summary>���Ų���</summary>
	struct ZoomParameters;
	///<summary>���Ų���</summary>
	struct ZoomProperties;
	///<summary>���������</summary>
	struct AxisProperties;
	///<summary>ͼ�β���</summary>
	struct GraphProperties;
	///<summary>�����ò���</summary>
	struct ConstructionArgs;

	///<summary>ʾ����</summary>
	class Oscilloscope final
	{

	public:

		///<summary>ʾ����</summary>
		~Oscilloscope();

		///<summary>ʾ����</summary>
		///<param name="t_Factory">D2D����</param>
		///<param name="t_ControlHwnd">��ͼ�����</param>
		///<param name="t_Args">�����ò���</param>
		///<param name="t_GraphProperties">ͼ�β�������</param>
		///<param name="t_Size">�����С</param>
		Oscilloscope(Microsoft::WRL::ComPtr<ID2D1Factory> const &t_Factory, HWND const t_ControlHwnd, const ConstructionArgs* const t_Args, const GraphProperties* const t_GraphProperties, const UINT t_Size);

		///<summary>ˮƽ�����ʡ����̵߳��á�</summary>
		float ScaleX() const noexcept;

		///<summary>ˮƽ�����ʼ���ֵ�����̵߳��á�</summary>
		float ScaleX_Limit() const noexcept;

		///<summary>ˮƽλ�ơ����̵߳��á�</summary>
		int TranslateX() const noexcept;

		///<summary>ˮƽλ�Ƽ���ֵ�����̵߳��á�</summary>
		int TranslateX_Limit() const noexcept;

		///<summary>��ֱ�����ʡ����̵߳��á�</summary>
		float ScaleY() const noexcept;

		///<summary>��ֱ�����ʼ���ֵ�����̵߳��á�</summary>
		float ScaleY_Limit() const noexcept;

		///<summary>��ֱλ�ơ����̵߳��á�</summary>
		int TranslateY() const noexcept;

		///<summary>��ֱλ�Ƽ���ֵ�����̵߳��á�</summary>
		int TranslateY_Limit() const noexcept;

		///<summary>�ı����ż���ֵ�����̵߳��á�</summary>
		///<param name="t_Value">ֵ��>=1��</param>
		bool ChangeScaleLimit(const UINT t_Value);

		///<summary>�ı�ˮƽ���ż���ֵ�����̵߳��á�</summary>
		///<param name="t_Value">ֵ��>=1��</param>
		bool ChangeScaleLimitX(const UINT t_Value);

		///<summary>�ı䴹ֱ���ż���ֵ�����̵߳��á�</summary>
		///<param name="t_Value">ֵ��>=1��</param>
		bool ChangeScaleLimitY(const UINT t_Value);

		///<summary>���š����̵߳��á�</summary>
		///<param name="t_Offset">��ֵ</param>
		bool Scale(const float t_Offset);

		///<summary>ˮƽ���š����̵߳��á�</summary>
		///<param name="t_Offset">��ֵ</param>
		bool ScaleX(const float t_Offset);

		///<summary>��ֱ���š����̵߳��á�</summary>
		///<param name="t_Offset">��ֵ</param>
		bool ScaleY(const float t_Offset);

		///<summary>ƽ�ơ����̵߳��á�</summary>
		///<param name="t_OffsetX">ˮƽ��ֵ</param>
		///<param name="t_OffsetY">��ֱ��ֵ</param>
		bool Translate(const int t_OffsetX, const int t_OffsetY);

		///<summary>ˮƽƽ�ơ����̵߳��á�</summary>
		///<param name="t_Offset">��ֵ</param>
		bool TranslateX(const int t_Offset);

		///<summary>��ֱƽ�ơ����̵߳��á�</summary>
		///<param name="t_Offset">��ֵ</param>
		bool TranslateY(const int t_Offset);

		///<summary>�ı�ˢ��ģʽ�����̵߳��á�</summary>
		///<param name="t_RefreshMode">ˢ��ģʽ</param>
		void ChangeRefreshMode(const Enum_RefreshMode t_RefreshMode);

		///<summary>�ı�������͡����̵߳��á�</summary>
		///<param name="t_GraphIndex">ͼ�����</param>
		///<param name="t_BaselineType">��������</param>
		void ChangeBaselineType(const UINT t_GraphIndex, const Enum_BaselineType t_BaselineType);

		///<summary>�ı�ͼ����ʽ�����̵߳��á�</summary>
		///<param name="t_GraphStyle">ͼ����ʽ����</param>
		void ChangeGraphStyles(const Enum_GraphStyle* const t_GraphStyle);

		///<summary>�ı�ͼ����ʽ�����̵߳��á�</summary>
		///<param name="t_GraphIndex">ͼ�����</param>
		///<param name="t_GraphStyle">ͼ����ʽ</param>
		void ChangeGraphStyle(const UINT t_GraphIndex, const Enum_GraphStyle t_GraphStyle);

		///<summary>�ı��������߿����̵߳��á�</summary>
		///<param name="t_AxisLineWidth">�������߿�</param>
		void ChangeAxisLineWidth(const UINT t_AxisLineWidth);

		///<summary>�ı���������ɫ�����̵߳��á�</summary>
		///<param name="t_AxisColor">��������ɫ</param>
		void ChangeAxisColor(const UINT32 t_AxisColor);

		///<summary>�ı䱳��ɫ�����̵߳��á�</summary>
		///<param name="t_Background">����ɫ</param>
		void ChangeBackgroundColor(const UINT32 t_Background);

		///<summary>�ı�ǰ��ɫ�����̵߳��á�</summary>
		///<param name="t_Foreground">ǰ��ɫ����</param>
		void ChangeForegroundColors(const UINT32* const t_Foreground);

		///<summary>�ı�ǰ��ɫ�����̵߳��á�</summary>
		///<param name="t_GraphIndex">ͼ�����</param>
		///<param name="t_Foreground">ǰ��ɫ</param>
		void ChangeForegroundColor(const UINT t_GraphIndex, const UINT32 t_Foreground);

		///<summary>�ı��߿����̵߳��á�</summary>
		///<param name="t_GraphIndex">ͼ�����</param>
		///<param name="t_LineWidth">�߿�</param>
		void ChangeLineWidth(const UINT t_GraphIndex, const byte t_LineWidth);

		///<summary>�ı����ƫ��ֵ�����̵߳��á�</summary>
		///<param name="t_GraphIndex">ͼ�����</param>
		///<param name="t_Offset">����ƫ��ֵ</param>
		void ChangeOffset(const UINT t_GraphIndex, const int t_Offset);

		///<summary>�ı��Զ����㡾���̵߳��á�</summary>
		///<param name="t_GraphIndex">ͼ�����</param>
		///<param name="t_Offset">ƫ��ֵ</param>
		void ChangeAutoTranslate(const UINT t_GraphIndex, const float t_Offset);

		///<summary>�Ƿ���á����̵߳��á�</summary>
		///<param name="t_GraphIndex">ͼ�����</param>
		///<param name="t_IsEnabled">�Ƿ����</param>
		void ChangeIsEnabled(const UINT t_GraphIndex, const bool t_IsEnabled) noexcept;

		///<summary>��ȫ��������</summary>
		///<param name="t_GraphIndex">ͼ�����</param>
		///<param name="t_Value">ֵ</param>
		void SafePush(const UINT t_GraphIndex, const float t_Value) noexcept;

		///<summary>��������</summary>
		///<param name="t_GraphIndex">ͼ�����</param>
		///<param name="t_Value">ֵ</param>
		void Push(const UINT t_GraphIndex, const float t_Value) noexcept;

		///<summary>��ȫ�滻ȫ������</summary>
		///<param name="t_GraphIndex">ͼ�����</param>
		///<param name="t_Value">ֵ</param>
		///<param name="t_Length">ֵ����</param>
		void SafeReplace(const UINT t_GraphIndex, const float* const t_Value, const UINT t_Length) noexcept;

		///<summary>�滻ȫ������</summary>
		///<param name="t_GraphIndex">ͼ�����</param>
		///<param name="t_Value">ֵ</param>
		///<param name="t_Length">ֵ����</param>
		void Replace(const UINT t_GraphIndex, const float* const t_Value, const UINT t_Length) noexcept;

		///<summary>��ȫ���</summary>
		void SafeClear() noexcept;

		///<summary>���</summary>
		void Clear() noexcept;

		///<summary>�ı�DPI</summary>
		///<param name="t_DpiX">ˮƽDPI</param>
		///<param name="t_DpiY">��ֱDPI</param>
		void ChangeDPI(const float t_DpiX, const float t_DpiY);

		///<summary>�ı��С</summary>
		void Resize();

		///<summary>ˢ��</summary>
		void Refresh();

		///<summary>��Ⱦ</summary>
		void Render();

	private:

		///<summary>ʾ������ṹ</summary>
		struct OscilloscopeItemStruct
		{

			///<summary>ʾ������ṹ</summary>
			~OscilloscopeItemStruct();

			///<summary>ʾ������ṹ</summary>
			///<param name="t_Count">Ҫ���ֵ�������</param>
			///<param name="t_Range">����</param>
			///<param name="t_GraphProperties">ͼ�β���</param>
			///<param name="t_Zoom">���Ų���</param>
			OscilloscopeItemStruct(const UINT32 t_Count, const float t_Range, const GraphProperties* const t_GraphProperties, const ZoomProperties* const t_Zoom);

			///<summary>��ʽ</summary>
			Enum_GraphStyle Style;

			///<summary>ǰ��ɫ</summary>
			UINT32 ForegroundValue;

			///<summary>ǰ��ɫ</summary>
			D2D1_COLOR_F Foreground;

			///<summary>����</summary>
			Cache* const DataCache;

			///<summary>ͼ����ɫ</summary>
			Microsoft::WRL::ComPtr<ID2D1SolidColorBrush> GraphColor;

			///<summary>������ʽ</summary>
			Microsoft::WRL::ComPtr<ID2D1StrokeStyle> LineStyle;

			///<summary>����ͼ</summary>
			Microsoft::WRL::ComPtr<ID2D1PathGeometry> Geometry;

			///<summary>�߿�</summary>
			float LineWeight;

			///<summary>�Ƿ�����</summary>
			bool IsEnabled;

		};

		///<summary>ƽ�Ʋ���</summary>
		struct TranslateArgs
		{
			///<summary>ƽ�Ʋ���</summary>
			///<param name="t_Args">����</param>
			///<param name="t_Offset">��ֵ</param>
			TranslateArgs(ZoomParameters* const t_Args, const int t_Offset = 0);
			///<summary>����</summary>
			ZoomParameters* const Args;
			///<summary>λ��</summary>
			int Translate;
		};

		///<summary>���Ų���</summary>
		struct ScaleArgs
		{
			///<summary>���Ų���</summary>
			///<param name="t_Args">����</param>
			///<param name="t_Offset">��ֵ</param>
			ScaleArgs(ZoomParameters* const t_Args, const float t_Offset = 0.0f);
			///<summary>����</summary>
			ZoomParameters* const Args;
			///<summary>������</summary>
			float Scale;
			///<summary>ƽ��</summary>
			int Translate;
		};

		///<summary>�ٽ���</summary>
		Concurrency::critical_section m_CriticalSection;

		///<summary>��ͼ�����</summary>
		HWND const m_Handle;

		///<summary>ͼ�����</summary>
		const D2D1_LAYER_PARAMETERS m_LayerParameters;

		///<summary>����ɫ</summary>
		UINT32 m_BackgroundValue;

		///<summary>����ɫ</summary>
		D2D1_COLOR_F m_Background;

		///<summary>ˢ��ģʽ</summary>
		Enum_RefreshMode m_RefreshMode;

		///<summary>����</summary>
		ZoomProperties* const m_Zoom;

		///<summary>������</summary>
		AxisProperties* const m_Axis;

		///<summary>D2D����</summary>
		Microsoft::WRL::ComPtr<ID2D1Factory> const m_D2dFactory;

		///<summary>����Ŀ��</summary>
		Microsoft::WRL::ComPtr<ID2D1HwndRenderTarget> m_RenderTarget;

		///<summary>ͼ��</summary>
		Microsoft::WRL::ComPtr<ID2D1Layer> m_Layer;

		///<summary>������</summary>
		const UINT m_DatalLength;

		///<summary>������</summary>
		std::unique_ptr<std::unique_ptr<OscilloscopeItemStruct>[]> const m_Items;

		///<summary>���������</summary>
		const UINT m_ItemsSize;

		///<summary>��ͼ�����</summary>
		float m_Width;

		///<summary>��ͼ���߶�</summary>
		float m_Height;

		///<summary>�Ƿ���Ҫˢ��</summary>
		std::atomic_bool m_IsNeedRefresh;

		///<summary>�ؽ�</summary>
		void m_Recreate();

		///<summary>��ȡ��ͼ���ߴ�</summary>
		D2D1_SIZE_U m_GetSize();

		///<summary>��ʼ��</summary>
		///<param name="t_Args">����</param>
		///<param name="t_Size">��ͼ���ߴ�</param>
		void m_Initialize(ZoomParameters &t_Args, const float t_Size);

		///<summary>�ı����ż���ֵ�����̵߳��á�</summary>
		///<param name="t_Args">����</param>
		///<param name="t_Size">��ͼ���ߴ�</param>
		///<param name="t_Limit">����ֵ</param>
		bool m_ChangeScaleLimit(ScaleArgs &t_Args, const float t_Size, const float t_Limit);

		///<summary>���š����̵߳��á�</summary>
		///<param name="t_Args">����</param>
		///<param name="t_Size">��ͼ���ߴ�</param>
		bool m_Scale(ScaleArgs &t_Args, const float t_Size);

		///<summary>ƽ�ơ����̵߳��á�</summary>
		///<param name="t_Args">����</param>
		bool m_Translate(TranslateArgs &t_Args);

		///<summary>��Ⱦ</summary>
		void m_Render();

		///<summary>���ƣ�ɨ��ģʽ</summary>
		///<param name="t_Item">��</param>
		///<param name="t_Clip">������</param>
		///<param name="t_Size">�ߴ�</param>
		///<param name="t_Area">����</param>
		///<param name="t_Step">����</param>
		void m_Draw_Scanning(OscilloscopeItemStruct* const t_Item, const D2D1_RECT_F &t_Clip, const D2D1_SIZE_F &t_Size, const D2D1_RECT_F &t_Area, const float t_Step);

		///<summary>���ƣ�����ģʽ</summary>
		///<param name="t_Item">��</param>
		///<param name="t_Clip">������</param>
		///<param name="t_Size">�ߴ�</param>
		///<param name="t_Area">����</param>
		///<param name="t_Step">����</param>
		void m_Draw_Scrolling(OscilloscopeItemStruct* const t_Item, const D2D1_RECT_F &t_Clip, const D2D1_SIZE_F &t_Size, const D2D1_RECT_F &t_Area, const float t_Step);

	};

}