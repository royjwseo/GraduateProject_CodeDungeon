#include "EngineDefines.h"
#include "UGraphicDevice.h"
#include "UDevice.h"
#include "UGpuCommand.h"
#include "USwapChain.h"
#include "URootSignature.h"
#include "UGraphicsTableDescriptor.h"
#include "URenderTargetGroup.h"
#include "UTexture.h"
#include "URenderObject.h"
#include "UShaderConstantBuffer.h"

UGraphicDevice::UGraphicDevice() :
	m_spGraphicRenderObject{ nullptr },
	m_spDevice{ nullptr },
	m_spGpuCommand{nullptr},
	m_spSwapChain{ nullptr },
	m_spDefaultRenderTargetGroup{ nullptr },
	m_isGamePaused{ false },
	m_isScreenMinimized{ false },
	m_isScreenMaximized{ false },
	m_isScreenResize{ false },
	m_isScreenFullscreen{ false },
	m_isMssa4xState{ false },
	m_iMssa4xQuality{ 0 },
	m_stD3DViewport{},
	m_stD3DWindowSizeRect{},
	m_spGraphicDesc{ nullptr }
{
}

void UGraphicDevice::Free()
{
}

HRESULT UGraphicDevice::ReadyGraphicDevice(const GRAPHICDESC& _stGraphicsDesc, CSHPTRREF<URenderObject> _spRenderObject)
{
	m_spGraphicDesc = std::make_shared<GRAPHICDESC>(_stGraphicsDesc);
	// Create DxObject Device Gpu 
	{
		m_spGraphicRenderObject = _spRenderObject;
		m_spDevice = m_spGraphicRenderObject->GetDevice();
		m_spGpuCommand = static_pointer_cast<UGpuCommand>(m_spGraphicRenderObject->GetCommand());
	}
	// CheckMssa
	{
		D3D12_FEATURE_DATA_MULTISAMPLE_QUALITY_LEVELS msQaulityLevels;
		msQaulityLevels.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		msQaulityLevels.SampleCount = 4;
		msQaulityLevels.Flags = D3D12_MULTISAMPLE_QUALITY_LEVELS_FLAG_NONE;
		msQaulityLevels.NumQualityLevels = 0;

		RETURN_CHECK_DXOBJECT(m_spDevice->GetDV()->CheckFeatureSupport(D3D12_FEATURE_MULTISAMPLE_QUALITY_LEVELS, &msQaulityLevels,
			sizeof(msQaulityLevels)), E_FAIL);

		m_iMssa4xQuality = msQaulityLevels.NumQualityLevels;
		m_isMssa4xState = m_iMssa4xQuality >= 1 ? true : false;
		assert(m_iMssa4xQuality > 0 && "Unexpected MSAA Qaulity Level.");
		// Erase
		m_isMssa4xState = false;
	}
	// CreateViewPort
	{
		ResizeViewPort();
	}
	// Create DxObject
	{
		// swapChain
		m_spSwapChain = CreateNative<USwapChain>(*m_spGraphicDesc.get(), m_iMssa4xQuality, m_isMssa4xState,
			m_spDevice, m_spGpuCommand);
	}
	return S_OK;
}

void UGraphicDevice::OmSetDefaultRenderTarget()
{
	m_spDefaultRenderTargetGroup->OmSetRenderTargets(1, m_spSwapChain->GetBackBufferIndex(), m_spGraphicRenderObject);
}

SHPTR<UTexture> UGraphicDevice::GetDefaultBackTexture()
{
	return m_spDefaultRenderTargetGroup->GetRenderTargetTexture(m_spSwapChain->GetBackBufferIndex());
}

HRESULT UGraphicDevice::OnResize(const _uint& _iWinSizeX, const _uint& _iWinSizeY, const GRAPHICDESC::WINMODE _eWindowMode)
{
	m_spGraphicDesc->iWinCX = _iWinSizeX;
	m_spGraphicDesc->iWinCY = _iWinSizeY;
	m_spGraphicDesc->eWinMode = _eWindowMode;

	m_spSwapChain.reset();
	m_spSwapChain = CreateNative<USwapChain>(*m_spGraphicDesc.get(), m_iMssa4xQuality, m_isMssa4xState,
		m_spDevice, m_spGpuCommand);

	ResizeViewPort();
	return S_OK;
}

