#include "EngineDefines.h"
#include <fstream>
#include "UModel.h"
#include "UMethod.h"
#include "UTexGroup.h"
#include "UBoneNode.h"
#include "UGameInstance.h"
#include "UShader.h"
#include "UMeshContainer.h"
#include "UTexture.h"
#include <algorithm>

UModel::UModel(CSHPTRREF<UDevice> _spDevice, const TYPE& _eType) :
    UResource(_spDevice),
    m_vecMeshContainer{},
    m_iMeshContainerCnt{ 0 },
    m_vecBoneNodes{},
    m_iBoneNodeCnt{ 0 },
    m_vecMaterials{},
    m_iMaterialCnt{ 0 },
    m_spFileGroup{ nullptr },
    m_spFileData{ nullptr }, 
	m_eType{_eType}
{
}

UModel::UModel(const UModel& _rhs) : 
    UResource(_rhs),
    m_vecMeshContainer{ _rhs.m_vecMeshContainer},
    m_iMeshContainerCnt{ _rhs.m_iMeshContainerCnt},
    m_vecBoneNodes{ _rhs.m_vecBoneNodes},
    m_iBoneNodeCnt{ _rhs.m_iBoneNodeCnt },
    m_vecMaterials{ _rhs.m_vecMaterials },
    m_iMaterialCnt{ _rhs.m_iMaterialCnt },
    m_spFileGroup{ _rhs.m_spFileGroup },
    m_spFileData{ _rhs.m_spFileData },
	m_eType{_rhs.m_eType}
{
}


SHPTR<UBoneNode> UModel::GetBoneNode(const _wstring& _strBoneNode)
{
	const BONENODES::iterator& it = std::find_if(m_vecBoneNodes.begin(), m_vecBoneNodes.end(), [&](const SHPTR<UBoneNode>& p) {
		if (p->GetName() == _strBoneNode)
			return true;

		return false;
		});
	RETURN_CHECK(m_vecBoneNodes.end() == it, nullptr);
	return *it;
}

void UModel::Free()
{
	m_vecBoneNodes.clear();
	m_vecMeshContainer.clear();
	m_vecMaterials.clear();
}

HRESULT UModel::NativeConstruct()
{
    return __super::NativeConstruct();
}

HRESULT UModel::NativeConstruct(const _wstring& _wstrPath)
{
	RETURN_CHECK_FAILED(NativeConstruct(), E_FAIL);
	LoadToData(_wstrPath);
	return S_OK;
}

HRESULT UModel::NativeConstruct(CSHPTRREF<FILEGROUP> _spFileGroup, CSHPTRREF<FILEDATA> _spFileData)
{
	RETURN_CHECK_FAILED(NativeConstruct(), E_FAIL);
	RETURN_CHECK(nullptr == _spFileGroup || nullptr == _spFileData, E_FAIL);
	m_spFileGroup = _spFileGroup; m_spFileData = _spFileData;
	LoadToData(m_spFileData->wstrfilePath);
	return S_OK;
}

HRESULT UModel::NativeConstruct(const PATHS& _vecPaths, const _wstring& _wstrFileName)
{
	RETURN_CHECK_FAILED(NativeConstruct(), E_FAIL);
	SHPTR<UGameInstance> spGameInstance = GET_INSTANCE(UGameInstance);
	m_spFileGroup = spGameInstance->FindFolder(_vecPaths);
	RETURN_CHECK(nullptr == m_spFileGroup, E_FAIL);
	m_spFileData = m_spFileGroup->FindData(_wstrFileName);
	RETURN_CHECK(nullptr == m_spFileData, E_FAIL);
	LoadToData(m_spFileData->wstrfilePath);
	return S_OK;
}

HRESULT UModel::NativeConstruct(const _wstring& _wstrModelFolder, const _wstring& _wstrFileName)
{
	RETURN_CHECK_FAILED(NativeConstruct(), E_FAIL);
	SHPTR<UGameInstance> spGameInstance = GET_INSTANCE(UGameInstance);
	m_spFileGroup = spGameInstance->FindFolder(_wstrModelFolder);
	RETURN_CHECK(nullptr == m_spFileGroup, E_FAIL);
	m_spFileData = m_spFileGroup->FindData(_wstrFileName);
	RETURN_CHECK(nullptr == m_spFileData, E_FAIL);
	LoadToData(m_spFileData->wstrfilePath);
	return S_OK;
}

