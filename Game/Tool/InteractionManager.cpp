#include "InteractionManager.h"
#include "Engine.h"

InteractionManager::InteractionManager()
{
}

InteractionManager::~InteractionManager()
{
}

void InteractionManager::MovePlayerWithKeyBoard(const shared_ptr<CPlayer>& pPlayer)
{
    static UCHAR pKeysBuffer[256];
    if (GetKeyboardState(pKeysBuffer))
    {
        DWORD Playerdirection = 0;
        if (pKeysBuffer['W'] & 0xF0)
            Playerdirection |= DIR_FORWARD;
        if (pKeysBuffer['S'] & 0xF0)
            Playerdirection |= DIR_BACKWARD;
        if (pKeysBuffer['A'] & 0xF0)
            Playerdirection |= DIR_LEFT;
        if (pKeysBuffer['D'] & 0xF0)
            Playerdirection |= DIR_RIGHT;
        if (pKeysBuffer[VK_SPACE] & 0xF0)
            Playerdirection |= DIR_UP;
        if (pKeysBuffer[VK_SHIFT] & 0xF0)
            Playerdirection |= DIR_DOWN;

        if (Playerdirection != 0)
            pPlayer->Move(Playerdirection, MOVEMENT_SPEED, false);
    }
}

void InteractionManager::PlayerZoomMoveMent(const shared_ptr<CPlayer>& pPlayer)
{
    static int prevScrollDelta = 0;
    int currentScrollDelta = GetScrollDelta();
    if (currentScrollDelta != 0)
    {
        int scrollDelta = currentScrollDelta - prevScrollDelta;
        pPlayer->Zoom(scrollDelta, ZOOM_SPEED);
        prevScrollDelta = currentScrollDelta;
    }
}

void InteractionManager::PlayerRotate(const shared_ptr<CPlayer>& pPlayer)
{
    if (m_isRightMouseButtonDown) {
        float cxDelta = 0.0f, cyDelta = 0.0f;
        POINT ptCursorPos;
        GetCursorPos(&ptCursorPos);

        if (GetCapture() == RTVDSVDESCRIPTORHEAP->m_hWnd) {
            cyDelta = (float)(ptCursorPos.y - m_ptOldCursorPos.y) / 3.0f;
            cxDelta = (float)(ptCursorPos.x - m_ptOldCursorPos.x) / 3.0f;
        }

        SetCursorPos(m_ptOldCursorPos.x, m_ptOldCursorPos.y);

        if (cxDelta || cyDelta)
        {
            pPlayer->Rotate(cyDelta, cxDelta, 0.0f);
        }
    }
}


void InteractionManager::SaveOldCursorPos()
{
    GetCursorPos(&m_ptOldCursorPos);    
}
