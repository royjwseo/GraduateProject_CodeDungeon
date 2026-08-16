#pragma once
#include "ToolDefines.h"
#include "ToolScene.h"
#include "ImguiManager.h"
#include "InteractionManager.h"
#include "Player.h"

class Tool
{
public:
	void Init(HINSTANCE hInstance, HWND hWnd);
	void InitImgui(HWND hWnd);
	void BuildToolScene();
	void Update();
	void Destroy();
	void Render();
	void ImguiRender();
	LRESULT CALLBACK OnProcessingWindowMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam);
	void OnProcessingMouseMessage(HWND hwnd, UINT nMessageID, WPARAM wParam, LPARAM lParam);
	void OnProcessingKeyboardMessage(HWND hwnd, UINT nMessageID, WPARAM wParam, LPARAM lParam);
	
	void ProcessInput();
	void AnimateObjects();

	shared_ptr<InteractionManager> GetInteractionManageer() { return m_pInteractionManager; }

private:
	int							m_scrollDelta = 0;

	bool						m_isRightMouseButtonDown;
	POINT						m_ptOldCursorPos;
	bool						m_bClipCursor = true;

	shared_ptr<CPlayer>				m_pToolPlayer;
	shared_ptr<ImguiManager>		m_pImguiManager;
	shared_ptr<InteractionManager>	m_pInteractionManager;
	shared_ptr<CCamera>				m_pToolCamera;
	shared_ptr<ToolScene>			m_pToolScene;
	ComPtr<ID3D12DescriptorHeap>	m_pdxgiSRVDescriptorHeapForImgui;

};