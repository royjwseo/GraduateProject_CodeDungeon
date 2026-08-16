#ifndef _PTH_FRAMEWORK_ENGIEN_PUBLIC_UROOTSIGNATURE_H
#define _PTH_FRAMEWORK_ENGIEN_PUBLIC_UROOTSIGNATURE_H

#include "UBase.h"

BEGIN(Engine)
class UDevice;
class UGpuCommand;

class DEF_CACHE_ALGIN URootSignature : public  UBase {
public:
	enum SAMPLERTYPE
	{
		S0, S1, S_END
	};
	using SAMPLERS = ARRAY< D3D12_STATIC_SAMPLER_DESC, S_END>;
public:
	URootSignature();
	NO_COPY(URootSignature)
	DESTRUCTOR(URootSignature)
public:
	const ComPtr<Dx12RootSignature>& GetRootSignature() const { return m_cpRootSignature; }
	const _uint GetRegisterSpace() const { return m_iRegisterSpace; }
public:
	virtual void Free() override;
	HRESULT NativeConstruct(CSHPTRREF<UDevice> _spDevice, const _uint _iRegisterSpace = 0);
protected:
	const SAMPLERS& GetSamplers() const { return m_arrSamplerDescs; }

	virtual HRESULT CreateRootSignature(CSHPTRREF<UDevice> _spDevice);
	HRESULT Create(CSHPTRREF<UDevice> _spDevice, const ComPtr<DxBlob>& _spBlob);
private:
	void NativeSampler();
private:
	_uint												m_iRegisterSpace;
	ComPtr<Dx12RootSignature>	m_cpRootSignature;

	SAMPLERS									m_arrSamplerDescs;
};

END

#endif // _PTH_FRAMEWORK_ENGIEN_PUBLIC_UROOTSIGNATURE_H