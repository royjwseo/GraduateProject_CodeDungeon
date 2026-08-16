#ifndef _PTH_FRAMEWORK_ENGIEN_PUBLIC_USHADERBUFFERMANAGER_H
#define _PTH_FRAMEWORK_ENGIEN_PUBLIC_USHADERBUFFERMANAGER_H

#include "UBase.h"

BEGIN(Engine)
class UGrobalConstantBuffer;
class UShaderConstantBuffer;
class UDevice;
class UCommand;

class UShaderBufferManager : public UBase {
	using GROBALBUFFERCONTAINER = ARRAY<SHPTR<UGrobalConstantBuffer>, GROBAL_BUFFER_END>;
	using PREALLOCATEDCBUFFERCONTANER = ARRAY<SHPTR<UShaderConstantBuffer>, PREALLOCATED_BUFFER_END>;
public:
	UShaderBufferManager();
	NO_COPY(UShaderBufferManager)
	DESTRUCTOR(UShaderBufferManager)
public:
	virtual void Free() override;
public:
	HRESULT ReadyShaderBufferManager(CSHPTRREF<UDevice> _spDevice);

	void	 BindGrobalBuffer(const GROBAL_CBUFFERTYPE _eGrobalCBuffer, CSHPTRREF<UCommand> _spCommand ,const void* _pBuffer, const _uint _iSize);
	HRESULT GetGrobalConstantBuffer(const GROBAL_CBUFFERTYPE _eGrobalCBuffer, SHPTRREF<UGrobalConstantBuffer> _spGrobalConstantBuffer);
	HRESULT GetPreAllocatedConstantBuffer(const PREALLOCATED_CBUFFERTYPE _ePreAllocatedCBufferType, SHPTRREF<UShaderConstantBuffer> _spShaderConstantBuffer);
private:
	// CreateGrobalConstantBuffer
	HRESULT CreateGrobalConstantBuffer(CSHPTRREF<UDevice> _spDevice, const GROBAL_CBUFFERTYPE _eType, 
		const CBV_REGISTER& _eReg, const _uint& _iBufferSize);
	// CreateGrobalConstantBuffer
	HRESULT CreateShaderConstantBuffer(CSHPTRREF<UDevice> _spDevice, const PREALLOCATED_CBUFFERTYPE _eType,
		const CBV_REGISTER& _eReg, const _uint& _iBufferSize, const _uint _iCnt);
private:
	// GrobalBufferContainer
	GROBALBUFFERCONTAINER					m_arrGrobalConstantBufferContainer;
	PREALLOCATEDCBUFFERCONTANER	m_arrPreAllocatedCBufferContainer;
};

END

#endif // _PTH_FRAMEWORK_ENGIEN_PUBLIC_USHADERBUFFERMANAGER_H