HRESULT UModel::NativeConstructClone(const VOIDDATAS& _vecDatas)
{
	RETURN_CHECK_FAILED(__super::NativeConstructClone(_vecDatas), E_FAIL);

	MESHCONTAINERS MeshContainers{};
	BONENODES BoneNodes{};

	for (auto& iter : m_vecBoneNodes) {
		BoneNodes.push_back(iter->Clone());
	}
	m_vecBoneNodes.clear();
	m_vecBoneNodes = BoneNodes;
	// Bone Nodes
	for (auto& iter : m_vecBoneNodes) {
		iter->FindParents(ThisShared<UModel>());
	}

	VOIDDATAS tDatas;
	// Insert Data
	{
		tDatas.push_back(this);
	}

	for (auto& iter : m_vecMeshContainer) {
		MeshContainers.push_back(static_pointer_cast<UMeshContainer>(iter->Clone(tDatas)));
	}
	m_vecMeshContainer.clear();
	m_vecMeshContainer = MeshContainers;

	return S_OK;
}

HRESULT UModel::BindTexture(const _uint _iMeshIndex, const SRV_REGISTER _eRegister,
	const TEXTYPE& _eTextureType, CSHPTRREF<UShader> _spShader)
{
	RETURN_CHECK(nullptr == _spShader, E_FAIL);
	RETURN_CHECK(GetMeshContainerCnt() <= _iMeshIndex, E_FAIL);
	RETURN_CHECK(nullptr == GetMeshContainers()[_iMeshIndex], E_FAIL);
	// BindSrvBuffer
	CSHPTRREF<UMeshContainer> spMeshContainer {GetMeshContainers()[_iMeshIndex]};
	if (GetMaterials().size() > 0)
	{
		if (nullptr != GetMaterials()[spMeshContainer->GetMaterialIndex()])
		{
			SHPTR<UTexGroup> spTexture = GetMaterials()[spMeshContainer->GetMaterialIndex()]->arrMaterialTexture[_eTextureType];
			if (nullptr != spTexture)
			{
				spTexture->SetUpTextureIndex(_spShader, _eRegister, 0);
			}
		}
	}
	return S_OK;
}

HRESULT UModel::Render(const _uint _iMeshIndex, CSHPTRREF<UShader> _spShader, CSHPTRREF<UCommand> _spCommand)
{
	RETURN_CHECK(nullptr == _spShader, E_FAIL);
	RETURN_CHECK(GetMeshContainerCnt() <= _iMeshIndex, E_FAIL);
	RETURN_CHECK(nullptr == GetMeshContainers()[_iMeshIndex], E_FAIL);
	// BindSrvBuffer
	CSHPTRREF<UMeshContainer> spMeshContainer {GetMeshContainers()[_iMeshIndex]};
	spMeshContainer->Render(_spShader, _spCommand);
	return S_OK;
}

HRESULT UModel::CreateBoneNode(void* _pData)
{
	MODELDESC* tDesc = static_cast<MODELDESC*>(_pData);

	for (auto& iter : tDesc->vecBNodeDatas) {

		SHPTR<UBoneNode> pBoneNode = CreateNative<UBoneNode>(iter);

		m_vecBoneNodes.push_back( pBoneNode);
	}

	std::sort(m_vecBoneNodes.begin(), m_vecBoneNodes.end(), [&](SHPTR<UBoneNode>& p1, SHPTR<UBoneNode>& p2) {
		return p1->GetDepth() < p2->GetDepth();
		});

	for (auto& iter : m_vecBoneNodes) {
		iter->FindParents(ThisShared<UModel> ());
	}
	m_iBoneNodeCnt = (_uint)(m_vecBoneNodes.size());
	return S_OK;
}

HRESULT UModel::CreateMeshContainers(void* _pData)
{
	MODELDESC* tDesc = static_cast<MODELDESC*>(_pData);

	for (auto& iter : tDesc->vecMeshes)
	{
		SHPTR<UMeshContainer> pMeshContainer{CreateConstructorNative<UMeshContainer>(
		GetDevice(),  (void*)&iter, ThisShared<UModel>())};

		m_vecMeshContainer.push_back(pMeshContainer);
	}
	m_iMeshContainerCnt = static_cast<_uint>(m_vecMeshContainer.size());
	return S_OK;
}

HRESULT UModel::CreateMaterial(void* _pData)
{
	MODELDESC* tDesc = static_cast<MODELDESC*>(_pData);
	if (nullptr != tDesc) {
		m_vecMaterials.resize(tDesc->uomapMatrialData.size());

		for (auto& iter : tDesc->uomapMatrialData) {
			SHPTR<MODELMATRIALDESC> pModelMaterial = std::make_shared<MODELMATRIALDESC>();
			ZeroMemory(pModelMaterial.get(), sizeof(MODELMATRIALDESC));

			for (_uint i = 0; i < TEXTYPE::TextureType_UNKNOWN; ++i)
			{
				if (iter.second[i] != L"NULL")
				{
					pModelMaterial->arrMaterialTexture[i] =CreateConstructorNative<UTexGroup>(GetDevice(), iter.second[i]);
				}
			}
			m_vecMaterials[iter.first] = pModelMaterial;
		}
	}
	return S_OK;
}

