#ifndef _PTH_FRAMEWORK_ENGIEN_PUBLIC_UMESHCONTAINER_H
#define _PTH_FRAMEWORK_ENGIEN_PUBLIC_UMESHCONTAINER_H

#include "UVIBuffer.h"

BEGIN(Engine)
class UModel;
class UBoneNode;

class ENGINE_DLL DEF_CACHE_ALGIN  UMeshContainer : public UVIBuffer {
public:
	UMeshContainer(CSHPTRREF <UDevice> _spDevice);
	UMeshContainer(const UMeshContainer& _rhs);
	DESTRUCTOR(UMeshContainer)
public:
	const _uint GetMaterialIndex() const { return m_iMaterialIndex; }
	const _uint GetNumBones() const { return m_iNumBones; }
public:
	CLONE_MACRO(UMeshContainer, "UMeshContainer::Clone To Failed")
	virtual void Free() override;
	virtual HRESULT NativeConstruct() override;
	HRESULT NativeConstruct(void* _pData, CSHPTRREF<UModel> _spModel);
	virtual HRESULT NativeConstructClone(const VOIDDATAS& _vecDatas) override;
	HRESULT SetUpBoneMatrix(ARRAY<_float4x4, MAX_BONE_SIZE>& _arrBones);
private:
	HRESULT ReadyVertices(void* _pData, CSHPTRREF<UModel> _spModel);
	HRESULT ReadyAnimVertices(void* _pData, CSHPTRREF<UModel> _spModel);
	HRESULT ReadyIndicies(void* _pData);
private:
	/* Make To Vertex */
	HRESULT MakeVertices(void* _pVertices, void* _pPosition, const _uint _iVertexSize, const _uint _iVertexCnt
	, const _float3& _vMinPos, const _float3& _vMaxPos, const _wstring& _wstrName, const _uint& _iMaterialIndex);

	void ReceiveBoneNode(CSHPTRREF<UModel> _spModel, const VECTOR<_wstring>& _convecBoneNameList, const _uint& _iBoneNodeCnt);
private:
	using BONENODES = VECTOR<SHPTR<UBoneNode>>;
	_uint							m_iMaterialIndex;
	_uint							m_iNumBones;
	_uint							m_iNumBuffers;
	_wstring						m_wstrName;
	BONENODES				m_vecBoneNodes;
};

END

#endif // _PTH_FRAMEWORK_ENGIEN_PUBLIC_UMESHCONTAINER_H