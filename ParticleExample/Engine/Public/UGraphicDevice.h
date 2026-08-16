#ifndef _PTH_FRAMEWORK_ENGIEN_PUBLIC_UGRAPHICDEVICE_H
#define _PTH_FRAMEWORK_ENGIEN_PUBLIC_UGRAPHICDEVICE_H
#include "UBase.h"

BEGIN(Engine)
class UTexture;
class UDevice;
class UGpuCommand;
class UComputeCommand;
class USwapChain;
class UGraphicsTableDescriptor;
class UGrobalConstantBuffer;
class UShaderConstantBuffer;
class URenderTargetGroup;
class URenderObject;

class UGraphicDevice : public UBase {
public:
	UGraphicDevice();
	NO_COPY(UGraphicDevice)
	DESTRUCTOR(UGraphicDevice)
public:
	CSHPTRREF<USwapChain> GetSwapChain() const { return m_spSwapChain; }

	const _bool IsGamePaused() const { return m_isGamePaused; }
	const _bool IsScreenMinimized() const { return m_isScreenMinimized; }
	const _bool IsScreenMaximized() const { return m_isScreenMaximized; }
	const _bool IsScreenFullScreen() const { return m_isScreenFullscreen; }
	const _bool IsMssa4xState() const { return m_isMssa4xState; }
	const _uint& GetMssa4xQuality() const { return m_iMssa4xQuality; }

	const D3D12_VIEWPORT& GetD3DViewport() const { return m_stD3DViewport; }
	const D3D12_RECT& GetD3DWindowSizeRect() const { return m_stD3DWindowSizeRect; }
	CSHPTRREF<GRAPHICDESC> GetGraphicDesc() const { return m_spGraphicDesc; }

	void SetDefaultRtGroup(CSHPTRREF<URenderTargetGroup> _rtGroup) { this->m_spDefaultRenderTargetGroup = _rtGroup; }
public:
	// Free
	virtual void	 Free() override;
	// Ready
	HRESULT	ReadyGraphicDevice(const GRAPHICDESC& _stGraphicsDesc, CSHPTRREF<URenderObject> _spRenderObject);
	void	 OmSetDefaultRenderTarget();
	SHPTR<UTexture> GetDefaultBackTexture();

	// Resize 
	HRESULT OnResize(const _uint& _iWinSizeX, const _uint& _iWinSizeY, const GRAPHICDESC::WINMODE _eWindowMode);
	// Clear BackBuffer View
	HRESULT MainRenderBegin();
	HRESULT MainRenderEnd();
private:
	void ResizeViewPort();
private:
	// DxClass
	SHPTR<URenderObject>								m_spGraphicRenderObject;
	SHPTR<UDevice>												m_spDevice;
	SHPTR<UGpuCommand>								m_spGpuCommand;

	SHPTR<USwapChain>										m_spSwapChain;
	// Default Rtv 
	SHPTR< URenderTargetGroup>					m_spDefaultRenderTargetGroup;
	// Bool Values
	_bool																	m_isGamePaused;
	_bool																	m_isScreenMinimized;
	_bool																	m_isScreenMaximized;
	_bool																	m_isScreenResize;
	_bool																	m_isScreenFullscreen;
	_bool																	m_isMssa4xState;
	_uint																	m_iMssa4xQuality;
	// ViewPort, Rect
	DEF_CACHE_ALGIN D3D12_VIEWPORT		m_stD3DViewport;
	DEF_CACHE_ALGIN D3D12_RECT					m_stD3DWindowSizeRect;
	// GraphicDesc
	SHPTR<GRAPHICDESC>									m_spGraphicDesc;
};

END

#endif // _PTH_FRAMEWORK_ENGIEN_PUBLIC_UGRAPHICDEVICE_H