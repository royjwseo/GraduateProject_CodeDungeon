#include "EngineDefines.h"
#include "URenderObject.h"
#include "UDevice.h"
#include "URootSignature.h"
#include "UTableDescriptor.h"
#include "URenderCommand.h"

URenderObject::URenderObject() :
	m_spDevice{ nullptr },
	m_spRootSignature{ nullptr },
	m_spTableDescriptor{ nullptr },
	m_spCommand{ nullptr }
{
}

void URenderObject::Free()
{
}

HRESULT URenderObject::NativeConstruct(CSHPTRREF<UDevice> _spDevice, CSHPTRREF<URootSignature> _spRootSignature, 
	CSHPTRREF<UTableDescriptor> _spTableDescriptor, CSHPTRREF<UCommand> _spCommand)
{
	RETURN_CHECK(nullptr == _spDevice || nullptr == _spRootSignature || nullptr == _spTableDescriptor ||
		nullptr == _spCommand, E_FAIL);

	m_spDevice = _spDevice;
	m_spRootSignature = _spRootSignature;
	m_spTableDescriptor = _spTableDescriptor;
	m_spCommand = _spCommand;
	return S_OK;
}


void URenderObject::BeginRendering()
{
	//m_spCommand->Clear();
	// Bind And Clear 
	m_spTableDescriptor->BindAndClear(m_spCommand);
	// Root Signature
	m_spCommand->BindRootSignature(m_spRootSignature);
}

void URenderObject::BeginRenderTareget(const D3D12_VIEWPORT& _d3dViewPort, const D3D12_RECT& _d3dRect)
{
	// Bind RT Target
	m_spCommand->Clear();

	BeginRendering();

	BindRenderTargetValues(_d3dViewPort, _d3dRect);
}

void URenderObject::BindRenderTargetValues( const D3D12_VIEWPORT& _d3dViewPort, const D3D12_RECT& _d3dRect)
{
	m_spCommand->GetGpuCmdList()->RSSetViewports(RENDER_OBJECT_VALUE, &_d3dViewPort);
	m_spCommand->GetGpuCmdList()->RSSetScissorRects(RENDER_OBJECT_VALUE, &_d3dRect);
}

void URenderObject::ExcuteCommand()
{
	m_spCommand->WaitForSynchronization();
}
