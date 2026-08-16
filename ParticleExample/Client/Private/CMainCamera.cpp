#include "ClientDefine.h"
#include "CMainCamera.h"
#include "UGameInstance.h"
#include "UTransform.h"
#include "UInputManager.h"

CMainCamera::CMainCamera(CSHPTRREF<UDevice> _spDevice,	const _wstring& _wstrLayer, const CLONETYPE& _eCloneType) : 
	UCamera(_spDevice, _wstrLayer, _eCloneType),
	m_isMoveState{ false }, m_isLock{false}, m_isSaveLock{false}, m_isEsc{false}, m_Timer{0.5f}
{
}

CMainCamera::CMainCamera(const CMainCamera& _rhs) :
	UCamera(_rhs),
	m_isMoveState{ false }, m_isLock{ false }, m_isSaveLock{false}, m_isEsc{ false }, m_Timer{ 0.5f }
{
}

void CMainCamera::Free()
{
}

HRESULT CMainCamera::NativeConstruct()
{
	if (FAILED(__super::NativeConstruct()))
		return E_FAIL;

	return S_OK;
}

HRESULT CMainCamera::NativeConstructClone(const VOIDDATAS& _vecDatas)
{
	if (FAILED(__super::NativeConstructClone(_vecDatas)))
		return E_FAIL;

    SHPTR<UGameInstance> pGameInstance = GET_INSTANCE(UGameInstance);
    pGameInstance->SetCursorLock(CURSOR_LOCKMODE::CONFINED);
    m_isLock = true;
	return S_OK;
}

void CMainCamera::TickActive(const _double& _dTimeDelta)
{
    SHPTR<UGameInstance> pGameInstance = GET_INSTANCE(UGameInstance);

    m_Timer.IsOver(_dTimeDelta);

    if (true == pGameInstance->GetDIKeyDown(DIK_ESCAPE))
    {
        m_isEsc = !m_isEsc;
    }

    if (true == pGameInstance->GetDIKeyDown(DIK_SPACE))
    {
        m_isEsc = false;
    }
    if (true == pGameInstance->GetDIMBtnDown(DIMB_L) || true == pGameInstance->GetDIMBtnDown(DIMB_R))
    {
        _float2 MousePos = pGameInstance->GetMousePosition();
        if (MousePos.x > 0 && MousePos.x < WINDOW_WIDTH)
        {
            if (MousePos.y > 0 && MousePos.y < WINDOW_HEIGHT)
            {
                m_isEsc = false;
            }
        }
    }

    if (true == m_isEsc)
        return;
    if (m_Timer.IsPass())
    {
        if (true == pGameInstance->GetDIKeyPressing(DIK_TAB))
        {
            if (false == m_isLock)
            {
                pGameInstance->SetCursorLock(CURSOR_LOCKMODE::CONFINED);
                m_isLock = true;
            }
            else
            {
                pGameInstance->SetCursorLock(CURSOR_LOCKMODE::NONE);
                m_isLock = false;
            }
        }
        m_Timer.ResetTimer();
    }

    if (true == m_isMoveState)
    {
        _float fSpeed{ 1000.f };

        if (pGameInstance->GetDIKeyPressing(DIK_W))
            GetTransform()->MoveForward(_dTimeDelta, fSpeed);
        if (pGameInstance->GetDIKeyPressing(DIK_A))
            GetTransform()->MoveLeft(_dTimeDelta, fSpeed);
        if (pGameInstance->GetDIKeyPressing(DIK_D))
            GetTransform()->MoveRight(_dTimeDelta, fSpeed);
        if (pGameInstance->GetDIKeyPressing(DIK_S))
            GetTransform()->MoveBack(_dTimeDelta, fSpeed);
    }

    _float2 MousePos = pGameInstance->GetMousePosition();
    _long		MouseMove = 0;

    if (MouseMove = pGameInstance->GetDIMMoveState(DIMOUSEMOVE::DIMM_X))
    {
        GetTransform()->RotateTurn(_float3(0.f, 1.f, 0.f), 90.f * MouseMove * 0.1f, _dTimeDelta);
    }

    if (MouseMove = pGameInstance->GetDIMMoveState(DIMOUSEMOVE::DIMM_Y))
        GetTransform()->RotateTurn(GetTransform()->GetRight(), 90.f * MouseMove * 0.1f, _dTimeDelta);
}

void CMainCamera::LateTickActive(const _double& _dTimeDelta)
{

}
