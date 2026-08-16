#pragma once
#include "EnginePch.h"
#include "Scene.h"
#include "DeviceAndFactory.h"
#include "SwapChainAndRtvDsvHeap.h"
#include "CommandQueue.h"
#include "RootSignature.h"
class Engine
{
	
public:
	bool OnCreate(HINSTANCE hInstance, HWND hMainWnd);
	void OnDestroy();
public:
	shared_ptr<DeviceAndFactory> GetDeviceAndFactory() { return m_spDevice; }
	shared_ptr<SwapChainAndRtvDsvHeap> GetSwapChainAndRtvDsvHeap() { return m_spSwapChainAndRtvDsvHeap; }
	shared_ptr<CommandQueue> GetCommandQueue() { return m_spCommandQueue; }
	shared_ptr<RootSignature> GetRootSignature() { return m_spRootSignature; }
public:
	void RenderBegin();
	void RenderEnd();
	void Render();
public:
	void ProcessInput();   
	void AnimateObjects(); 
	void BuildObjects();
	void ReleaseObjects();
	CGameTimer GetTimer(){ return m_GameTimer; }
	_TCHAR m_pszFrameRate[50];
private:
	CGameTimer m_GameTimer;
	shared_ptr<CCamera> m_pCamera = nullptr;

private:
	shared_ptr<CScene> m_pScene;
	ComPtr<ID3D12PipelineState> m_pd3dPipelineState;
	

private:
	shared_ptr<DeviceAndFactory> m_spDevice= make_shared<DeviceAndFactory>();
	shared_ptr<SwapChainAndRtvDsvHeap> m_spSwapChainAndRtvDsvHeap= make_shared<SwapChainAndRtvDsvHeap>();
	shared_ptr<CommandQueue> m_spCommandQueue = make_shared<CommandQueue>();
	shared_ptr<RootSignature> m_spRootSignature = make_shared<RootSignature>();
public:
	//INPUT 관련
	void OnProcessingMouseMessage(HWND hwnd, UINT nMessageID, WPARAM wParam, LPARAM lParam);

	void OnProcessingKeyboardMessage(HWND hwnd, UINT nMessageID, WPARAM wParam, LPARAM lParam);
	
	LRESULT CALLBACK OnProcessingWindowMessage(HWND hwnd, UINT nMessageID, WPARAM wParam, LPARAM lParam);
};

