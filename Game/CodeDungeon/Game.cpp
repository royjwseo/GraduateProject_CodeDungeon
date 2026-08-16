#include "stdafx.h"
#include "Game.h"
#include "Engine.h"

void Game::Init(HINSTANCE hInstance, HWND hWnd)
{
	gGameFrameWork->OnCreate(hInstance,hWnd);
	//2024-01-18 이성현 프로젝트별 타이머 분리.
	gGameFrameWork->GetTimer().Reset();
	gGameFrameWork->BuildObjects();
}

void Game::Update()
{
	//2024-01-07 이성현
	//Tool에서의 렌더링 과정을 RenderBegin과 RenderEnd 사이에 적용시키기 위한
	//Render와 RenderBegin, RenderEnd의 분리.
	//2024-01-18 이성현 프로젝트별 타이머 분리.
	gGameFrameWork->RenderBegin();
	gGameFrameWork->GetTimer().Tick(0.0f);
	gGameFrameWork->Render();
	gGameFrameWork->RenderEnd();
	gGameFrameWork->GetTimer().GetFrameRate(gGameFrameWork->m_pszFrameRate + 12, 37);
	::SetWindowText(RTVDSVDESCRIPTORHEAP->m_hWnd, gGameFrameWork->m_pszFrameRate);
}

void Game::Destroy()
{
	gGameFrameWork->OnDestroy();

}

LRESULT CALLBACK Game::OnProcessingWindowMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam) {
	return gGameFrameWork->OnProcessingWindowMessage(hWnd, nMessageID, wParam, lParam);
}

void Game::OnProcessingKeyboardMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam) {
	gGameFrameWork->OnProcessingKeyboardMessage(hWnd, nMessageID, wParam, lParam);
}

void Game::OnProcessingMouseMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam) {
	gGameFrameWork->OnProcessingMouseMessage(hWnd, nMessageID, wParam, lParam);
}

