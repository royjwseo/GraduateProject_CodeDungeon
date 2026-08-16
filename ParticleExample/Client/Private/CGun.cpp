#include "ClientDefine.h"
#include "CGun.h"
#include "UTexGroup.h"
#include "UMethod.h"
#include "UModel.h"
#include "UTransform.h"
#include "UGameInstance.h"
#include "UShaderConstantBuffer.h"
#include "UShader.h"

CGun::CGun(CSHPTRREF<UDevice> _spDevice, const _wstring& _wstrLayer, const CLONETYPE& _eCloneType) :
	UPawn(_spDevice, _wstrLayer, _eCloneType, BACKINGTYPE::DYNAMIC),
	 m_spOwner{nullptr}, m_spSocketMatrixBuffer{nullptr}
{
}

CGun::CGun(const CGun& _rhs) :
	UPawn(_rhs),
	m_spOwner{nullptr}, m_spSocketMatrixBuffer{ nullptr }
{
}

void CGun::SetOwner(CSHPTRREF<UPawn> _spPawn)
{
	this->m_spOwner = _spPawn;
}

void CGun::Free()
{
}

HRESULT CGun::NativeConstruct()
{
	return __super::NativeConstruct();
}

HRESULT CGun::NativeConstructClone(const VOIDDATAS& _vecDatas)
{
	if (FAILED(__super::NativeConstructClone(_vecDatas)))
		return E_FAIL;

	return S_OK;
}

void CGun::ShootBullet(const _float3& _vPos, const _float3& _vDir)
{
}

void CGun::TickActive(const _double& _dTimeDelta)
{
}

void CGun::LateTickActive(const _double& _dTimeDelta)
{
}

HRESULT CGun::RenderActive(CSHPTRREF<UCommand> _spCommand, CSHPTRREF<UTableDescriptor> _spTableDescriptor)
{
	__super::RenderActive(_spCommand, _spTableDescriptor);

	if (nullptr != m_spOwner)
	{
		m_SocketMatrixParam.SocketMatrix = DirectX::XMMatrixTranslationFromVector(m_spOwner->GetTransform()->GetPos());
	}
	return S_OK;
}

void CGun::Collision(CSHPTRREF<UPawn> _pEnemy)
{
}

HRESULT CGun::CreateSocketMatirxBuffer(const _uint _iSize)
{
	m_spSocketMatrixBuffer = CreateNative<UShaderConstantBuffer>(GetDevice(), CBV_REGISTER::SOCKETMATRIX, SOCKETMATRIXPARAM_SIZE, _iSize);
	RETURN_CHECK(nullptr == m_spSocketMatrixBuffer, E_FAIL);
	return S_OK;
}

void CGun::BindSocketMatrixBuffer()
{
	GetShader()->BindCBVBuffer(m_spSocketMatrixBuffer, &m_SocketMatrixParam, SOCKETMATRIXPARAM_SIZE);
}