HRESULT UGraphicDevice::MainRenderBegin()
{
	UGpuCommand::FRAMECONTEXT* FrameContext = m_spGpuCommand->WaitForNextFrameResources(
		m_spSwapChain->GewtSwapChainWaitableObject(), m_spSwapChain->GetBackBufferIndex());

	ComPtr<Dx12GraphicsCommandList> pGraphicCmdList = m_spGpuCommand->GetGpuCmdList();

	SHPTR<UTexture> pDefaultTexture =
		m_spDefaultRenderTargetGroup->GetRenderTargetTexture(m_spSwapChain->GetBackBufferIndex());

	// 재설정은 GPU가 관련 명령 목록들을 모두 처리한 후에 일어난다. 
	RETURN_CHECK_FAILED(FrameContext->cpAllocator->Reset(), E_FAIL);
	// 명령 목록을 대기열에 추가했다면 명령 목록을 재설정해야 한다. 
	RETURN_CHECK_FAILED(pGraphicCmdList->Reset(FrameContext->cpAllocator.Get(), nullptr), E_FAIL);

	// Set Root Signature
	m_spGraphicRenderObject->BeginRendering();

	D3D12_RESOURCE_BARRIER Barrier = CD3DX12_RESOURCE_BARRIER::Transition(pDefaultTexture->GetTex().Get(),
		D3D12_RESOURCE_STATE_PRESENT, D3D12_RESOURCE_STATE_RENDER_TARGET);
	pGraphicCmdList->ResourceBarrier(1, &Barrier);
	// Set the viewport and scissor rect.  This needs to be reset whenever the command list is reset.
	pGraphicCmdList->RSSetViewports(1, &m_stD3DViewport);
	pGraphicCmdList->RSSetScissorRects(1, &m_stD3DWindowSizeRect);

	m_spDefaultRenderTargetGroup->ClearRenderTargetView(m_spSwapChain->GetBackBufferIndex(), m_spGraphicRenderObject);
	m_spDefaultRenderTargetGroup->OmSetRenderTargets(1, m_spSwapChain->GetBackBufferIndex(), m_spGraphicRenderObject);
	return S_OK;
}

HRESULT UGraphicDevice::MainRenderEnd() 
{
	ComPtr<Dx12CommandQueue> pCmdQueue = m_spGpuCommand->GetCmdQue();
	ComPtr<Dx12GraphicsCommandList> pGraphicCmdList = m_spGpuCommand->GetGpuCmdList();

	SHPTR<UTexture> pDefaultTexture =
		m_spDefaultRenderTargetGroup->GetRenderTargetTexture(m_spSwapChain->GetBackBufferIndex());

	D3D12_RESOURCE_BARRIER Barrier = CD3DX12_RESOURCE_BARRIER::Transition(pDefaultTexture->GetTex().Get(),
		D3D12_RESOURCE_STATE_RENDER_TARGET, D3D12_RESOURCE_STATE_PRESENT);
	pGraphicCmdList->ResourceBarrier(1, &Barrier);
	pGraphicCmdList->Close();

	// 커맨드 리스트 수행
	ID3D12CommandList* cmdListArr[] = { pGraphicCmdList.Get() };
	pCmdQueue->ExecuteCommandLists(_countof(cmdListArr), cmdListArr);

	m_spSwapChain->Present();
	m_spGpuCommand->WaitForEndFrameResource();
	m_spSwapChain->SwapChainIndx();
	return S_OK;
}

void UGraphicDevice::ResizeViewPort()
{
	// CreateViewPort
	m_stD3DViewport.TopLeftX = 0;
	m_stD3DViewport.TopLeftY = 0;
	m_stD3DViewport.Width = static_cast<_float>(m_spGraphicDesc->iWinCX);
	m_stD3DViewport.Height = static_cast<_float>(m_spGraphicDesc->iWinCY);
	m_stD3DViewport.MinDepth = 0.f;
	m_stD3DViewport.MaxDepth = 1.f;

	m_stD3DWindowSizeRect = CD3DX12_RECT(0, 0, (LONG)(m_spGraphicDesc->iWinCX), (LONG)(m_spGraphicDesc->iWinCY));
}
