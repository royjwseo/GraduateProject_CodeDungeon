#pragma once
#include "ToolDefines.h"
#include "ToolManager.h"
#include "ToolScene.h"

class InteractionManager : public ToolManager
{
public:
	InteractionManager();
	~InteractionManager();
	void MovePlayerWithKeyBoard(const shared_ptr<CPlayer>& pPlayer);
	void PlayerZoomMoveMent(const shared_ptr<CPlayer>& pPlayer);
	void PlayerRotate(const shared_ptr<CPlayer>& pPlayer);

	int GetScrollDelta() { return m_scrollDelta; }
	int SetScrollDelta(int d) { return m_scrollDelta += d; }
	void SetRightMouseButtonDown(bool state) { m_isRightMouseButtonDown = state; }
	bool IsRightMouseButtonDown() const { return m_isRightMouseButtonDown; }
	void SaveOldCursorPos();

private:
	int							m_scrollDelta = 0;

	bool						m_isRightMouseButtonDown = false;
	POINT						m_ptOldCursorPos;
	bool						m_bClipCursor = true;
};

