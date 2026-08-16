#include "EngineDefines.h"
#include "UFont.h"
#include "URenderObject.h"
#include "UDevice.h"
#include<DirectXTK/ResourceUploadBatch.h>
#include "UCommand.h"

UFont::UFont() : 
	m_SpriteFont{ },
	m_spRenderObject{nullptr }
{
}

UFont::UFont(const UFont& _rhs) : 
	m_SpriteFont{ _rhs.m_SpriteFont },
	m_spRenderObject{ _rhs.m_spRenderObject }
{
}

SHPTR<UFont> UFont::Clone()
{
	SHPTR<UFont> pFont = std::make_shared<UFont>(*this);
	pFont->m_bClone = true;
	return pFont;
}

HRESULT UFont::NativeConstruct(CSHPTRREF<URenderObject> _spRenderObject, CSHPTRREF<DirectX::DescriptorHeap> _descriptor, _uint& _iFontIndex, const _wstring& _strFontPath)
{
	m_spRenderObject = _spRenderObject;

	ComPtr<Dx12Device> DEVICE = m_spRenderObject->GetDevice()->GetDV();

	DirectX::ResourceUploadBatch Batch{ DEVICE.Get()};
	Batch.Begin();

	m_SpriteFont = std::make_shared<DirectX::SpriteFont>(DEVICE.Get(), Batch,
		_strFontPath.c_str(), _descriptor->GetCpuHandle(_iFontIndex), _descriptor->GetGpuHandle(_iFontIndex));

	auto upLoadResourceFinished = Batch.End(m_spRenderObject->GetCommand()->GetCmdQue().Get());
	upLoadResourceFinished.wait();

	m_iFontIndex = _iFontIndex;
	return S_OK;
}

void UFont::Render(DirectX::SpriteBatch* _pBatch, const D3D12_VIEWPORT& _tViewPort)
{
	m_SpriteFont->DrawString(_pBatch, m_tFontDesc.strText.c_str(), m_tFontDesc.vPos, m_tFontDesc.vColor,
		m_tFontDesc.fRotation, m_tFontDesc.vOrigin, m_tFontDesc.vScale, m_tFontDesc.eSpriteEffects, m_tFontDesc.fDepths);
}

void UFont::Free()
{
}
