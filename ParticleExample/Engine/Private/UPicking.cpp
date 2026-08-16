#include "EngineDefines.h"
#include "UPicking.h"
#include "UTransform.h"
#include "UVIBuffer.h"
#include "UActor.h"
#include "UGameInstance.h"

UPicking::UPicking() :
	m_vWindowSize{ 0.f, 0.f },
	m_stClientRect{ 0, 0, 0, 0 },
	m_vRayPos{ 0.f, 0.f, 0.f },
	m_vRayDir{ 0.f, 0.f, 0.f },
	m_lsPickingList{},
	m_stPickingDesc{},
	m_isMouseInScreen{ false }
{
}

void UPicking::Free()
{
}

HRESULT UPicking::ReadyPickingDesc(CSHPTRREF<GRAPHICDESC> _spGraphicDesc)
{
	RETURN_CHECK(nullptr == _spGraphicDesc, E_FAIL);

	GetClientRect(_spGraphicDesc->hWnd, &m_stClientRect);
	m_vWindowSize.x = static_cast<_float>(_spGraphicDesc->iWinCX);
	m_vWindowSize.y = static_cast<_float>(_spGraphicDesc->iWinCY);
	return S_OK;
}

void UPicking::TickRayInWorldSpace(UGameInstance* _pGameInstance)
{
	RETURN_CHECK(nullptr == _pGameInstance, ;);
	m_WaitCheckActorList.clear();

	_float2 vMousePos{ _pGameInstance->GetMousePosition() };
	if ((m_stClientRect.left > vMousePos.x || vMousePos.x > m_stClientRect.right) &&
		(m_stClientRect.top > vMousePos.y || vMousePos.y > m_stClientRect.bottom))
	{
		m_isMouseInScreen = true;
		return;
	}
	m_isMouseInScreen = false;
	const _float4x4 mViewMatrixInv = _pGameInstance->GetMainCamViewMatrix().Invert();
	const _float4x4& mProjMatrix = _pGameInstance->GetMainCamProjMatrix();
	// Position 
	_float2		vPosition{};
	vPosition.x = (2.f * vMousePos.x / m_vWindowSize.x - 1.f) / mProjMatrix._11;
	vPosition.y = (-2.f * vMousePos.y / m_vWindowSize.y + 1.f) / mProjMatrix._22;

	m_vRayPos = { 0.f, 0.f, 0.f };
	m_vRayDir = { vPosition.x, vPosition.y, 1.f };

	m_vRayPos = _float3::TransformCoord(m_vRayPos, mViewMatrixInv);
	m_vRayDir = _float3::TransformNormal(m_vRayDir, mViewMatrixInv);
}

void UPicking::AddPickingObject(CSHPTRREF<UActor> _spActor, CSHPTRREF<UVIBuffer> _spVIBuffer)
{
	WAITCHECKACTOR stActor;
	stActor.spActor = _spActor;
	stActor.spVIBuffer = _spVIBuffer;
	m_WaitCheckActorList.insert(stActor);
}

SHPTR<UActor> UPicking::GetPickingActor()
{
	m_stPickingDesc = GetPickDesc();
	return m_stPickingDesc.spActor;
}

const PICKINGDESC& UPicking::GetPickDesc()
{
	for (auto& iter : m_WaitCheckActorList)
	{
		_float3 v3Pos = _float3(0.f, 0.f, 0.f);
		_float fDist = 0.f;
		if (true == PickingMesh(iter.spActor, iter.spVIBuffer, &fDist, &v3Pos))
			AddPickingObject(PICKINGDESC(iter.spActor, v3Pos, fDist));
	}

	SHPTR<UGameInstance> pGameInstance = GET_INSTANCE(UGameInstance);
	_float3 vCamPos = pGameInstance->GetMainCamPosition();
	{	
		m_lsPickingList.sort([&](const PICKINGDESC& _t1, const PICKINGDESC& _t2)
			{
				_float fLen1 = _float3::LengthSquared(_t1.vPickPos - vCamPos);
				_float fLen2 = _float3::LengthSquared(_t2.vPickPos - vCamPos);
				return fLen1 < fLen2;
			});
		if (m_lsPickingList.size() > 0)
		{
			m_stPickingDesc = m_lsPickingList.front();
			m_lsPickingList.clear();
			return m_stPickingDesc;
		}
		m_stPickingDesc = PICKINGDESC{};
	}
	return m_stPickingDesc;
}

