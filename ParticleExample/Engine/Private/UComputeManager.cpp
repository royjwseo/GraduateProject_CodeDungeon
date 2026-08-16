#include "EngineDefines.h"
#include "UComputeManager.h"
#include "UGraphicDevice.h"
#include "UComputeCommand.h"
#include "UComputeTableDescriptor.h"
#include "UComputeRootSignature.h"
#include "UComputeShader.h"
#include "UTexture.h"
#include "UDevice.h"
#include "UGpuCommand.h"
#include "UGameInstance.h"
#include "UGraphicRenderManager.h"
#include "URenderObject.h"
#include "UShaderConstantBuffer.h"

#ifdef _USE_DEBUGGING

#include "UVIBufferRect.h"
#include "UTransform.h"
#include "UShader.h"

#endif 

UComputeManager::UComputeManager() : 
	m_spDevice{ nullptr },
	m_spGpuCommand{ nullptr },
	m_ComputeShader{},
	m_vecTextures{}
#ifdef _USE_DEBUGGING
	, m_vecComputeDebugs{},
	m_spTransformConstantBuffer{ nullptr },
	m_spVIBufferRect{nullptr},
	m_spFinalShader{nullptr}
#endif 
{
}

void UComputeManager::Free()
{
}

HRESULT UComputeManager::NativeComputeManager(CSHPTRREF<URenderObject> _spGpuRenderObject)
{
	RETURN_CHECK(nullptr == _spGpuRenderObject, E_FAIL);

	m_spDevice = _spGpuRenderObject->GetDevice();
	m_spGpuCommand = static_pointer_cast<UGpuCommand>(_spGpuRenderObject->GetCommand());


	

	return S_OK;
}

HRESULT UComputeManager::ReadyComputeManager(CSHPTRREF<UGraphicDevice> _spGraphicDevice)
{
	SHPTR<UGameInstance> spGameInstance = GET_INSTANCE(UGameInstance);
	CSHPTRREF<GRAPHICDESC> GraphicDesc = _spGraphicDevice->GetGraphicDesc();
	// Create 
	{
		m_vecTextures.resize(TEXTUREORDER::TEXTURE_END);
		{
			// Draw Texture Type
			TEXTURETYPEDESC typeDesc = TEXTURETYPEDESC{ DXGI_FORMAT::DXGI_FORMAT_R8G8B8A8_UNORM,
				1024, 1024, { 0.f, 0.f, 0.f, 1.f },
				D3D12_HEAP_TYPE_DEFAULT, D3D12_HEAP_FLAG_NONE,
				D3D12_RESOURCE_FLAGS::D3D12_RESOURCE_FLAG_ALLOW_UNORDERED_ACCESS };

			m_vecTextures[TEXTUREORDER::COMPUTE_ANIMTEXTURE] = CreateNative<UTexture>(m_spDevice, typeDesc);

			m_ComputeShader.insert(std::pair<_wstring, CSHPTR<UComputeShader>>(PROTO_RES_COMPUTEANIMATIONSHADER,
				static_pointer_cast<UComputeShader>(spGameInstance->CloneResource(PROTO_RES_COMPUTEANIMATIONSHADER))));
		}
	}
	
	if (nullptr != m_spComputeRenderObject)
	{
		m_spComputeRenderObject->BeginRendering();

		ComputeTexture(COMPUTE_ANIMTEXTURE);
	}

	

#ifdef _USE_DEBUGGING
	m_spTransformConstantBuffer = CreateNative<UShaderConstantBuffer>(m_spDevice, CBV_REGISTER::TRANSFORM, UTransform::TRANSFORMPARAM_SIZE);
	m_spVIBufferRect = static_pointer_cast<UVIBufferRect>(spGameInstance->CloneResource(PROTO_RES_VIBUFFERRECT));
	m_spFinalShader = static_pointer_cast<UShader>(spGameInstance->CloneResource(PROTO_RES_FINALDEFFEREDSHADER));
#endif 
	return S_OK;
}

void UComputeManager::ComputeTexture(const TEXTUREORDER& _eOrder)
{
	switch (_eOrder)
	{
	case COMPUTE_ANIMTEXTURE:
		break;
	}
}

#ifdef _USE_DEBUGGING

void UComputeManager::OnResizeDebugRenderObject(CSHPTRREF<UGraphicDevice> _spGraphicDevice)
{
	COMPUTEDEBUGS ComputeDebugs = m_vecComputeDebugs;
	m_vecComputeDebugs.clear();
	m_vecComputeDebugs.reserve(ComputeDebugs.size());

	for (auto& iter : ComputeDebugs)
	{
		AddDebugingObject(iter.eTextureOrder, iter.vPos, iter.vSize, _spGraphicDevice->GetGraphicDesc());
	}
}

#endif 

SHPTR<UComputeShader> UComputeManager::FindShader(const _wstring& _wstrShaderProtoName)
{
	const COMPUTESHADERS::iterator& iter = m_ComputeShader.find(_wstrShaderProtoName);
	RETURN_CHECK(iter == m_ComputeShader.end(), nullptr);
	return iter->second;
}

#ifdef _USE_DEBUGGING

void UComputeManager::AddDebugingObject(const TEXTUREORDER& _eTextureOrder, const _float2& _vPos, const _float2& _vScale,
	CSHPTRREF<GRAPHICDESC> _spGraphicDesc)
{
	_float4x4 WorldMatrix;
	WorldMatrix._11 = _vScale.x;
	WorldMatrix._22 = _vScale.y;

	WorldMatrix._41 = _vPos.x - _spGraphicDesc->iWinCX * 0.5f;
	WorldMatrix._42 = -_vPos.y + _spGraphicDesc->iWinCY * 0.5f;

	m_vecComputeDebugs.push_back({ _eTextureOrder, WorldMatrix.Transpose(), _vScale, _vPos });
}

void UComputeManager::RenderDebugObjects()
{
	// Setting PipeLine
	m_spFinalShader->SettingPipeLineState(m_spComputeRenderObject->GetCommand());

	for (auto& iter : m_vecComputeDebugs)
	{
		m_spFinalShader->BindCBVBuffer(m_spTransformConstantBuffer, &iter.stTransformParam, UTransform::TRANSFORMPARAM_SIZE);
		m_spFinalShader->BindSRVBuffer(SRV_REGISTER::T0, m_vecTextures[iter.eTextureOrder]);
		m_spVIBufferRect->Render(m_spFinalShader, m_spComputeRenderObject->GetCommand());
	}
}

#endif 
