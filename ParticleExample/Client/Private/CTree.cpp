#include "ClientDefine.h"
#include "CTree.h"
#include "UTexGroup.h"
#include "UMethod.h"
#include "UModel.h"
#include "UTransform.h"
#include "UGameInstance.h"

CTree::CTree(CSHPTRREF<UDevice> _spDevice, const _wstring& _wstrLayer, const CLONETYPE& _eCloneType) :
	UPawn(_spDevice, _wstrLayer, _eCloneType, BACKINGTYPE::DYNAMIC),
	m_spModel{nullptr}
{
}

CTree::CTree(const CTree& _rhs) : 
	UPawn(_rhs),
	m_spModel{ nullptr }
{
}

void CTree::Free()
{
}

HRESULT CTree::NativeConstruct()
{
	return __super::NativeConstruct();
}

HRESULT CTree::NativeConstructClone(const VOIDDATAS& _vecDatas)
{
	if (FAILED(__super::NativeConstructClone(_vecDatas)))
		return E_FAIL;

	SHPTR<UGameInstance> spGameInstance = GET_INSTANCE(UGameInstance);

	_wstring PROTO_MODEL{ L"" };
	if (spGameInstance->GetRandomNumber(10000) & 01)
	{
		PROTO_MODEL = PROTO_RES_TREEMODEL_01;
	}
	else
	{
		PROTO_MODEL = PROTO_RES_TREEMODEL_02;
	}
	m_spModel = static_pointer_cast<UModel>(spGameInstance->CloneResource(PROTO_MODEL));
	AddShader(PROTO_RES_MODELSHADER, RES_SHADER);

//	GetTransform()->SetPos(_float3{ 10, 1, 10 });
//	GetTransform()->SetScale(_float3{ 0.25f, 0.25f, 0.25f });
	return S_OK;
}

void CTree::TickActive(const _double& _dTimeDelta)
{
}

void CTree::LateTickActive(const _double& _dTimeDelta)
{
	SHPTR<UGameInstance> spGameInstance = GET_INSTANCE(UGameInstance);
	if (false == spGameInstance->IsFrustomContains(GetTransform()->GetPos(), GetTransform()->GetScale().x * GetTransform()->GetScale().z
		* 2))
		return;

	if (nullptr != m_spModel)
	{
		AddRenderGroup(RI_NONALPHA_MIDDLE);
	}
}

HRESULT CTree::RenderActive(CSHPTRREF<UCommand> _spCommand, CSHPTRREF<UTableDescriptor> _spTableDescriptor)
{
	if (nullptr != m_spModel)
	{
		__super::RenderActive(_spCommand, _spTableDescriptor);

		for (_uint i = 0; i < m_spModel->GetMeshContainerCnt(); ++i)
		{
			// Bind Transform 
			GetTransform()->BindTransformData(GetShader());

			m_spModel->BindTexture(i, SRV_REGISTER::T0, TEXTYPE::TextureType_DIFFUSE, GetShader());
			m_spModel->BindTexture(i, SRV_REGISTER::T1, TEXTYPE::TextureType_NORMALS, GetShader());
			// Render
			m_spModel->Render(i, GetShader(), _spCommand);
		}
	}
	return S_OK;
}

void CTree::Collision(CSHPTRREF<UPawn> _pEnemy)
{
}
