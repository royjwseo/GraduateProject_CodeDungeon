#include "EngineDefines.h"
#include <fstream>
#include "UAnimModel.h"
#include "UMethod.h"
#include "UAnimation.h"
#include <filesystem>
#include "UShader.h"
#include "UMeshContainer.h"
#include "UBoneNode.h"
#include "UShaderConstantBuffer.h"
#include "UTexture.h"
#include "UDevice.h"
#include "UTexGroup.h"
#include "UGpuCommand.h"
#include "UGameInstance.h"

namespace fs = std::filesystem;

UAnimModel::UAnimModel(CSHPTRREF<UDevice> _spDevice) :
	UModel(_spDevice, TYPE::ANIM),
	m_vecAnimations{},
	m_vecSetupBonMatrix{},
	m_AnimNamesGroup{},
	m_spAnimShaderConstantBuffer{ nullptr },
	m_stAnimParam{},
	m_spBoneMatrixShaderConstantBuffer{ nullptr },
	m_spPrevBoneMatrixShaderConstantBuffer{ nullptr },
	m_cpUpLoadResource{ nullptr },
	m_spCurAnimation{ nullptr },
	m_spNextAnimation{ nullptr },
	m_iCurAnimIndex{ 0 },
	m_iNextAnimIndex{ 0 },
	m_fSupplyLerpValue{ 0.f },
	m_isChangeAnim{ false }
{
}

UAnimModel::UAnimModel(const UAnimModel& _rhs) : 
	UModel(_rhs),
	m_vecAnimations{ _rhs.m_vecAnimations },
	m_AnimNamesGroup{ _rhs.m_AnimNamesGroup },
	m_spAnimShaderConstantBuffer{ nullptr },
	m_stAnimParam{},
	m_spBoneMatrixShaderConstantBuffer{ nullptr },
	m_spPrevBoneMatrixShaderConstantBuffer{ nullptr },
	m_cpUpLoadResource{ nullptr },
	m_spCurAnimation{ nullptr },
	m_spNextAnimation{ nullptr },
	m_iCurAnimIndex{ 0 },
	m_iNextAnimIndex{ 0 },
	m_fSupplyLerpValue{ 0.f },
	m_isChangeAnim{ false }
{

}

void UAnimModel::Free()
{
}

HRESULT UAnimModel::NativeConstruct()
{
	return __super::NativeConstruct();
}

HRESULT UAnimModel::NativeConstructClone(const VOIDDATAS& _vecDatas)
{
	RETURN_CHECK_FAILED(__super::NativeConstructClone(_vecDatas), E_FAIL);

	SHPTR<UAnimModel> spAnimModel = ThisShared<UAnimModel>();

	ANIMATIONS vecAnims;
	vecAnims.reserve(m_vecAnimations.size());
	for(auto& iter : m_vecAnimations)
	{
		vecAnims.push_back(iter->Clone(spAnimModel));
	}
	m_vecAnimations = vecAnims;

	if (m_vecAnimations.size() > 0)
	{
		// 애니메이션이 nullptr일 때, Animation 0으로 초기화 
		SetAnimation(0);
	}

	RETURN_CHECK_FAILED(CreateShaderConstantBuffer(), E_FAIL);
	return S_OK;
}

void UAnimModel::TickAnimation(const _double& _dDeltaTime)
{
	if (nullptr != m_spNextAnimation)
	{
		m_spCurAnimation->UpdateNextAnimTransformMatrices(_dDeltaTime, m_fSupplyLerpValue,
			m_spNextAnimation);
		if (false == m_spCurAnimation->IsSupplySituation())
		{
			m_spCurAnimation = m_spNextAnimation;
			m_spNextAnimation = nullptr;
		}
		m_isChangeAnim = true;
	}
	else
	{
		m_spCurAnimation->UpdateTransformMatrices(_dDeltaTime);
		m_isChangeAnim = false;
	}

	/* 부모로부터 자식뼈에게 누적시켜 전달한다.(CombinedTransformationMatrix) */
	{
		for (auto& BoneNode : GetBoneNodes())
			BoneNode->UpdateCombinedMatrix();
	}
}

