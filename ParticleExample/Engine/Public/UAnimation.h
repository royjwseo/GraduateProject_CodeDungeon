#ifndef _PTH_FRAMEWORK_ENGIEN_PUBLIC_UANIMATION_H
#define _PTH_FRAMEWORK_ENGIEN_PUBLIC_UANIMATION_H

#include "UBase.h"

BEGIN(Engine)
class UAnimModel;
class UChannel;

class ENGINE_DLL DEF_CACHE_ALGIN  UAnimation : public UBase {
public:
	UAnimation();
	UAnimation(const UAnimation& _rhs);
	DESTRUCTOR(UAnimation)
public:
	const _wstring& GetAnimName() const { return m_wstrName; }
	const _bool IsFinished() const { return m_isFinished; }
	const _bool IsStop() const { return m_isStop; }
	const _bool IsRepeat() const { return m_isRepeat; }
	const _bool IsSupplySituation() const { return m_isSupplySituation; }

	// Set Finish
	void SetFinish(const _bool _isFinish) { this->m_isFinished = _isFinish; }
	void SetStop(const _bool _isStop) { this->m_isStop = _isStop; }
	void SetRepeat(const _bool _isRepeat) { this->m_isRepeat = _isRepeat; }
	void SetSupplySituation(const _bool _isSupplySituation) { this->m_isSupplySituation = _isSupplySituation; }
public:
	SHPTR<UAnimation> Clone(CSHPTRREF<UAnimModel> _spAnimModel);
	virtual void Free() override;
	HRESULT NativeConstruct(CSHPTRREF<UAnimModel> _spAnimModel, const ANIMDESC& _stAnimDesc);
	void UpdateTransformMatrices(const _double& _dTimeDelta);
	void UpdateNextAnimTransformMatrices(const _double& _dTimeDelta, const _float _fSupplyValue,
		CSHPTRREF<UAnimation> _spAnimation);

	void ResetData();
	// Save Sections
	void SaveSections(const _wstring& _wstrPath);
	void LoadSections(const _wstring& _wstrPath);
private:
	using CHANNELS = VECTOR<SHPTR<UChannel>>;
	using ANIMFASTSECTIONS = VECTOR<ANIMFASTSECTION>;
	using ANIMCLIPSECTIONS = VECTOR<ANIMCLIPSECTION>;

	static constexpr _float	MAX_SUPPLY_VALUE{1.f};

	CHANNELS						m_Channels;
	_wstring								m_wstrName;
	_uint									m_iNumChannels;
	_double								m_dTickPerSeconds;
	_double								m_dDuration;
	_double								m_dTimeAcc;
	_bool									m_isFinished;
	_bool									m_isStop;
	_bool									m_isRepeat;
	_bool									m_isSupplySituation;

	_float									m_fSupplySituationValue;

	ANIMFASTSECTIONS		m_AnimFastSections;
	ANIMCLIPSECTIONS		m_AnimClipSection;
};

END


#endif // _PTH_FRAMEWORK_ENGIEN_PUBLIC_UANIMATION_H