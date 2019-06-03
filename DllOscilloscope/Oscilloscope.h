#pragma once

#include "Common.h"

namespace DllOscilloscope
{

	///<summary>缓存</summary>
	class Cache;
	///<summary>缩放参数</summary>
	struct ZoomParameters;
	///<summary>缩放参数</summary>
	struct ZoomProperties;
	///<summary>坐标轴参数</summary>
	struct AxisProperties;
	///<summary>图形参数</summary>
	struct GraphProperties;
	///<summary>构造用参数</summary>
	struct ConstructionArgs;

	///<summary>示波器</summary>
	class Oscilloscope final
	{

	public:

		///<summary>示波器</summary>
		~Oscilloscope();

		///<summary>示波器</summary>
		///<param name="t_Factory">D2D工厂</param>
		///<param name="t_ControlHwnd">绘图区句柄</param>
		///<param name="t_Args">构造用参数</param>
		///<param name="t_GraphProperties">图形参数数组</param>
		///<param name="t_Size">数组大小</param>
		Oscilloscope(Microsoft::WRL::ComPtr<ID2D1Factory> const &t_Factory, HWND const t_ControlHwnd, const ConstructionArgs* const t_Args, const GraphProperties* const t_GraphProperties, const UINT t_Size);

		///<summary>水平缩放率【单线程调用】</summary>
		float ScaleX() const noexcept;

		///<summary>水平缩放率极限值【单线程调用】</summary>
		float ScaleX_Limit() const noexcept;

		///<summary>水平位移【单线程调用】</summary>
		int TranslateX() const noexcept;

		///<summary>水平位移极限值【单线程调用】</summary>
		int TranslateX_Limit() const noexcept;

		///<summary>垂直缩放率【单线程调用】</summary>
		float ScaleY() const noexcept;

		///<summary>垂直缩放率极限值【单线程调用】</summary>
		float ScaleY_Limit() const noexcept;

		///<summary>垂直位移【单线程调用】</summary>
		int TranslateY() const noexcept;

		///<summary>垂直位移极限值【单线程调用】</summary>
		int TranslateY_Limit() const noexcept;

		///<summary>改变缩放极限值【单线程调用】</summary>
		///<param name="t_Value">值（>=1）</param>
		bool ChangeScaleLimit(const UINT t_Value);

		///<summary>改变水平缩放极限值【单线程调用】</summary>
		///<param name="t_Value">值（>=1）</param>
		bool ChangeScaleLimitX(const UINT t_Value);

		///<summary>改变垂直缩放极限值【单线程调用】</summary>
		///<param name="t_Value">值（>=1）</param>
		bool ChangeScaleLimitY(const UINT t_Value);

		///<summary>缩放【单线程调用】</summary>
		///<param name="t_Offset">差值</param>
		bool Scale(const float t_Offset);

		///<summary>水平缩放【单线程调用】</summary>
		///<param name="t_Offset">差值</param>
		bool ScaleX(const float t_Offset);

		///<summary>垂直缩放【单线程调用】</summary>
		///<param name="t_Offset">差值</param>
		bool ScaleY(const float t_Offset);

		///<summary>平移【单线程调用】</summary>
		///<param name="t_OffsetX">水平差值</param>
		///<param name="t_OffsetY">垂直差值</param>
		bool Translate(const int t_OffsetX, const int t_OffsetY);

		///<summary>水平平移【单线程调用】</summary>
		///<param name="t_Offset">差值</param>
		bool TranslateX(const int t_Offset);

		///<summary>垂直平移【单线程调用】</summary>
		///<param name="t_Offset">差值</param>
		bool TranslateY(const int t_Offset);

		///<summary>改变刷新模式【单线程调用】</summary>
		///<param name="t_RefreshMode">刷新模式</param>
		void ChangeRefreshMode(const Enum_RefreshMode t_RefreshMode);

		///<summary>改变基线类型【单线程调用】</summary>
		///<param name="t_GraphIndex">图形序号</param>
		///<param name="t_BaselineType">基线类型</param>
		void ChangeBaselineType(const UINT t_GraphIndex, const Enum_BaselineType t_BaselineType);

		///<summary>改变图形样式【单线程调用】</summary>
		///<param name="t_GraphStyle">图形样式数组</param>
		void ChangeGraphStyles(const Enum_GraphStyle* const t_GraphStyle);

		///<summary>改变图形样式【单线程调用】</summary>
		///<param name="t_GraphIndex">图形序号</param>
		///<param name="t_GraphStyle">图形样式</param>
		void ChangeGraphStyle(const UINT t_GraphIndex, const Enum_GraphStyle t_GraphStyle);

		///<summary>改变坐标轴线宽【单线程调用】</summary>
		///<param name="t_AxisLineWidth">坐标轴线宽</param>
		void ChangeAxisLineWidth(const UINT t_AxisLineWidth);

