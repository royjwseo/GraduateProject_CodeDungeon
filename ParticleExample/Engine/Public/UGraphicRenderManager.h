#ifndef _PTH_FRAMEWORK_ENGIEN_PUBLIC_UGRAPHICRENDERMANAGER_H
#define _PTH_FRAMEWORK_ENGIEN_PUBLIC_UGRAPHICRENDERMANAGER_H

#include "UBase.h"

BEGIN(Engine)
class URootSignature;
class UTableDescriptor;
class UCommand;
class UGpuCommand;
class UActor;
class UDevice;
class UGraphicDevice;
class URenderObject;
class UThreadPoolManager;

class UGraphicRenderManager : public UBase {
public:
	UGraphicRenderManager();
	NO_COPY(UGraphicRenderManager)
	DESTRUCTOR(UGraphicRenderManager)
public:
	using ROOTREGISTERS = ARRAY<SHPTR<URootSignature>,MAX_ROOT_REGISTER_CNT>;
	using RENDEROBJECTCONTAINERS = ARRAY <SHPTR<URenderObject>, MAX_RENDEROBJECT_CNT>;

public:
	CSHPTRREF<UDevice> GetDevice() const { return m_spDevice; }
	CSHPTRREF<UGpuCommand> GetGpuCommand() const { return m_spGpuCommand; }
	SHPTR<URootSignature> GetRootSignature(const ROOT_REGISTER_TYPE _eSpace);
	SHPTR<URenderObject> GetRenderObject(const GRAPHICRENDEROBJECT_TYPE _eType);
	const ComPtr<Dx12CommandQueue>& GetGpuCommandQueue() const { return m_cpGraphicCommandQueue; }
	const ComPtr<Dx12CommandQueue>& GetComputeCommandQueue() const { return m_cpComputeCommandQueue; }
public:
	virtual void Free() override;
	HRESULT ReadyGraphicRenderManager(OUTPUTDATA& _stOutData, const _uint _iThreadCnt);
private:
	HRESULT CreateRootSignature();
	HRESULT CreateRenderObject(const _uint _iThreadCnt);
private:
	// Device
	SHPTR<UDevice>											m_spDevice;
	SHPTR<UGpuCommand>							m_spGpuCommand;

	ComPtr<Dx12CommandQueue>				m_cpGraphicCommandQueue;
	ComPtr<Dx12CommandQueue>				m_cpComputeCommandQueue;
	// Root Registers 
	ROOTREGISTERS											m_arrRootRegisters;
	RENDEROBJECTCONTAINERS					m_arrRenderObjectConatiners;
};

END

#endif //_PTH_FRAMEWORK_ENGIEN_PUBLIC_UGRAPHICRENDERMANAGER_H
