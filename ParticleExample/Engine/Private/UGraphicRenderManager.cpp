#include "EngineDefines.h"
#include "UGraphicRenderManager.h"
#include "UComputeRootSignature.h"
#include "UTableDescriptor.h"
#include "UComputeTableDescriptor.h"
#include "UComputeCommand.h"
#include "URenderCommand.h"
#include "UDevice.h"
#include "UGraphicsTableDescriptor.h"
#include "URenderObject.h"
#include "UGpuCommand.h"
#include "ULightRootSignature.h"
#include "UMethod.h"
#include "UGameInstance.h"
#include "UGpuCommand.h"

UGraphicRenderManager::UGraphicRenderManager()  :
	m_spDevice{ nullptr },
	m_spGpuCommand{nullptr},
	m_cpGraphicCommandQueue{ nullptr },
	m_cpComputeCommandQueue{ nullptr },
	m_arrRootRegisters{}
{
}

SHPTR<URootSignature> UGraphicRenderManager::GetRootSignature(const ROOT_REGISTER_TYPE _eSpace)
{
	_uint iSpace{ static_cast<_uint>(_eSpace) };
	if (MAX_ROOT_REGISTER_CNT <= iSpace)
	{
		return nullptr;
	}

	return m_arrRootRegisters[iSpace];
}

SHPTR<URenderObject> UGraphicRenderManager::GetRenderObject(const GRAPHICRENDEROBJECT_TYPE _eType)
{
	_uint iType{ static_cast<_uint>(_eType) };
	if (MAX_RENDEROBJECT_CNT <= iType)
	{
		return nullptr;
	}

	return m_arrRenderObjectConatiners[iType];
}

void UGraphicRenderManager::Free()
{
	m_cpGraphicCommandQueue.Reset();
	m_cpComputeCommandQueue.Reset();

	m_arrRootRegisters;
	m_arrRenderObjectConatiners;
}

HRESULT UGraphicRenderManager::ReadyGraphicRenderManager(OUTPUTDATA& _stOutData, const _uint _iThreadCnt)
{
	m_spDevice = CreateNative<UDevice>();
	RETURN_CHECK(nullptr == m_spDevice, E_FAIL);

	{
		D3D12_COMMAND_QUEUE_DESC GraphicsCommandQueue{};
		GraphicsCommandQueue.Type = D3D12_COMMAND_LIST_TYPE_DIRECT;
		GraphicsCommandQueue.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;
		RETURN_CHECK_DXOBJECT(m_spDevice->GetDV()->CreateCommandQueue(&GraphicsCommandQueue,
			IID_PPV_ARGS(&m_cpGraphicCommandQueue)), E_FAIL);
	}
	{
		D3D12_COMMAND_QUEUE_DESC ComputeQueuDesc{};
		ComputeQueuDesc.Type = D3D12_COMMAND_LIST_TYPE_COMPUTE;
		ComputeQueuDesc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;
		RETURN_CHECK_DXOBJECT(m_spDevice->GetDV()->CreateCommandQueue(&ComputeQueuDesc,
			IID_PPV_ARGS(&m_cpComputeCommandQueue)), E_FAIL);
	}

	RETURN_CHECK_FAILED(CreateRootSignature(), E_FAIL);
	RETURN_CHECK_FAILED(CreateRenderObject(_iThreadCnt), E_FAIL);
	{
		SHPTR<URenderObject> spRenderObject = GetRenderObject(GRAPHICRENDEROBJECT_TYPE::COMMON);
		if (nullptr != spRenderObject)
		{
			_stOutData.wpDevice = m_spDevice;
			_stOutData.wpGpuCmd = spRenderObject->GetCommand();
		}
	}

	return S_OK;
}

HRESULT UGraphicRenderManager::CreateRootSignature()
{
	{
		_uint iRegister = static_cast<_uint>(ROOT_REGISTER_TYPE::COMMON);
		m_arrRootRegisters[iRegister] = CreateNative<URootSignature>(m_spDevice, ROOTREGISTER_SPACE_COMMON);
	}
	{
		_uint iRegister = static_cast<_uint>(ROOT_REGISTER_TYPE::GRAPHI_LIGHT);
		m_arrRootRegisters[iRegister] = CreateNative<ULightRootSignature>(m_spDevice, ROOTREGISTER_SPACE_COMMON);
	}
	{
		_uint iRegister = static_cast<_uint>(ROOT_REGISTER_TYPE::COMPUTE_COMMON);
		m_arrRootRegisters[iRegister] = CreateNative<UComputeRootSignature>(m_spDevice, ROOTREGISTER_SPACE_COMMON);
	}

	return S_OK;
}

