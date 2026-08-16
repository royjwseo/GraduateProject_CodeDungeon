#ifndef _PTH_FRAMEWORK_ENGIEN_PUBLIC_UINPUTMANAGER_H
#define _PTH_FRAMEWORK_ENGIEN_PUBLIC_UINPUTMANAGER_H

#include "UBase.h"

BEGIN(Engine)

using KEYMETHOD = UDelegate<void, const _double&>;

class UInputManager : public UBase {
public:
	UInputManager();
	NO_COPY(UInputManager)
	DESTRUCTOR(UInputManager)
public:
	void SetCursorLock(const CURSOR_LOCKMODE& _eLockMode);
	const CURSOR_LOCKMODE& GetCursorMode() const { return m_eCursorLockMode; }
public:
	// Override
	virtual void Free() override;
public:
	HRESULT ReadyInpuDevice(CSHPTRREF<GRAPHICDESC> _spGraphicDesc);
	void MouseTick();
	void KeyTick();
	void InvokeKeyMethods(const _ubyte& _bFirstKeyID, const _double& _dDeltaTime);

	void AddKeyMethod(const _ubyte& _bFirstKeyID, const _wstring& _wstrSecondsKeyName, const KEYMETHOD& _keyMethod);
	void DeleteKeyMethod(const _ubyte& _bFirstKeyID, const _wstring& _wstrSecondsKeyName);
public:
	_bool GetDIKeyDown(const _ubyte& _bKeyID);
	_bool GetDIKeyUp(const _ubyte& _bKeyID);
	_bool GetDIKeyPressing(const _ubyte& _bKeyID);
	_bool GetDIMBtnDown(const DIMOUSEBUTTON& _eMouseBtn);
	_bool GetDIMBtnUp(const DIMOUSEBUTTON& _eMouseBtn);
	_bool GetDIMBtnPressing(const DIMOUSEBUTTON& _eMouseBtn);
	_long GetDIMMoveState(const DIMOUSEMOVE& _eMouseMove) { return *((_long*)&m_eMouseState + _eMouseMove); }
	// Get Mouse Position
	_float2 GetMousePosition();

private:
	RECT GetClientAsRect();

	_bool	IsMouseInWindowSize();
private:
	using	KEYARR = ARRAY<_ubyte, MAX_KEYSIZE>;
	using	KEYMETHODS = UNORMAP<_wstring, KEYMETHOD>;
	using	KEYBINDS = ARRAY< KEYMETHODS, MAX_KEYSIZE>;

	LPDIRECTINPUT8								m_pInput;
	LPDIRECTINPUTDEVICE8				m_pKeyboard;
	LPDIRECTINPUTDEVICE8				m_pMouse;

	KEYARR												m_arrKeyState;
	DIMOUSESTATE								m_eMouseState;
	KEYARR												m_arrPrevKeyState;
	DIMOUSESTATE								m_ePrevMouseState;

	// Using Press Key 
	KEYBINDS											m_arrKeyBinds;

	CURSOR_LOCKMODE						m_eCursorLockMode;
	SHPTR<GRAPHICDESC>					m_spGraphicDesc;
	RECT													m_stWindowRect;
};

END

#endif // _PTH_FRAMEWORK_ENGIEN_PUBLIC_UINPUTMANAGER_H