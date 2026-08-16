#include "EngineDefines.h"
#include "UAnimator.h"


UAnimator::UAnimator(CSHPTRREF<UDevice> _spDevice) :
	UComponent(_spDevice),
	m_spAnimModel{ nullptr },
	m_spCurAnimation{ nullptr }
{
}

UAnimator::UAnimator(const UAnimator& _rhs)  : UComponent(_rhs), 
	m_spAnimModel{ nullptr}, 
	m_spCurAnimation{nullptr}
{
}

void UAnimator::Free()
{
}

HRESULT UAnimator::NativeConstruct()
{
	RETURN_CHECK_FAILED(__super::NativeConstruct(), E_FAIL);

	return S_OK;
}

HRESULT UAnimator::NativeConstructClone(const VOIDDATAS& _vecDatas)
{
	RETURN_CHECK_FAILED(NativeConstructClone(_vecDatas), E_FAIL);

	return S_OK;
}

void UAnimator::Tick(const _double& _dDeltaTime)
{

}
