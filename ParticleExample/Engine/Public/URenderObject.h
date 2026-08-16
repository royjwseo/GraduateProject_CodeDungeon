#ifndef _PTH_FRAMEWORK_ENGIEN_PUBLIC_URENDERTHREADOBJECT_H
#define _PTH_FRAMEWORK_ENGIEN_PUBLIC_URENDERTHREADOBJECT_H

#include "UBase.h"

BEGIN(Engine)
class UCommand;
class UTableDescriptor;
class URootSignature;
class UDevice;

class DEF_CACHE_ALGIN URenderObject final : public UBase {
public:
	URenderObject();
	NO_COPY(URenderObject)
	DESTRUCTOR(URenderObject)
public:
	CSHPTRREF<UDevice> GetDevice() const { return m_spDevice; }
	CSHPTRREF<URootSignature>	 GetRootSignature() const { return m_spRootSignature; }
	CSHPTRREF<UTableDescriptor> GetTableDescriptor() const { return m_spTableDescriptor; }
	CSHPTRREF<UCommand> GetCommand() const { return m_spCommand; }
public:
	virtual void Free() override;
	HRESULT NativeConstruct(CSHPTRREF<UDevice> _spDevice, CSHPTRREF<URootSignature> _spRootSignature,
		CSHPTRREF<UTableDescriptor> _spTableDescriptor, CSHPTRREF<UCommand> _spCommand);

	void BeginRendering();
	void BeginRenderTareget(const D3D12_VIEWPORT& _d3dViewPort, const D3D12_RECT& _d3dRect);
	void BindRenderTargetValues(const D3D12_VIEWPORT& _d3dViewPort, const D3D12_RECT& _d3dRect);
	// End
	void ExcuteCommand();
private:
	static constexpr	_uint			RENDER_OBJECT_VALUE{1};

	SHPTR<UDevice>						m_spDevice;
	SHPTR<URootSignature>		m_spRootSignature;
	SHPTR<UTableDescriptor>	m_spTableDescriptor;
	SHPTR<UCommand>				m_spCommand;
};

END

#endif // _PTH_FRAMEWORK_ENGIEN_PUBLIC_URENDERTHREADOBJECT_H