HRESULT UGraphicRenderManager::CreateRenderObject(const _uint _iThreadCnt)
{
	// Graphics Common
	{
		const _uint MAX_GRAPHIC_TABLES = 1'0000;

		m_spGpuCommand = CreateNative<UGpuCommand>(m_spDevice, m_cpGraphicCommandQueue);

		SHPTR<UGraphicsTableDescriptor> spTableDescriptor = CreateNative<UGraphicsTableDescriptor>(m_spDevice, MAX_GRAPHIC_TABLES,
			CBV_REGISTER_END + SRV_REGISTER_END, GRAPHICS_CONSTANT_BUFFER_VALUE);

		_uint iRenderObjectID = static_cast<_uint>(GRAPHICRENDEROBJECT_TYPE::COMMON);

		m_arrRenderObjectConatiners[iRenderObjectID] =	CreateNative<URenderObject>(m_spDevice, 
			m_arrRootRegisters[static_cast<_uint>(ROOT_REGISTER_TYPE::COMMON)], spTableDescriptor, m_spGpuCommand);
	}
	// Graphics Light 
	{
		ComPtr<Dx12CommandQueue> cpCommandQueue{nullptr};
		D3D12_COMMAND_QUEUE_DESC GraphicsCommandQueue{};
		GraphicsCommandQueue.Type = D3D12_COMMAND_LIST_TYPE_DIRECT;
		GraphicsCommandQueue.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;
		RETURN_CHECK_DXOBJECT(m_spDevice->GetDV()->CreateCommandQueue(&GraphicsCommandQueue,
			IID_PPV_ARGS(&cpCommandQueue)), E_FAIL;);

		const _uint MAX_GRAPHIC_TABLES  = 100;

		SHPTR<UCommand> spCommand = CreateNative<URenderCommand>(m_spDevice, cpCommandQueue);
		SHPTR<UGraphicsTableDescriptor> spTableDescriptor = CreateNative<UGraphicsTableDescriptor>(m_spDevice, MAX_GRAPHIC_TABLES,
			LIGHT_GRAPHICS_REGISTER_CNT, LIGHT_CONSTANT_BUFFER_VALUE);

		_uint iRenderObjectID = static_cast<_uint>(GRAPHICRENDEROBJECT_TYPE::LIGHT);

		m_arrRenderObjectConatiners[iRenderObjectID] = CreateNative<URenderObject>(m_spDevice,
			m_arrRootRegisters[static_cast<_uint>(ROOT_REGISTER_TYPE::GRAPHI_LIGHT)], spTableDescriptor, spCommand);
	}
	// RenderTargets
	{
		const _uint MAX_GRAPHIC_TABLES = 1'0000;

		for (_uint i = 0; i < 2; ++i)
		{
			ComPtr<Dx12CommandQueue> cpCommandQueue{ nullptr };
			D3D12_COMMAND_QUEUE_DESC GraphicsCommandQueue{};
			GraphicsCommandQueue.Type = D3D12_COMMAND_LIST_TYPE_DIRECT;
			GraphicsCommandQueue.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;
			RETURN_CHECK_DXOBJECT(m_spDevice->GetDV()->CreateCommandQueue(&GraphicsCommandQueue,
				IID_PPV_ARGS(&cpCommandQueue)), E_FAIL;);

			SHPTR<URenderCommand> spRenderCommand = CreateNative<URenderCommand>(m_spDevice, cpCommandQueue);

			SHPTR<UGraphicsTableDescriptor> spTableDescriptor = CreateNative<UGraphicsTableDescriptor>(m_spDevice, MAX_GRAPHIC_TABLES,
				CBV_REGISTER_END + SRV_REGISTER_END, GRAPHICS_CONSTANT_BUFFER_VALUE);

			_uint iRenderObjectID = static_cast<_uint>(GRAPHICRENDEROBJECT_TYPE::ALPHA) + i;

			m_arrRenderObjectConatiners[iRenderObjectID] = CreateNative<URenderObject>(m_spDevice,
				CreateNative<URootSignature>(m_spDevice, ROOTREGISTER_SPACE_COMMON), spTableDescriptor, spRenderCommand);
		}
	}
	return S_OK;
}
