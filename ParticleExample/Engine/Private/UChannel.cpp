#include "EngineDefines.h"
#include "UChannel.h"
#include "UBoneNode.h"
#include "UAnimModel.h"
#include "UAnimation.h"
#include "UGameInstance.h"

UChannel::UChannel() : 
	UBase(),
m_wstrBoneName{L""},
m_iNumKeyMaxRealSize{0},
m_spBoneNode{nullptr},
m_vecKeyFrames{},
m_iCurrentKeyFrames{0},
m_dCurTimeAcc{0},
m_vScale{},
m_vRotation{},
m_vPosition{}
{
}

UChannel::UChannel(const UChannel& _rhs) :
	UBase(_rhs),
	m_wstrBoneName{ _rhs.m_wstrBoneName },
	m_iNumKeyMaxRealSize{ _rhs.m_iNumKeyMaxRealSize },
	m_spBoneNode{ _rhs.m_spBoneNode },
	m_iCurrentKeyFrames{ _rhs.m_iCurrentKeyFrames },
	m_vecKeyFrames{ _rhs.m_vecKeyFrames },
	m_dCurTimeAcc{ 0 },
	m_vScale{},
	m_vRotation{},
	m_vPosition{}
{
}

SHPTR<UChannel> UChannel::Clone(CSHPTRREF<UAnimModel> _spAnimModel)
{
	SHPTR<UChannel> pChannel{CloneThis<UChannel>(*this)};
	pChannel->m_spBoneNode = _spAnimModel->GetBoneNode(m_spBoneNode->GetName());
	return pChannel;
}

void UChannel::Free()
{
}

HRESULT UChannel::NativeConstruct(CSHPTRREF<UAnimModel> _spAnimModel, const CHANNELDESC& _stChannelDesc)
{
	RETURN_CHECK(nullptr == _spAnimModel, E_FAIL);
	m_iNumKeyMaxRealSize = _stChannelDesc.iNumMaxKeyFrames - 1;
	m_vecKeyFrames.resize(_stChannelDesc.iNumMaxKeyFrames);
	::memcpy(&m_vecKeyFrames[0], _stChannelDesc.pKeyFrames, sizeof(KEYFRAME) * _stChannelDesc.iNumMaxKeyFrames);
	m_wstrBoneName = _stChannelDesc.wstrBoneName;
	m_spBoneNode = _spAnimModel->GetBoneNode(m_wstrBoneName);
	KEYFRAME* pKeyFrame = _stChannelDesc.pKeyFrames;
	Safe_Delete_Array(pKeyFrame);
	return S_OK;
}

// Update Transform Matrix
void UChannel::UpdateTransformMatrix(const _double& _dTimeAcc, UAnimation* _pAnimation)
{
	RETURN_CHECK(nullptr == _pAnimation, ;);

	if (_dTimeAcc >= m_vecKeyFrames[m_iNumKeyMaxRealSize].dTime)
	{
		m_vScale = m_vecKeyFrames[m_iNumKeyMaxRealSize ].vScale;
		m_vRotation = m_vecKeyFrames[m_iNumKeyMaxRealSize ].vRotation;
		m_vPosition = m_vecKeyFrames[m_iNumKeyMaxRealSize ].vPosition;
	}
	else
	{
		ComputeCurKeyFrames(_dTimeAcc, m_iCurrentKeyFrames);

		if (m_iCurrentKeyFrames >= m_iNumKeyMaxRealSize)
		{
			m_vScale = m_vecKeyFrames[m_iNumKeyMaxRealSize].vScale;
			m_vRotation = m_vecKeyFrames[m_iNumKeyMaxRealSize].vRotation;
			m_vPosition = m_vecKeyFrames[m_iNumKeyMaxRealSize].vPosition;
		}
		else
		{
			// 현재 키프레임하고 다음 키프레임하고 결합하는 것이다.
			_float fCurrentRatio = static_cast<_float>((_dTimeAcc - m_vecKeyFrames[m_iCurrentKeyFrames].dTime)
				/ (m_vecKeyFrames[m_iCurrentKeyFrames + 1].dTime - m_vecKeyFrames[m_iCurrentKeyFrames].dTime));

			m_vScale = DirectX::XMVectorLerp(m_vecKeyFrames[m_iCurrentKeyFrames].vScale,
				m_vecKeyFrames[m_iCurrentKeyFrames + 1].vScale, fCurrentRatio);
			m_vRotation = DirectX::XMQuaternionSlerp(m_vecKeyFrames[m_iCurrentKeyFrames].vRotation,
				m_vecKeyFrames[m_iCurrentKeyFrames + 1].vRotation, fCurrentRatio);
			m_vPosition = DirectX::XMVectorLerp(m_vecKeyFrames[m_iCurrentKeyFrames].vPosition,
				m_vecKeyFrames[m_iCurrentKeyFrames + 1].vPosition, fCurrentRatio);
		}
	}

	if (nullptr != m_spBoneNode)
	{
	//	std::lock_guard<std::mutex> LL{UGameInstance::GetMainMutex()};
		m_spBoneNode->SetTransformMatrix(DirectX::XMMatrixAffineTransformation(m_vScale, ZERO_VALUE,
			m_vRotation, m_vPosition));
	}
}