		///<summary>改变坐标轴颜色【单线程调用】</summary>
		///<param name="t_AxisColor">坐标轴颜色</param>
		void ChangeAxisColor(const UINT32 t_AxisColor);

		///<summary>改变背景色【单线程调用】</summary>
		///<param name="t_Background">背景色</param>
		void ChangeBackgroundColor(const UINT32 t_Background);

		///<summary>改变前景色【单线程调用】</summary>
		///<param name="t_Foreground">前景色数组</param>
		void ChangeForegroundColors(const UINT32* const t_Foreground);

		///<summary>改变前景色【单线程调用】</summary>
		///<param name="t_GraphIndex">图形序号</param>
		///<param name="t_Foreground">前景色</param>
		void ChangeForegroundColor(const UINT t_GraphIndex, const UINT32 t_Foreground);

		///<summary>改变线宽【单线程调用】</summary>
		///<param name="t_GraphIndex">图形序号</param>
		///<param name="t_LineWidth">线宽</param>
		void ChangeLineWidth(const UINT t_GraphIndex, const byte t_LineWidth);

		///<summary>改变基线偏移值【单线程调用】</summary>
		///<param name="t_GraphIndex">图形序号</param>
		///<param name="t_Offset">基线偏移值</param>
		void ChangeOffset(const UINT t_GraphIndex, const int t_Offset);

		///<summary>改变自动归零【单线程调用】</summary>
		///<param name="t_GraphIndex">图形序号</param>
		///<param name="t_Offset">偏移值</param>
		void ChangeAutoTranslate(const UINT t_GraphIndex, const float t_Offset);

		///<summary>是否可用【单线程调用】</summary>
		///<param name="t_GraphIndex">图形序号</param>
		///<param name="t_IsEnabled">是否可用</param>
		void ChangeIsEnabled(const UINT t_GraphIndex, const bool t_IsEnabled) noexcept;

		///<summary>安全存入数据</summary>
		///<param name="t_GraphIndex">图形序号</param>
		///<param name="t_Value">值</param>
		void SafePush(const UINT t_GraphIndex, const float t_Value) noexcept;

		///<summary>存入数据</summary>
		///<param name="t_GraphIndex">图形序号</param>
		///<param name="t_Value">值</param>
		void Push(const UINT t_GraphIndex, const float t_Value) noexcept;

		///<summary>安全替换全部数据</summary>
		///<param name="t_GraphIndex">图形序号</param>
		///<param name="t_Value">值</param>
		///<param name="t_Length">值数量</param>
		void SafeReplace(const UINT t_GraphIndex, const float* const t_Value, const UINT t_Length) noexcept;

		///<summary>替换全部数据</summary>
		///<param name="t_GraphIndex">图形序号</param>
		///<param name="t_Value">值</param>
		///<param name="t_Length">值数量</param>
		void Replace(const UINT t_GraphIndex, const float* const t_Value, const UINT t_Length) noexcept;

		///<summary>安全清空</summary>
		void SafeClear() noexcept;

		///<summary>清空</summary>
		void Clear() noexcept;

		///<summary>改变DPI</summary>
		///<param name="t_DpiX">水平DPI</param>
		///<param name="t_DpiY">垂直DPI</param>
		void ChangeDPI(const float t_DpiX, const float t_DpiY);

		///<summary>改变大小</summary>
		void Resize();

		///<summary>刷新</summary>
		void Refresh();

		///<summary>渲染</summary>
		void Render();

	private:

		///<summary>示波器项结构</summary>
		struct OscilloscopeItemStruct
		{

			///<summary>示波器项结构</summary>
			~OscilloscopeItemStruct();

			///<summary>示波器项结构</summary>
			///<param name="t_Count">要呈现的数据量</param>
			///<param name="t_Range">量程</param>
			///<param name="t_GraphProperties">图形参数</param>
			///<param name="t_Zoom">缩放参数</param>
			OscilloscopeItemStruct(const UINT32 t_Count, const float t_Range, const GraphProperties* const t_GraphProperties, const ZoomProperties* const t_Zoom);

			///<summary>样式</summary>
			Enum_GraphStyle Style;

			///<summary>前景色</summary>
			UINT32 ForegroundValue;

			///<summary>前景色</summary>
			D2D1_COLOR_F Foreground;

			///<summary>缓存</summary>
			Cache* const DataCache;

			///<summary>图形颜色</summary>
			Microsoft::WRL::ComPtr<ID2D1SolidColorBrush> GraphColor;

			///<summary>折线样式</summary>
			Microsoft::WRL::ComPtr<ID2D1StrokeStyle> LineStyle;

			///<summary>折线图</summary>
			Microsoft::WRL::ComPtr<ID2D1PathGeometry> Geometry;

