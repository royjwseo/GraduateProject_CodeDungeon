#ifndef _PTH_FRAMEWORK_CLIENT_PUBLIC_CCLIENTAPP_H
#define _PTH_FRAMEWORK_CLIENT_PUBLIC_CCLIENTAPP_H

#include "UBase.h"
#include <latch>

BEGIN(Engine)
class UGameInstance;
class UTimer;
class UDevice;
class UGpuCommand;
END

BEGIN(Client)

class CClientApp final  : public UBase {
public:
	CClientApp();
	NO_COPY(CClientApp)
	DESTRUCTOR(CClientApp)
public:
	virtual void Free() override;
	HRESULT NativeConstruct(const HINSTANCE& _hInst, const _uint& _iCmdShow);
	// Tick
	void Render();
	void KeyUpdate(WPARAM _wParam);
private:
	BOOL InitInstance(HINSTANCE hInstance, int nCmdShow);
private:
	HINSTANCE									m_hInst;
	_uint												m_iCmdShow;
	_uint												m_iTickCount;
	HWND												m_hwnd;
	_double											m_dRenderTick;

	SHPTR<UGameInstance>			m_spGameInstance;

	SHPTR<UTimer>							m_spTickTimer;
	SHPTR<UTimer>							m_spDeltaTimer;
	SHPTR<UTimer>							m_spRenderTimer;
	SHPTR<UTimer>							m_spRenderDeltaTimer;

	_double											m_dDeltaTime;
	_double											m_dShowTickFPS;
	const _double								TICK_DOUBLE;

	const _wstring								DELTA_TIMER;
	const _wstring								TICK_TIMER;
	const _wstring								RENDER_TIMER;
	const _wstring								RENDER_DELETATIMER;

	_bool												m_isRenderingThread;
	_bool												m_isTickThread;
	_bool												m_isTickEnd;

};

END

#endif // _PTH_FRAMEWORK_CLIENT_PUBLIC_CCLIENTAPP_H