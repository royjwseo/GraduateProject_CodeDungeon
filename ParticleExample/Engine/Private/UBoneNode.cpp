#include "EngineDefines.h"
#include "UBoneNode.h"
#include "UModel.h"

_wstring					UBoneNode::BASE_BONENODE_NAME{L""};


UBoneNode::UBoneNode() :
	m_mOffsetMatrix{ _float4x4::Identity },
	m_mTransformMatrix{ _float4x4::Identity },
	m_mCombineTransformMatirx{ _float4x4::Identity },
	m_spParentsNode{ nullptr },
	m_wstrName{ L"" },
	m_wstrParetnsName{ L"" },
	m_fDepths{ 0.f }
{
}

UBoneNode::UBoneNode(const UBoneNode& _rhs) :
	m_mOffsetMatrix{ _float4x4::Identity },
	m_mTransformMatrix{ _float4x4::Identity },
	m_mCombineTransformMatirx{ _float4x4::Identity },
	m_spParentsNode{ nullptr },
	m_wstrName{ _rhs.m_wstrName },
	m_wstrParetnsName{ _rhs.m_wstrParetnsName },
	m_fDepths{ _rhs.m_fDepths }
{
}

SHPTR<UBoneNode> UBoneNode::Clone()
{
	return { CloneThis<UBoneNode>(*this) };
}

void UBoneNode::Free()
{
}

HRESULT UBoneNode::NativeConstruct(const BONENODEDESC& _stBoneNodeDesc)
{
	m_wstrName = _stBoneNodeDesc.wstrName;
	m_wstrParetnsName = _stBoneNodeDesc.wstrParentsName;
	m_fDepths = _stBoneNodeDesc.fDepth;
	m_mOffsetMatrix = _stBoneNodeDesc.mOffsetMatrix;
	m_mTransformMatrix = _stBoneNodeDesc.mTransformMatrix;

	return S_OK;
}

void UBoneNode::FindParents(CSHPTRREF<UModel> _spModel)
{
	m_spParentsNode = _spModel->GetBoneNode(m_wstrParetnsName);
}

void UBoneNode::UpdateCombinedMatrix()
{
	if (nullptr != m_spParentsNode)
	{
		m_mCombineTransformMatirx = m_mTransformMatrix * m_spParentsNode->GetCombineMatrix();
	} 
	else
	{
		m_mCombineTransformMatirx = m_mTransformMatrix;
	}
}