_bool UPicking::PickingMesh(CSHPTRREF<UActor> _spActor, CSHPTRREF<UVIBuffer> _spVIBuffer,
	_float* _pDist, _float3* _pOut)
{
	_float3 vLocalRayDir, vLocalRayPos;
	_float4x4 WorldInv = _spActor->GetTransform()->GetWorldMatrixInv();

	vLocalRayPos = _float3::TransformCoord(m_vRayPos, WorldInv);
	vLocalRayDir = _float3::TransformNormal(m_vRayDir, WorldInv);
	vLocalRayDir.Normalize();

	_uint iNumFaces = _spVIBuffer->GetIndexCnt();
	const VECTOR<_float3>& pVerticesPos = *_spVIBuffer->GetVertexPos().get();

	const void* pIndices = _spVIBuffer->GetIndices();
	DXGI_FORMAT eFormat = _spVIBuffer->GetIndexFormat();

	_uint iSize = 0;
	if (DXGI_FORMAT_R16_UINT == eFormat)
	{
		iSize = sizeof(INDICIES16);
		for (_uint i = 0; i < iNumFaces; ++i)
		{
			_ushort iIndices[3];
		//	memcpy(&iIndices, (_byte*)pIndices + iSize * i, iSize);

			_float3 v1 = pVerticesPos[iIndices[0]];
			_float3 v2 = pVerticesPos[iIndices[1]];
			_float3 v3 = pVerticesPos[iIndices[2]];

			if (true == IsPickingCheck(vLocalRayPos, vLocalRayDir, v1, v2, v3, _spActor->GetTransform()->GetWorldMatrix(),
				_pDist, _pOut))
			{
				return true;
			}
		}
	}
	else
	{
		iSize = sizeof(INDICIES32);
		for (_uint i = 0; i < iNumFaces; ++i)
		{
			_uint iIndices[3];
	//		memcpy(&iIndices, (_uint*)pIndices + iSize * i, iSize);

			_float3 v1 = pVerticesPos[iIndices[0]];
			_float3 v2 = pVerticesPos[iIndices[1]];
			_float3 v3 = pVerticesPos[iIndices[2]];

			_float fDist = 0.f;
			_float3 v3Pos = _float3(0.f, 0.f, 0.f);
			if (true == IsPickingCheck(vLocalRayPos, vLocalRayDir, v1, v2, v3, _spActor->GetTransform()->GetWorldMatrix(),
				_pDist, _pOut))
			{
				return true;
			}
		}
	}
	return false;
}

_bool UPicking::IsPickingCheck(const _float3& _vLocalRay, const _float3& _vDirRay, const _float3& _vPos1,
	const _float3& _vPos2, const _float3& _vPos3, const _float4x4& _mWorldMatrix, _float* _pDist, _float3* _pOut)
{
	_float fDist;
	if (DirectX::TriangleTests::Intersects(_vLocalRay, _vDirRay, _vPos1, _vPos2, _vPos3, fDist))
	{
		_float3 v3Pos = _vLocalRay + _vDirRay * fDist;
		v3Pos = XMVector3TransformCoord(v3Pos, _mWorldMatrix);

		if (nullptr != _pDist)
			*_pDist = fDist;

		if (nullptr != _pOut)
			*_pOut = v3Pos;
		return true;
	}
	return false;
}

void UPicking::AddPickingObject(const PICKINGDESC& _stDesc)
{
	m_lsPickingList.push_back(_stDesc);
}
