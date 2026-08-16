#pragma once
#include "ToolDefines.h"
#include "ToolManager.h"
#include "ToolScene.h"
#include "PickingManager.h"

class ImguiManager : public ToolManager
{
public:
	ImguiManager();
	~ImguiManager();
	void DisplayWindow(const shared_ptr<CPlayer>& pPlayer, const shared_ptr<CScene>& pScene, const shared_ptr<CCamera>& pCamera);
private:
	shared_ptr<PickingManager> m_pPickingManager;
	XMFLOAT3 m_xmf3PickedWorldPos;

	bool m_bPickingEnabled = false;
};

