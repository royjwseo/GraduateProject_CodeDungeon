#include "EngineDefines.h"
#include "UMeshContainer.h"
#include "UBoneNode.h"
#include "UModel.h"
#include "UMethod.h"

UMeshContainer::UMeshContainer(CSHPTRREF<UDevice> _spDevice)
	: UVIBuffer(_spDevice, VISPACE_TYPE::SPACE_3D, VIINSTANCE_TYPE::SINGLE),
	m_iMaterialIndex{ 0 },
	m_iNumBones{ 0 },
	m_iNumBuffers{0},
	m_vecBoneNodes{  },
	m_wstrName{ L""}
{
}

UMeshContainer::UMeshContainer(const UMeshContainer& _rhs) :
	 UVIBuffer(_rhs), 
	m_iMaterialIndex{ _rhs.m_iMaterialIndex },
	m_iNumBones{ _rhs.m_iNumBones },
	m_iNumBuffers{ _rhs.m_iNumBuffers },
	m_vecBoneNodes{ _rhs.m_vecBoneNodes },
	m_wstrName{ _rhs.m_wstrName }
{
}

void UMeshContainer::Free()
{
	m_vecBoneNodes.clear();
}

HRESULT UMeshContainer::NativeConstruct()
{
	return __super::NativeConstruct();
}

HRESULT UMeshContainer::NativeConstruct(void* _pData, CSHPTRREF<UModel> _spModel)
{
	RETURN_CHECK_FAILED(NativeConstruct(), E_FAIL);

	if (UModel::TYPE::ANIM == _spModel->GetType())
	{
		RETURN_CHECK_FAILED(ReadyAnimVertices(_pData, _spModel), E_FAIL);
	}
	else
	{
		RETURN_CHECK_FAILED(ReadyVertices(_pData, _spModel), E_FAIL);
	}
	RETURN_CHECK_FAILED(ReadyIndicies(_pData), E_FAIL);
	return S_OK;
}

HRESULT UMeshContainer::NativeConstructClone(const VOIDDATAS& _vecDatas)
{
	RETURN_CHECK(0 >= _vecDatas.size(), E_FAIL);
	UModel* pModel = UMethod::ConvertTemplate_Index<UModel*>(_vecDatas, 0);
	RETURN_CHECK(nullptr == pModel, E_FAIL);
	VECTOR<SHPTR<UBoneNode>> BoneModels{};

	for (auto& BoneNode : m_vecBoneNodes) {
		SHPTR<UBoneNode> pBoneNode = pModel->GetBoneNode(BoneNode->GetName());
		BoneModels.push_back(pBoneNode);
	}

	m_vecBoneNodes.clear();
	m_vecBoneNodes = BoneModels;
	return S_OK;
}

HRESULT UMeshContainer::SetUpBoneMatrix(ARRAY<_float4x4, MAX_BONE_SIZE>& _arrBones)
{
	for (_uint i = 0; i < m_vecBoneNodes.size(); ++i)
	{
		_arrBones[i] = m_vecBoneNodes[i]->GetOffsetMatrix() *
			m_vecBoneNodes[i]->GetCombineMatrix();

		_arrBones[i] = _arrBones[i].Transpose();
	}
	return S_OK;
}

HRESULT UMeshContainer::ReadyVertices(void* _pData, CSHPTRREF<UModel> _spModel)
{
	MESHDESC* tMeshDesc = static_cast<MESHDESC*>(_pData);
	ReceiveBoneNode(_spModel, tMeshDesc->vecBoneNodeNameList, tMeshDesc->iBoneNodeCnt);
	return MakeVertices(tMeshDesc->pVtxModel, (void*)tMeshDesc->pPosition, sizeof(VTXMODEL),
		tMeshDesc->iNumVertices, tMeshDesc->vMinVertex, tMeshDesc->vMaxVertex, 
		tMeshDesc->wstrName, tMeshDesc->iMatrialIndex);
}

HRESULT UMeshContainer::ReadyAnimVertices(void* _pData, CSHPTRREF<UModel> _spModel)
{
	ANIMMESHDESC* tMeshDesc = static_cast<ANIMMESHDESC*>(_pData);
	ReceiveBoneNode(_spModel, tMeshDesc->vecBoneNodeNameList, tMeshDesc->iBoneNodeCnt);
	return MakeVertices(tMeshDesc->pVtxModel, (void*)tMeshDesc->pPosition, sizeof(VTXANIMMODEL),
		tMeshDesc->iNumVertices, tMeshDesc->vMinVertex, tMeshDesc->vMaxVertex,
		tMeshDesc->wstrName, tMeshDesc->iMatrialIndex);
}

HRESULT UMeshContainer::ReadyIndicies(void* _pData)
{
	MESHDESC* tMeshDesc = static_cast<MESHDESC*>(_pData);
	RETURN_CHECK_FAILED(CreateIndexBuffer(tMeshDesc->iNumIndices, sizeof(INDICIES32),
		tMeshDesc->pIndices, DXGI_FORMAT_R32_UINT), E_FAIL);
	return S_OK;
}
/* Make To Vertex */
HRESULT UMeshContainer::MakeVertices(void* _pVertices, void* _pPosition, const _uint _iVertexSize, const _uint _iVertexCnt
	, const _float3& _vMinPos, const _float3& _vMaxPos ,const _wstring& _wstrName, const _uint& _iMaterialIndex)
{
	VECTOR<_float3> pVerticesPos;
	pVerticesPos.resize(_iVertexCnt);
	::memcpy(&pVerticesPos[0], _pPosition, sizeof(_float3) * _iVertexCnt);


	RETURN_CHECK_FAILED(CreateVtxBuffer(_iVertexCnt, _iVertexSize, _pVertices,
		D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST, pVerticesPos, false), E_FAIL);

	m_wstrName = _wstrName;
	m_iMaterialIndex = _iMaterialIndex;

	SetMinPosition(_vMinPos);
	SetMaxPosition(_vMaxPos);

	Safe_Delete_Array(_pVertices);
	Safe_Delete_Array(_pPosition);
	return S_OK;
}

void UMeshContainer::ReceiveBoneNode(CSHPTRREF<UModel> _spModel, const VECTOR<_wstring>& _convecBoneNameList,
	const _uint& _iBoneNodeCnt)
{
	for (_uint i = 0; i < _iBoneNodeCnt; ++i)
	{
		SHPTR<UBoneNode> spBoneNode = _spModel->GetBoneNode(_convecBoneNameList[i]);
		m_vecBoneNodes.push_back(spBoneNode);
	}
	m_iNumBones = _iBoneNodeCnt;
}
