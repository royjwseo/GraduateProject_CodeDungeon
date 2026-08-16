#ifndef _PTH_FRAMEWORK_CLIENT_PUBLIC_CMAINCAMERA_H
#define _PTH_FRAMEWORK_CLIENT_PUBLIC_CMAINCAMERA_H

#include "UCamera.h"

BEGIN(Client)

class CMainCamera final : public UCamera {
public:
	CMainCamera(CSHPTRREF<UDevice> _spDevice,	const _wstring& _wstrLayer, const CLONETYPE& _eCloneType);
	CMainCamera(const CMainCamera& _rhs);
	DESTRUCTOR(CMainCamera)
public:
	const _bool IsMoveState() const { return m_isMoveState; }
	void SetMoveState(const _bool _isMoveState) { this->m_isMoveState = _isMoveState; }
public:
	CLONE_MACRO(CMainCamera, "CMainCamera::Clone To Failed")
	virtual void Free() override;
	virtual HRESULT NativeConstruct() override;
	virtual HRESULT NativeConstructClone(const VOIDDATAS& _vecDatas) override;
protected:
	virtual void TickActive(const _double& _dTimeDelta) override;
	virtual void LateTickActive(const _double& _dTimeDelta) override;
private:
	_bool				m_isMoveState;
	_bool				m_isLock;
	_bool				m_isSaveLock;
	_bool				m_isEsc;
	CUSTIMER		m_Timer;
};

END

#endif // _PTH_FRAMEWORK_CLIENT_PUBLIC_CMAINCAMERA_H