			///<summary>线宽</summary>
			float LineWeight;

			///<summary>是否启用</summary>
			bool IsEnabled;

		};

		///<summary>平移参数</summary>
		struct TranslateArgs
		{
			///<summary>平移参数</summary>
			///<param name="t_Args">参数</param>
			///<param name="t_Offset">差值</param>
			TranslateArgs(ZoomParameters* const t_Args, const int t_Offset = 0);
			///<summary>参数</summary>
			ZoomParameters* const Args;
			///<summary>位移</summary>
			int Translate;
		};

		///<summary>缩放参数</summary>
		struct ScaleArgs
		{
			///<summary>缩放参数</summary>
			///<param name="t_Args">参数</param>
			///<param name="t_Offset">差值</param>
			ScaleArgs(ZoomParameters* const t_Args, const float t_Offset = 0.0f);
			///<summary>参数</summary>
			ZoomParameters* const Args;
			///<summary>缩放率</summary>
			float Scale;
			///<summary>平移</summary>
			int Translate;
		};

		///<summary>临界区</summary>
		Concurrency::critical_section m_CriticalSection;

		///<summary>绘图区句柄</summary>
		HWND const m_Handle;

		///<summary>图层参数</summary>
		const D2D1_LAYER_PARAMETERS m_LayerParameters;

		///<summary>背景色</summary>
		UINT32 m_BackgroundValue;

		///<summary>背景色</summary>
		D2D1_COLOR_F m_Background;

		///<summary>刷新模式</summary>
		Enum_RefreshMode m_RefreshMode;

		///<summary>缩放</summary>
		ZoomProperties* const m_Zoom;

		///<summary>坐标轴</summary>
		AxisProperties* const m_Axis;

		///<summary>D2D工厂</summary>
		Microsoft::WRL::ComPtr<ID2D1Factory> const m_D2dFactory;

		///<summary>绘制目标</summary>
		Microsoft::WRL::ComPtr<ID2D1HwndRenderTarget> m_RenderTarget;

		///<summary>图层</summary>
		Microsoft::WRL::ComPtr<ID2D1Layer> m_Layer;

		///<summary>数据量</summary>
		const UINT m_DatalLength;

		///<summary>项数组</summary>
		std::unique_ptr<std::unique_ptr<OscilloscopeItemStruct>[]> const m_Items;

		///<summary>项数组个数</summary>
		const UINT m_ItemsSize;

		///<summary>绘图区宽度</summary>
		float m_Width;

		///<summary>绘图区高度</summary>
		float m_Height;

		///<summary>是否需要刷新</summary>
		std::atomic_bool m_IsNeedRefresh;

		///<summary>重建</summary>
		void m_Recreate();

		///<summary>获取绘图区尺寸</summary>
		D2D1_SIZE_U m_GetSize();

		///<summary>初始化</summary>
		///<param name="t_Args">参数</param>
		///<param name="t_Size">绘图区尺寸</param>
		void m_Initialize(ZoomParameters &t_Args, const float t_Size);

		///<summary>改变缩放极限值【单线程调用】</summary>
		///<param name="t_Args">参数</param>
		///<param name="t_Size">绘图区尺寸</param>
		///<param name="t_Limit">极限值</param>
		bool m_ChangeScaleLimit(ScaleArgs &t_Args, const float t_Size, const float t_Limit);

		///<summary>缩放【单线程调用】</summary>
		///<param name="t_Args">参数</param>
		///<param name="t_Size">绘图区尺寸</param>
		bool m_Scale(ScaleArgs &t_Args, const float t_Size);

		///<summary>平移【单线程调用】</summary>
		///<param name="t_Args">参数</param>
		bool m_Translate(TranslateArgs &t_Args);

		///<summary>渲染</summary>
		void m_Render();

		///<summary>绘制：扫描模式</summary>
		///<param name="t_Item">项</param>
		///<param name="t_Clip">裁切区</param>
		///<param name="t_Size">尺寸</param>
		///<param name="t_Area">区域</param>
		///<param name="t_Step">步长</param>
		void m_Draw_Scanning(OscilloscopeItemStruct* const t_Item, const D2D1_RECT_F &t_Clip, const D2D1_SIZE_F &t_Size, const D2D1_RECT_F &t_Area, const float t_Step);

		///<summary>绘制：滚动模式</summary>
		///<param name="t_Item">项</param>
		///<param name="t_Clip">裁切区</param>
		///<param name="t_Size">尺寸</param>
		///<param name="t_Area">区域</param>
		///<param name="t_Step">步长</param>
		void m_Draw_Scrolling(OscilloscopeItemStruct* const t_Item, const D2D1_RECT_F &t_Clip, const D2D1_SIZE_F &t_Size, const D2D1_RECT_F &t_Area, const float t_Step);

	};

}