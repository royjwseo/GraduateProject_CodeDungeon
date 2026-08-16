#include "ClientDefine.h"
#include "CClientApp.h"
#include "UGameInstance.h"
#include "UTimer.h"
#include "CProtoMaker.h"
#include "CStage1Scene.h"

CClientApp::CClientApp() :
	m_iTickCount{ 0 },
	m_hwnd{ NULL },
	m_dRenderTick{ 0.0 },
	m_spGameInstance{ nullptr },
	m_spTickTimer{ nullptr },
	m_spDeltaTimer{ nullptr },
	m_spRenderDeltaTimer{nullptr},
	m_spRenderTimer{ nullptr },
	m_dDeltaTime{ 0 },
	TICK_DOUBLE{ 1 / 60.f },
	m_dShowTickFPS{ 0.0 },
	DELTA_TIMER{ L"DELTA_TIMER" },
	TICK_TIMER{ L"TICK_TIMER" },
	RENDER_TIMER{ L"RENDER_TIMER" },
	RENDER_DELETATIMER{L"RENDER_DELTATIMER"},
	m_isTickThread{ true },
	m_isRenderingThread{true},
	m_isTickEnd{false}
{
}

void CClientApp::Free()
{
}

HRESULT CClientApp::NativeConstruct(const HINSTANCE& _hInst, const _uint& _iCmdShow)
{
	m_hInst = _hInst;
	m_iCmdShow = _iCmdShow;
	m_spGameInstance = GET_INSTANCE(UGameInstance);

	m_spDeltaTimer = m_spGameInstance->CreateTimerAdd(DELTA_TIMER);
	m_spTickTimer = m_spGameInstance->CreateTimerAdd(TICK_TIMER);
	m_spRenderTimer = m_spGameInstance->CreateTimerAdd(RENDER_TIMER);
	m_spRenderDeltaTimer = m_spGameInstance->CreateTimerAdd(RENDER_DELETATIMER);

	return S_OK;
}

void CClientApp::Render()
{
	if (FALSE == InitInstance(m_hInst, m_iCmdShow))
	{
		return;
	}

	/* Graphic Desc */
	GRAPHICDESC	stGraphicDesc;
	{
		stGraphicDesc.eWinMode = GRAPHICDESC::MODE_WIN;
		stGraphicDesc.hInst = m_hInst;
		stGraphicDesc.hWnd = m_hwnd;
		stGraphicDesc.iWinCX = WINDOW_WIDTH;
		stGraphicDesc.iWinCY = WINDOW_HEIGHT;
	}
	OUTPUTDATA stOutputData;
	RETURN_CHECK_FAILED(m_spGameInstance->ReadyInstance(stGraphicDesc, stOutputData), ;);
	RETURN_CHECK_FAILED(m_spGameInstance->LoadFirstFilder(FIRST_RESOURCE_FOLDER), ;);

	CProtoMaker::CreateProtoData(m_spGameInstance, stOutputData.wpDevice.lock(), stOutputData.wpGpuCmd.lock());

	// Register 
	m_spGameInstance->RegisterScene(CreateConstructorNative<CStage1Scene>(stOutputData.wpDevice.lock()));

	MSG msg{};
	ZeroMemory(&msg, sizeof(MSG));


	// 기본 메시지 루프입니다:
	while (m_spGameInstance->IsGamming())
	{
		// if PeekMeesage
		while (::PeekMessage(&msg, NULL, 0U, 0U, PM_REMOVE))
		{
			::TranslateMessage(&msg);
			::DispatchMessage(&msg);
			if (msg.message == WM_QUIT)
			{
				m_isRenderingThread = false;
				m_isTickThread = false;
				m_spGameInstance->Free();
				return;
			}
		}


		// Run Func
		m_spDeltaTimer->Tick();

		m_dDeltaTime += m_spDeltaTimer->GetDeltaTime();
		if (m_dDeltaTime >= TICK_DOUBLE)
		{
			m_spTickTimer->Tick();
			m_dDeltaTime = m_spTickTimer->GetDeltaTime();

			m_spGameInstance->AwakeTick();
			// Tick 
			m_spGameInstance->Tick(m_dDeltaTime);
			// LateTick
			m_spGameInstance->LateTick(m_dDeltaTime);
			// Render
			m_spGameInstance->RenderBegin();
			m_spGameInstance->RenderEnd();

			++m_iTickCount;

			m_dShowTickFPS += m_dDeltaTime;
			if (m_dShowTickFPS >= 1.0)
			{
				static _tchar FPS[200];
				wsprintf(FPS, TEXT("RFPS : %d"), m_iTickCount);
				SetWindowTextW(m_hwnd, FPS);
				m_iTickCount = 0;
				m_dShowTickFPS = 0.0;
			}
			m_dDeltaTime = 0.0;
		}
	}
}

void CClientApp::KeyUpdate(WPARAM _wParam)
{
	m_spGameInstance->OtherFrame(m_dDeltaTime, _wParam);
}

BOOL CClientApp::InitInstance(HINSTANCE hInstance, int nCmdShow)
{
	RECT rc = { 0, 0, WINDOW_WIDTH, WINDOW_HEIGHT };

	AdjustWindowRect(&rc, WS_OVERLAPPEDWINDOW, true);

	m_hwnd = CreateWindowW(L"Client", L"Clients", WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX,
		0, 0, rc.right - rc.left, rc.bottom - rc.top, nullptr, nullptr, hInstance, nullptr);

	if (!m_hwnd)
	{
		return FALSE;
	}

	ShowWindow(m_hwnd, nCmdShow);
	UpdateWindow(m_hwnd);

	return TRUE;
}
