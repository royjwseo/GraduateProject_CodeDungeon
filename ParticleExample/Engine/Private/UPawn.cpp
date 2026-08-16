#include "EngineDefines.h"
#include "UPawn.h"
#include "UShader.h"
#include "URenderer.h"
#include "UGameInstance.h"

UPawn::UPawn(CSHPTRREF<UDevice> _spDevice, const _wstring& _wstrLayer, const CLONETYPE& _eCloneType, const BACKINGTYPE _eBackingType) :
	UActor(_spDevice, _wstrLayer, _eCloneType, _eBackingType, USECOLLISIONTYPE::ACTIVE),
	m_spRenderer{nullptr}, 
	m_spShader{nullptr}
#ifdef _USE_DEBUGGING
	, m_isDebugRenderingType{false}
#endif 
{
}

UPawn::UPawn(const UPawn& _rhs) : 
	UActor(_rhs), 
	m_spRenderer{ nullptr }, 
	m_spShader{ nullptr }
#ifdef _USE_DEBUGGING
	, m_isDebugRenderingType{ false }
#endif 
{
}

CSHPTRREF<UShader> UPawn::GetShader() const {
	return m_spShader;
}


void UPawn::Free()
{
}

HRESULT UPawn::NativeConstruct()
{
	return __super::NativeConstruct();
}

HRESULT UPawn::NativeConstructClone(const VOIDDATAS& _vecDatas)
{
	RETURN_CHECK_FAILED(__super::NativeConstructClone(_vecDatas), E_FAIL);
	// Renderer
	m_spRenderer = AddComponent<URenderer>(PROTO_COMP_RENDERER, COM_RENDERER);
	return S_OK;
}

bool UPawn::IsHit(CSHPTRREF<UPawn> _pEnemy)
{
	return false;
}

void UPawn::TickActive(const _double& _dTimeDelta)
{
}

void UPawn::LateTickActive(const _double& _dTimeDelta)
{
}

HRESULT UPawn::RenderActive(CSHPTRREF<UCommand> _spCommand, CSHPTRREF<UTableDescriptor> _spTableDescriptor)
{
	// Settings 
	GetShader()->SetTableDescriptor(_spTableDescriptor);
	return S_OK;
}

void UPawn::Collision(CSHPTRREF<UPawn> _pEnemy)
{
}

void UPawn::AddShader(const _wstring& _wstrProtoTag, const _wstring& _wstrTag, const VOIDDATAS& _vecDatas)
{
	m_spShader = AddResource<UShader>(_wstrProtoTag, _wstrTag, _vecDatas);
}

void UPawn::AddRenderGroup(const RENDERID _iRenderID)
{
	m_vecRenderingDatas.push_back(_iRenderID);
	m_spRenderer->AddRenderGroup(_iRenderID, GetShader(), ThisShared<UPawn>());
}
#ifdef _USE_DEBUGGING
void UPawn::AddDebugRenderGroup(const DEBUGRENDERID _iRenderID)
{
	m_vecRenderingDatas.push_back(_iRenderID);
	m_spRenderer->AddDebugRenderGroup(_iRenderID, GetShader(), ThisShared<UPawn>());
	m_isDebugRenderingType = true;
}
#endif
#ifdef _USE_IMGUI

void UPawn::ShowObjectInfo()
{
	__super::ShowObjectInfo();


}

#endif 
