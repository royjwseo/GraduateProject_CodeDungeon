#ifndef _PTH_FRAMEWORK_ENGIEN_PUBLIC_UANIMATOR_H
#define _PTH_FRAMEWORK_ENGIEN_PUBLIC_UANIMATOR_H

#include "UComponent.h"

BEGIN(Engine)
class UAnimation;
class UAnimModel;

class UAnimator : public UComponent {
public:
	UAnimator(CSHPTRREF<UDevice> _spDevice);
	UAnimator(const UAnimator& _rhs);
	DESTRUCTOR(UAnimator)
public:
	// UComponent을(를) 통해 상속됨
	virtual void Free() override;
	virtual HRESULT NativeConstruct() override;
	virtual HRESULT NativeConstructClone(const VOIDDATAS& _vecDatas) override;
	void Tick(const _double& _dDeltaTime);
private:
	SHPTR<UAnimModel>			m_spAnimModel;
	SHPTR<UAnimation>				m_spCurAnimation;
};

END

#endif // _PTH_FRAMEWORK_ENGIEN_PUBLIC_UCOMPONENT_H