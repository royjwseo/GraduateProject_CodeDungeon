#include "EngineDefines.h"
#include "UUserInterface.h"
#include "URectTransform.h"
#include "UGameInstance.h"
#include "UMethod.h"

UUserInterface::UUserInterface(CSHPTRREF<UDevice> _spDevice,  const _wstring& _wstrLayer, const CLONETYPE& _eCloneType)
	: UPawn(_spDevice, _wstrLayer, _eCloneType),
	m_spRectTransform{nullptr},  m_stUIDesc{}
{
}

UUserInterface::UUserInterface(const UUserInterface& _rhs) : 
	UPawn(_rhs), m_spRectTransform{ nullptr },  m_stUIDesc{}
{
}

void UUserInterface::Free()
{
}

HRESULT UUserInterface::NativeConstruct()
{
	return __super::NativeConstruct();
}

HRESULT UUserInterface::NativeConstructClone(const VOIDDATAS& _vecDatas)
{
	RETURN_CHECK_FAILED(__super::NativeConstructClone(_vecDatas), E_FAIL);
	RETURN_CHECK_FAILED(ReadyRectTransform(_vecDatas), E_FAIL);
	return S_OK;
}

void UUserInterface::UpdateRatio(const _float _fRatio)
{
	RETURN_CHECK(nullptr == m_spRectTransform, ;);
	m_spRectTransform->UpdateRatio(_fRatio);
}

void UUserInterface::UpdatePos(const _float2& _vPos)
{
	RETURN_CHECK(nullptr == m_spRectTransform, ;);
	m_spRectTransform->UpdatePos(_vPos);
}

void UUserInterface::UpdateSize(const _float2& _vSize)
{
	RETURN_CHECK(nullptr == m_spRectTransform, ;);
	m_spRectTransform->UpdateSize(_vSize);
}

_bool UUserInterface::IsMouseOnRect()
{
	RETURN_CHECK(nullptr == m_spRectTransform, false);

	SHPTR<UGameInstance> pGameInstance = GET_INSTANCE(UGameInstance);
	_float2 vMousePos = pGameInstance->GetMousePosition();
	return m_spRectTransform->IsMouseOnRect(vMousePos);
}

void UUserInterface::TickActive(const _double& _dTimeDelta)
{

}

void UUserInterface::LateTickActive(const _double& _dTimeDelta)
{
}

HRESULT UUserInterface::RenderActive(CSHPTRREF<UCommand> _spCommand, CSHPTRREF<UTableDescriptor> _spTableDescriptor)
{
	return __super::RenderActive(_spCommand, _spTableDescriptor);
}

HRESULT UUserInterface::ReadyRectTransform(const VOIDDATAS& _vecDatas)
{
	SHPTR<UGameInstance> pGameInstance = GET_INSTANCE(UGameInstance);
	{
		CSHPTRREF<GRAPHICDESC> DESC = pGameInstance->GetGraphicDesc();
		UIDESC tDesc = UMethod::ConvertTemplate_Index<UIDESC>(_vecDatas, 0);
		URectTransform::RECTDESC tRtDesc;
		tRtDesc.fRatio = tDesc.fZBufferOrder;
		tRtDesc.vPos = tDesc.v2Pos;
		tRtDesc.vSize = tDesc.v2Size;
		tRtDesc.ProjMatrix = DirectX::XMMatrixOrthographicLH((_float)DESC->iWinCX, (_float)DESC->iWinCY,
			0.f, 1.f);
		tRtDesc.pTransform = GetTransform();
		ReadyRectTransform(&tRtDesc);
	}
	return S_OK;
}

HRESULT UUserInterface::ReadyRectTransform(void* _pDesc)
{
	VOIDDATAS vecDatas;
	vecDatas.push_back(_pDesc);

	m_spRectTransform = AddComponent<URectTransform>(PROTO_COMP_RECTTRANSFORM,
		COM_RECTTRANSFORM, vecDatas);
	RETURN_CHECK(nullptr == m_spRectTransform, E_FAIL);
	return S_OK;
}
