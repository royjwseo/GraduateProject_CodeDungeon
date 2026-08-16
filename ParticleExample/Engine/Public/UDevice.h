#ifndef _PTH_FRAMEWORK_ENGIEN_PUBLIC_UDEVICE_H
#define _PTH_FRAMEWORK_ENGIEN_PUBLIC_UDEVICE_H

#include "UBase.h"

BEGIN(Engine)

class ENGINE_DLL DEF_CACHE_ALGIN UDevice : public UBase {
public:
	UDevice();
	NO_COPY(UDevice)
	DESTRUCTOR(UDevice)
public:
	const ComPtr<Dx12Device>& GetDV() const { return m_cpDevice; }
	const ComPtr<DxGIFactory>& GetFactory() const { return m_cpFactory; }

	virtual void Free() override;
	HRESULT NativeConstruct();
private:
	ComPtr<Dx12Device>		m_cpDevice;
	ComPtr<DxGIFactory>		m_cpFactory;
	ComPtr<IDXGIAdapter1>	m_cpAdaptor;
#ifdef _DEBUG
	ComPtr<ID3D12Debug>		m_cpDebug;
#endif 
};

END

#endif // _PTH_FRAMEWORK_ENGIEN_PUBLIC_UDEVICE_H