HRESULT UAnimModel::Render(const _uint _iMeshIndex, CSHPTRREF<UShader> _spShader, CSHPTRREF<UCommand> _spCommand)
{
	RETURN_CHECK(nullptr == _spShader, E_FAIL);
	RETURN_CHECK(GetMeshContainerCnt() <= _iMeshIndex, E_FAIL);
	RETURN_CHECK(nullptr == GetMeshContainers()[_iMeshIndex], E_FAIL);
	// 이전 본 값 세팅
	_spShader->BindCBVBuffer(m_spPrevBoneMatrixShaderConstantBuffer, m_vecSetupBonMatrix[_iMeshIndex].data(), BONEMATRIXPARA_SIZE);
	CSHPTRREF<UMeshContainer> spMeshContainer {GetMeshContainers()[_iMeshIndex]};
	spMeshContainer->SetUpBoneMatrix(m_vecSetupBonMatrix[_iMeshIndex]);
	// 현재 본 값 세팅
	_spShader->BindCBVBuffer(m_spBoneMatrixShaderConstantBuffer, m_vecSetupBonMatrix[_iMeshIndex].data(), BONEMATRIXPARA_SIZE);
	// 애니메이션 세팅
	_spShader->BindCBVBuffer(m_spAnimShaderConstantBuffer, &m_stAnimParam, ANIMPARAM_SIZE);
	spMeshContainer->Render(_spShader, _spCommand);
	return S_OK;
}

void UAnimModel::SetAnimation(const _uint& _iAnimIndex)
{
	ChangeAnimIndex(_iAnimIndex, m_iCurAnimIndex);
	// 현재 애니메이션이 세팅되는 상황일 때의 함수 실행
	SettingCurAnimSituation();
}

void UAnimModel::SetAnimation(const _wstring& _wstrAnimName)
{
	ChangeAnimIndex(_wstrAnimName, m_iCurAnimIndex);
	// 현재 애니메이션이 세팅되는 상황일 때의 함수 실행
	SettingCurAnimSituation();
}

void UAnimModel::ChangeAnimation(const _uint& _iAnimIndex)
{
	ChangeAnimIndex(_iAnimIndex, m_iNextAnimIndex);
	// 다음 애니메이션이 세팅되는 상황일 때의 함수 실행
	SettingNextAnimSituation();
}

void UAnimModel::ChangeAnimation(const _wstring& _wstrAnimName)
{
	ChangeAnimIndex(_wstrAnimName, m_iNextAnimIndex);
	// 다음 애니메이션이 세팅되는 상황일 때의 함수 실행
	SettingNextAnimSituation();
}

HRESULT UAnimModel::CreateAnimation(const  VECTOR<ANIMDESC>& _convecAnimDesc, const _wstring& _wstrPath)
{
	m_vecAnimations.reserve(_convecAnimDesc.size());
	_uint iIndex{ 0 };
	for (auto& iter : _convecAnimDesc)
	{
		SHPTR<UAnimation> spAnimation {CreateNative<UAnimation>(ThisShared<UAnimModel>(), iter)};
		RETURN_CHECK(nullptr == spAnimation, E_FAIL);

		m_vecAnimations.push_back(spAnimation);

		m_AnimNamesGroup.insert(std::pair<_wstring, _uint>(spAnimation->GetAnimName(), iIndex++));

		_wstring wstrPath = _wstrPath;
		wstrPath += spAnimation->GetAnimName();
		wstrPath += DEFAULT_OUTFOLDEREXTENSION;
		spAnimation->LoadSections(wstrPath);
	}
	return S_OK;
}

void UAnimModel::LoadToData(const _wstring& _wstrPath)
{
	std::ifstream read{_wstrPath, std::ios::binary};
	RETURN_CHECK(!read, ;);

	ANIMMODELDESC tDesc;
	{
		// MESH
		LoadAnimMeshData(read, tDesc.vecMeshes);
		// BoneNode
		LoadBoneData(read, tDesc.vecBNodeDatas);
		// Material
		LoadMaterial(read, tDesc.uomapMatrialData);
		// Animations
		LoadAnimationData(read, tDesc.convecAnimes);

		fs::path FolderPath{_wstrPath};
		_wstring wstrPath = FolderPath;
		_uint iFindIndex{ static_cast<_uint>(wstrPath.find(FolderPath.filename())) };
		wstrPath = wstrPath.substr(0, iFindIndex);
		wstrPath += SECTION_FOLDEDER_NAME;
		// Create
		CreateBoneNode(&tDesc);
		CreateMeshContainers(&tDesc);
		CreateMaterial(&tDesc);
		CreateAnimation(tDesc.convecAnimes, wstrPath);


		m_vecSetupBonMatrix.resize(GetMeshContainerCnt());
		for (_uint i = 0; i < GetMeshContainerCnt(); ++i)
		{
			for (auto& iter : m_vecSetupBonMatrix[i])
				iter = _float4x4::Identity;
		}

#ifdef _USE_DEBUGGING
		CreateShaderConstantBuffer();
#endif

		SetAnimation(5);
	}
}