void UModel::LoadToData(const _wstring& _wstrPath)
{
    std::ifstream read{_wstrPath, std::ios::binary};
    RETURN_CHECK(!read, ;);

    MODELDESC tDesc;
    {
		// MESH
		LoadMeshData(read, tDesc.vecMeshes);
		// BoneNode
		LoadBoneData(read, tDesc.vecBNodeDatas);
		// Material
		LoadMaterial(read, tDesc.uomapMatrialData);

		CreateBoneNode(&tDesc);
		CreateMeshContainers(&tDesc);
		CreateMaterial(&tDesc);
    }
}

void UModel::LoadMeshData(std::ifstream& _ifRead, VECTOR<MESHDESC>& _convecMeshes)
{
	size_t SIZE = 0;
	_ifRead.read((_char*)&SIZE, sizeof(size_t));
	_convecMeshes.resize(SIZE);
	if (0 != SIZE) {
		for (auto& iter : _convecMeshes) {
			// Name 
			UMethod::ReadString(_ifRead, iter.wstrName);
			// Vertex Model
			_ifRead.read((_char*)&iter.iNumVertices, sizeof(iter.iNumVertices));
			_ifRead.read((_char*)&iter.iNumIndices, sizeof(iter.iNumIndices));
			// Positions
			{
				iter.pVtxModel = new VTXMODEL[iter.iNumVertices];
				size_t vtxModel = sizeof(VTXMODEL) * iter.iNumVertices;
				_ifRead.read((_char*)iter.pVtxModel, vtxModel);
			}
			// Indexies
			{
				size_t index = sizeof(INDICIES32) * iter.iNumIndices;
				iter.pIndices = new INDICIES32[iter.iNumIndices];
				_ifRead.read((_char*)iter.pIndices, index);
			}
			// Positions 
			{
				size_t Positions = sizeof(_float3) * iter.iNumVertices;
				iter.pPosition = new _float3[Positions];
				_ifRead.read((_char*)iter.pPosition, Positions);
			}
			{
				_ifRead.read((_char*)&iter.vMinVertex, sizeof(_float3));
				_ifRead.read((_char*)&iter.vMaxVertex, sizeof(_float3));
			}
			// BonCnt 
			_ifRead.read((_char*)&iter.iBoneNodeCnt, sizeof(iter.iBoneNodeCnt));
			iter.vecBoneNodeNameList.resize(iter.iBoneNodeCnt);
			if (0 != iter.iBoneNodeCnt) {
				for (auto& name : iter.vecBoneNodeNameList)
				{
					UMethod::ReadString(_ifRead, name);
				}
			}
			// Material Index
			_ifRead.read((_char*)&iter.iMatrialIndex, sizeof(iter.iMatrialIndex));
		}
	}
}

void UModel::LoadBoneData(std::ifstream& _ifRead, VECTOR<BONENODEDESC>& _convecBones)
{
	size_t SIZE = 0;
	_ifRead.read((_char*)&SIZE, sizeof(size_t));
	_convecBones.resize(SIZE);
	if (0 != SIZE) {
		for (auto& iter : _convecBones) {
			// Name
			_ifRead.read((_char*)&iter.mOffsetMatrix, sizeof(_float4x4));
			_ifRead.read((_char*)&iter.mTransformMatrix, sizeof(_float4x4));
			_ifRead.read((_char*)&iter.fDepth, sizeof(_float));
			UMethod::ReadString(_ifRead, iter.wstrParentsName);
			UMethod::ReadString(_ifRead, iter.wstrName);
		}
	}
}

void UModel::LoadMaterial(std::ifstream& _ifRead, UNORMAP<_uint, VECTOR<_wstring>>& _uomapMaterials)
{
	size_t SIZE = 0;
	_ifRead.read((_char*)&SIZE, sizeof(size_t));
	if (0 != SIZE) {
		for (int i = 0; i < SIZE; ++i) {

			_uint first{ 0 };
			_ifRead.read((_char*)&first, sizeof(first));
			// Materials
			size_t STRING_SIZE = 0;
			_ifRead.read((_char*)&STRING_SIZE, sizeof(size_t));

			VECTOR<_wstring> rtVec;
			rtVec.reserve(STRING_SIZE);
			for (int j = 0; j < STRING_SIZE; ++j) {
				_wstring str;
				UMethod::ReadString(_ifRead, str);
				rtVec.push_back(str);
			}
			_uomapMaterials.insert(std::pair<_uint, VECTOR<_wstring>>(first, rtVec));
		}
	}
}
