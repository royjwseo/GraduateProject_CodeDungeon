#include "EngineDefines.h"
#include <fstream>
#include "UAnimation.h"
#include "UChannel.h"
#include "UGameInstance.h"

UAnimation::UAnimation() :
	UBase(),
	m_Channels{},
	m_wstrName{ L"" },
	m_iNumChannels{ 0 },
	m_dTickPerSeconds{ 0 },
	m_dDuration{ 0 },
	m_dTimeAcc{ 0 },
	m_isFinished{ false },
	m_isStop{false},
	m_isRepeat{false},
	m_isSupplySituation{false},
	m_fSupplySituationValue{0}
{
}

UAnimation::UAnimation(const UAnimation& _rhs) :
	UBase(_rhs),
	m_Channels{ _rhs.m_Channels},
	m_wstrName{ _rhs.m_wstrName },
	m_iNumChannels{ _rhs.m_iNumChannels },
	m_dTickPerSeconds{ _rhs.m_dTickPerSeconds },
	m_dDuration{ _rhs.m_dDuration },
	m_dTimeAcc{ 0.0 },
	m_isFinished{ false },
	m_isStop{ false },
	m_isRepeat{ false },
	m_isSupplySituation{ false },
	m_fSupplySituationValue{ 0 }
{
}

SHPTR<UAnimation> UAnimation::Clone(CSHPTRREF<UAnimModel> _spAnimModel)
{
	SHPTR<UAnimation> pAnimation = CloneThis<UAnimation>(*this);

	for (auto & iter : m_Channels)
		pAnimation->m_Channels.push_back(iter->Clone(_spAnimModel));

	return pAnimation;
}

void UAnimation::Free()
{
	m_Channels.clear();
}

HRESULT UAnimation::NativeConstruct(CSHPTRREF<UAnimModel> _spAnimModel, const ANIMDESC& _stAnimDesc)
{
	m_iNumChannels = static_cast<_uint>(_stAnimDesc.vecChannels.size());
	m_dDuration = _stAnimDesc.stExtraData.dDuration;
	m_dTickPerSeconds = _stAnimDesc.stExtraData.dTickPerSeconds;
	m_wstrName = _stAnimDesc.wstrName;
	m_Channels.reserve(m_iNumChannels);
	for (auto& iter : _stAnimDesc.vecChannels)
	{
		SHPTR<UChannel> pChannel{CreateNative<UChannel>(_spAnimModel, iter)};
		m_Channels.push_back(pChannel);
	}

	return S_OK;
}

void UAnimation::UpdateTransformMatrices(const _double& _dTimeDelta)
{
	RETURN_CHECK(true == m_isStop, ;);
	if (true == m_isRepeat)
	{
		RETURN_CHECK(true == m_isFinished, ;);
	}
	_double dValue = m_dTickPerSeconds * _dTimeDelta ;
	for (auto& iter : m_AnimFastSections)
	{
		 iter.Convert(dValue, m_dTimeAcc, dValue);
	}
	m_dTimeAcc += dValue;

	if (m_dTimeAcc >= m_dDuration)
	{
		m_dTimeAcc = 0.0;
		m_isFinished = true;
		m_isSupplySituation = false;
	}
	else
	{
		for(auto& iter : m_Channels)
		{
			iter->UpdateTransformMatrix(m_dTimeAcc, this);
		}
	}
}

void UAnimation::UpdateNextAnimTransformMatrices(const _double& _dTimeDelta, const _float _fSupplyValue, CSHPTRREF<UAnimation> _spAnimation)
{
	m_fSupplySituationValue += (_float)(_dTimeDelta)*_fSupplyValue;

	if (m_fSupplySituationValue >= MAX_SUPPLY_VALUE)
	{
		ResetData();
	}
	else
	{
		if (_spAnimation->m_Channels.size() > 0)
		{
			m_isSupplySituation = true;
			for (_uint i = 0; i < m_iNumChannels; ++i)
			{
				m_Channels[i]->UpdatSupplyToCurAndNextTransformMatrix(m_dTimeAcc, m_fSupplySituationValue, 
					this, _spAnimation->m_Channels[i]);
			}
		}
	}
}

void UAnimation::ResetData()
{
	m_dTimeAcc = 0.0;
	m_fSupplySituationValue = 0.001f;
	m_isSupplySituation = false;
	m_isFinished = false;
}

/*
====================================================
Save
====================================================
*/
void UAnimation::SaveSections(const _wstring& _wstrPath)
{
	std::wofstream Saves{_wstrPath, std::ios::binary };
	RETURN_CHECK(!Saves, ;);

	_uint iFastSection = static_cast<_uint>(m_AnimFastSections.size());
	// Save Fast Section 
	Saves.write((_tchar*)&iFastSection, sizeof(_uint));
	Saves.write((_tchar*)&m_AnimFastSections, sizeof(ANIMFASTSECTION) * iFastSection);
	// Save Clip Section
	_uint iClipSection = static_cast<_uint>(m_AnimClipSection.size());
	// Save Clip Section
	Saves.write((_tchar*)&iClipSection, sizeof(_uint));
	Saves.write((_tchar*)&m_AnimClipSection, sizeof(ANIMCLIPSECTION) * iClipSection);
}

void UAnimation::LoadSections(const _wstring& _wstrPath)
{
	std::wifstream Read{_wstrPath, std::ios::binary };
	RETURN_CHECK(!Read, ;);

	_uint iFastSection{ 0 };
	Read.read((_tchar*)&iFastSection, sizeof(_uint));
	m_AnimFastSections.resize(iFastSection);
	Read.read((_tchar*)&m_AnimFastSections, sizeof(ANIMFASTSECTION) * iFastSection);

	_uint iClipSection{ 0 };
	Read.read((_tchar*)&iClipSection, sizeof(_uint));
	m_AnimClipSection.resize(iClipSection);
	Read.read((_tchar*)&m_AnimClipSection, sizeof(ANIMCLIPSECTION) * iClipSection);
}

/*
====================================================
Save
====================================================
*/