void UAnimModel::LoadAnimMeshData(std::ifstream& _ifRead, VECTOR<ANIMMESHDESC>& _convecMeshes)
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
				iter.pVtxModel = new VTXANIMMODEL[iter.iNumVertices];
				size_t vtxModel = sizeof(VTXANIMMODEL) * iter.iNumVertices;
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

void UAnimModel::LoadAnimationData(std::ifstream& _ifRead, VECTOR<ANIMDESC>& _convecAnimes)
{
	_uint iSize{ 0 };
	_ifRead.read((_char*)&iSize, sizeof(_uint));
	if (0 != iSize)
	{
		_convecAnimes.resize(iSize);
		for (auto& Animation : _convecAnimes)
		{
			_ifRead.read((_char*)&Animation.stExtraData, sizeof(ANIMEXTRADATA));
			UMethod::ReadString(_ifRead, Animation.wstrName);
			_uint iChannelSize{ 0 };
			_ifRead.read((_char*)&iChannelSize, sizeof(_uint));
			if (iChannelSize == 0)
				continue; 
			Animation.vecChannels.resize(iChannelSize);
			for (auto& Channel : Animation.vecChannels)
			{
				UMethod::ReadString(_ifRead, Channel.wstrBoneName);
				_ifRead.read((_char*)&Channel.iNumMaxKeyFrames, sizeof(_uint));
				Channel.pKeyFrames = new KEYFRAME[Channel.iNumMaxKeyFrames];
				_ifRead.read((_char*)Channel.pKeyFrames, sizeof(KEYFRAME) * Channel.iNumMaxKeyFrames);
			}
		}
	}
}

void UAnimModel::ChangeAnimIndex(const _uint& _iAnimIndex, _uint& _iIndex)
{
	if (m_vecAnimations.size() <= _iAnimIndex)
	{
		_iIndex = 0;
	}
	else
	{
		_iIndex = _iAnimIndex;
	}
}

void UAnimModel::ChangeAnimIndex(const _wstring& _wstrAnimName, _uint& _iIndex)
{
	const auto& FindIterator = m_AnimNamesGroup.find(_wstrAnimName);
	if (m_AnimNamesGroup.end() == FindIterator)
	{
		_iIndex = 0;
	}
	else
	{
		_iIndex = FindIterator->second;
	}
}

void UAnimModel::SettingCurAnimSituation()
{
	RETURN_CHECK(nullptr == m_vecAnimations[m_iCurAnimIndex], ;);
	m_spCurAnimation = m_vecAnimations[m_iCurAnimIndex];
	m_spCurAnimation->ResetData();
	m_spNextAnimation = nullptr;
}

void UAnimModel::SettingNextAnimSituation()
{
	m_spNextAnimation = m_vecAnimations[m_iNextAnimIndex];
	// Reset
	m_spCurAnimation->SetFinish(false);
	m_spCurAnimation->SetSupplySituation(false);
}

HRESULT UAnimModel::CreateShaderConstantBuffer()
{
	m_spAnimShaderConstantBuffer = CreateNative<UShaderConstantBuffer>(GetDevice(), CBV_REGISTER::ANIMMODEL, ANIMPARAM_SIZE);
	RETURN_CHECK(nullptr == m_spAnimShaderConstantBuffer, E_FAIL);

	m_spBoneMatrixShaderConstantBuffer = CreateNative<UShaderConstantBuffer>(GetDevice(), CBV_REGISTER::BONEMATRIX, BONEMATRIXPARA_SIZE);
	RETURN_CHECK(nullptr == m_spBoneMatrixShaderConstantBuffer, E_FAIL);

	m_spPrevBoneMatrixShaderConstantBuffer = CreateNative<UShaderConstantBuffer>(GetDevice(), CBV_REGISTER::PREVBONEMATRIX, BONEMATRIXPARA_SIZE);
	RETURN_CHECK(nullptr == m_spPrevBoneMatrixShaderConstantBuffer, E_FAIL);

	return S_OK;
}