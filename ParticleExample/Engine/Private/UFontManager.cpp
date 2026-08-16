#include "EngineDefines.h"
#include "UFontManager.h"
#include "URenderObject.h"
#include "UCommand.h"
#include "UDevice.h"
#include "URenderTargetManager.h"
#include "UFont.h"
#include "USwapChain.h"
#include <DirectXTK/ResourceUploadBatch.h>

UFontManager::UFontManager() : 
	m_FontContainer{},
	m_FontDrawList{},

	m_spRenderObject{ nullptr },
	m_pDescriptor{ nullptr },
	m_pGraphicMemory{ nullptr },

	m_iFontCnt{ 0 },
	m_RemainFontDescriptorIndex{},

	m_spSpriteBatch{ nullptr },
	m_tViewPort{},
	FONT_MAX_SIZE{ 50 }
{
}

void UFontManager::Free()
{
}

HRESULT UFontManager::ReadyFontManager(CSHPTRREF<URenderObject> _spRenderObject, CSHPTRREF<USwapChain> _spSwapChain, 
	CSHPTRREF<URenderTargetManager>_spRenderTargetManager, const D3D12_VIEWPORT& _viewPort)
{
    m_spRenderObject = _spRenderObject;
    m_tViewPort = _viewPort;

    m_pDescriptor = std::make_shared<DirectX::DescriptorHeap>(m_spRenderObject->GetDevice()->GetDV().Get(), D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV,
        D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE, FONT_MAX_SIZE);

    m_pGraphicMemory = std::make_shared<DirectX::GraphicsMemory>(m_spRenderObject->GetDevice()->GetDV().Get());
    DXGI_SWAP_CHAIN_DESC1 tDesc = _spSwapChain->GetSwapChainDesc();

    DirectX::ResourceUploadBatch Batch(m_spRenderObject->GetDevice()->GetDV().Get());

    Batch.Begin();

    DirectX::RenderTargetState rtState{ tDesc.Format, _spRenderTargetManager->GetDepthFormat() };

    DirectX::SpriteBatchPipelineStateDescription pipeLine{ rtState };

    m_spSpriteBatch = std::make_shared<DirectX::SpriteBatch>(m_spRenderObject->GetDevice()->GetDV().Get(), Batch, pipeLine,
        &m_tViewPort);
    auto upLoadResourceFinished = Batch.End(m_spRenderObject->GetCommand()->GetCmdQue().Get());
    upLoadResourceFinished.wait();
    RETURN_CHECK(!upLoadResourceFinished.valid(), E_FAIL);
    return S_OK;
}

HRESULT UFontManager::FontCreate(const _wstring& _strFontName, const _wstring& _strPath)
{
    if (m_FontContainer.size() >= FONT_MAX_SIZE)
        return S_OK;

    SHPTR<UFont> pFont = nullptr;

    if (m_RemainFontDescriptorIndex.empty()) {
        pFont = CreateNative<UFont>(m_spRenderObject, m_pDescriptor,m_iFontCnt, _strPath);
    }
    else {
        std::list<_uint>::iterator it = m_RemainFontDescriptorIndex.begin();
        pFont = CreateNative<UFont>(m_spRenderObject, m_pDescriptor,  *it, _strPath);
        m_RemainFontDescriptorIndex.erase(it);
    }

    RETURN_CHECK(nullptr == pFont, E_FAIL);
    m_FontContainer.insert(std::pair<_wstring, SHPTR<UFont>>(_strFontName, pFont));
    return S_OK;
}

SHPTR<UFont> UFontManager::FontCreateAdd(const _wstring& _strFontName, const _wstring& _strPath)
{
    if (m_FontContainer.size() >= FONT_MAX_SIZE)
        return nullptr;

    SHPTR<UFont> pFont = nullptr;

    if (m_RemainFontDescriptorIndex.empty()) {
        pFont = CreateNative<UFont>(m_spRenderObject, m_pDescriptor, m_iFontCnt, _strPath);
    }
    else {
        std::list<_uint>::iterator it = m_RemainFontDescriptorIndex.begin();
        pFont = CreateNative<UFont>(m_spRenderObject, m_pDescriptor, *it, _strPath);
        m_RemainFontDescriptorIndex.erase(it);
    }

    RETURN_CHECK(nullptr == pFont, nullptr);
    m_FontContainer.insert(std::pair<_wstring, SHPTR<UFont>>(_strFontName, pFont));
    return pFont->Clone();
}

SHPTR<UFont> UFontManager::AddFont(const _wstring& _strFontName)
{
    FONTCONTAINER::iterator Font = FindFont(_strFontName);
    RETURN_CHECK(m_FontContainer.end() == Font, nullptr);

    SHPTR<UFont> CloneFont = (*Font).second->Clone();
    m_FontDrawList.push_back(CloneFont);
    return CloneFont;
}

void UFontManager::ReleaseOriginFont(const _wstring& _strFontName)
{
    FONTCONTAINER::iterator Font = FindFont(_strFontName);
    RETURN_CHECK(m_FontContainer.end() == Font, ;);

    for (FONTDRAWLIST::iterator it = m_FontDrawList.begin(); it != m_FontDrawList.end();) {
        if ((*it)->GetFontIndex() == (*Font).second->GetFontIndex()) {
            it = m_FontDrawList.erase(it);
        }
        else {
            ++it;
        }
    }
    m_RemainFontDescriptorIndex.push_back(Font->second->GetFontIndex());
    m_FontContainer.erase(Font);
}

void UFontManager::RemoveFont(CSHPTRREF<UFont> _pFont)
{
    FONTDRAWLIST::iterator it = std::find(m_FontDrawList.begin(), m_FontDrawList.end(), _pFont);
    RETURN_CHECK(it == m_FontDrawList.end(), ;);
    m_FontDrawList.erase(it);
}

void UFontManager::Render()
{
    Dx12DescriptorHeap* DescHeap[] = { m_pDescriptor->Heap()};
    //CBV , Sampler 2개 인자만 넣을 수 있다.
    m_spRenderObject->GetCommand()->GetGpuCmdList()->SetDescriptorHeaps(1, DescHeap);
    m_spSpriteBatch->SetViewport(m_tViewPort);
    m_spSpriteBatch->Begin(m_spRenderObject->GetCommand()->GetGpuCmdList().Get());
    for (FONTDRAWLIST::iterator it = m_FontDrawList.begin(); it != m_FontDrawList.end(); ++it) {
        (*it)->Render(m_spSpriteBatch.get(), m_tViewPort);
    }
    m_spSpriteBatch->End();
    // GraphicMemory가 필요함, SpriteBatch를 사용하기 위해서는, 왜 그런지 이해 불가
    m_pGraphicMemory->Commit(m_spRenderObject->GetCommand()->GetCmdQue().Get());
}

UFontManager::FONTCONTAINER::iterator UFontManager::FindFont(const _wstring& _strFontName)
{
    FONTCONTAINER::iterator it = m_FontContainer.find(_strFontName);
    return it;
}