void UChannel::UpdatSupplyToCurAndNextTransformMatrix(const _double& _dTimeAcc, const _float _fRatio, 
	UAnimation* _pAnimation, CSHPTRREF<UChannel> _spChannel)
{
	RETURN_CHECK(nullptr == _pAnimation || nullptr == _spChannel, ;);
	// 현재 애니메이션과 다음 애니메이션의 첫 번째 값들을 Lerp 시킨다. 
	ComputeCurKeyFrames(_dTimeAcc, m_iCurrentKeyFrames);
	// 값들을 Lerp 시킴
	m_vScale = _float3::Lerp(m_vecKeyFrames[m_iCurrentKeyFrames].vScale,
		_spChannel->m_vecKeyFrames[0].vScale, _fRatio);
	m_vRotation = _float4::Lerp(m_vecKeyFrames[m_iCurrentKeyFrames].vRotation,
		_spChannel->m_vecKeyFrames[0].vRotation, _fRatio);
	m_vPosition = _float4::Lerp(m_vecKeyFrames[m_iCurrentKeyFrames].vPosition,
		_spChannel->m_vecKeyFrames[0].vPosition, _fRatio);

	if (nullptr != m_spBoneNode)
	{
//		std::lock_guard<std::mutex> LL{UGameInstance::GetMainMutex()};
		m_spBoneNode->SetTransformMatrix(DirectX::XMMatrixAffineTransformation(m_vScale, ZERO_VALUE,
			m_vRotation, m_vPosition));
	}
}

void UChannel::ComputeCurKeyFrames(const _double& _dTimeAcc, _uint& _iCurKeyFrame)
{
	// 키 프레임이 1개 밖에 없을 경우.
	if (m_vecKeyFrames.size() <= 1)
	{
		_iCurKeyFrame = 0;
	}
	// 1번 키프레임보다 작을 경우
	else if (_dTimeAcc < m_vecKeyFrames[1].dTime)
	{
		_iCurKeyFrame = 0;
	}
	else if (!(_dTimeAcc >= m_vecKeyFrames[m_iNumKeyMaxRealSize].dTime))
	{
		while (_dTimeAcc > m_vecKeyFrames[_iCurKeyFrame + 1].dTime)
		{
			++_iCurKeyFrame;
		}

		// 애니메이션을 뒤로 돌리는 경우에 KeyFrame을 맞춰준다.
		while (_dTimeAcc < m_vecKeyFrames[_iCurKeyFrame].dTime)
		{
			if (0 == _iCurKeyFrame)
				break;

			--_iCurKeyFrame;
		}
	}
	m_dCurTimeAcc = _dTimeAcc;
}
