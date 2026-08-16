#include "ClientDefine.h"
#include "CAimingPoint.h"
#include "URectTransform.h"
#include "UTexGroup.h"
#include "UMethod.h"
#include "UModel.h"
#include "UTransform.h"
#include "UGameInstance.h"
#include "UVIBufferRect.h"
#include "UShader.h"

CAimingPoint::CAimingPoint(CSHPTRREF<UDevice> _spDevice, const _wstring& _wstrLayer, const CLONETYPE& _eCloneType) : 
	UUserInterface(_spDevice, _wstrLayer, _eCloneType)
{
}

CAimingPoint::CAimingPoint(const CAimingPoint& _rhs) :
	UUserInterface(_rhs)
{
}

void CAimingPoint::Free()
{
}

HRESULT CAimingPoint::NativeConstruct()
{
	return __super::NativeConstruct();
}

HRESULT CAimingPoint::NativeConstructClone(const VOIDDATAS& _vecDatas)
{
	RETURN_CHECK_FAILED(__super::NativeConstructClone(_vecDatas), E_FAIL);

	SHPTR<UGameInstance> spGameInstance = GET_INSTANCE(UGameInstance);
	m_spVIBufferRect = std::static_pointer_cast<UVIBufferRect>(spGameInstance->CloneResource(PROTO_RES_VIBUFFERRECT));
	m_spUITextureGroup = std::static_pointer_cast<UTexGroup>(spGameInstance->CloneResource(PROTO_RES_UITEXUREGROUP));
	
	AddShader(PROTO_RES_2DUISHADER);
	GetRectTransform()->ChangeSize(WINDOW_WIDTH, WINDOW_HEIGHT);
	return S_OK;
}

void CAimingPoint::TickActive(const _double& _dTimeDelta)
{
}

void CAimingPoint::LateTickActive(const _double& _dTimeDelta)
{
	if (nullptr != m_spVIBufferRect)
		AddRenderGroup(RENDERID::RI_2DUI);
}

HRESULT CAimingPoint::RenderActive(CSHPTRREF<UCommand> _spCommand, CSHPTRREF<UTableDescriptor> _spTableDescriptor)
{
	__super::RenderActive(_spCommand, _spTableDescriptor);

	GetTransform()->BindTransformData(GetShader(), DEFFERED_CAMERA_ID);
	GetShader()->BindSRVBuffer(SRV_REGISTER::T0, m_spUITextureGroup->GetTexture(0));
	m_spVIBufferRect->Render(GetShader(), _spCommand);
	return S_OK;
}

void CAimingPoint::Collision(CSHPTRREF<UPawn> _pEnemy)
{
}

HRESULT CAimingPoint::ReadyRectTransform(const VOIDDATAS& _vecDatas)
{
	return __super::ReadyRectTransform(_vecDatas);
}
