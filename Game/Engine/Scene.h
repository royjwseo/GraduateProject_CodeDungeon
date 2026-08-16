#pragma once
#include "Timer.h"
#include "Shader.h"
#include "Player.h"


class CbvSrvUavDescriptorHeap;
class CTexture;

class CScene
{
public:
	CScene();
	~CScene();
	//씬에서 마우스와 키보드 메시지를 처리한다. 
	bool OnProcessingMouseMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam);
	bool OnProcessingKeyboardMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam);
	void BuildObjects(const ComPtr<ID3D12Device>& _Device, const ComPtr<ID3D12GraphicsCommandList>& _CommandList,const ComPtr<ID3D12RootSignature>& _RootSignature);
	void ReleaseObjects();
	bool ProcessInput(UCHAR* pKeysBuffer);
	void AnimateObjects(float fTimeElapsed);
	void Render(const ComPtr<ID3D12GraphicsCommandList>& _CommandList, const shared_ptr<CCamera>& pCamera);
	void ReleaseUploadBuffers();
	virtual void UpdateShaderVariables(const ComPtr<ID3D12GraphicsCommandList>& _CommandList) {}
	virtual void ReleaseShaderVariables() {}
	//그래픽 루트 시그너쳐를 생성한다. 
	vector<shared_ptr<CGameObject>> GetObjects() { return m_ppObjects;}

protected:
	//씬은 셰이더들의 집합이다. 셰이더들은 게임 객체들의 집합이다.
	vector<shared_ptr<CGameObject>> m_ppObjects;
	shared_ptr<CPlayer> m_pPlayer;
public:
	shared_ptr<CPlayer> GetPlayer() { return m_pPlayer;}
public:
	static unique_ptr<CbvSrvUavDescriptorHeap> m_pDescriptorHeap;


	static void CreateCbvSrvDescriptorHeaps(int nConstantBufferViews, int nShaderResourceViews);
	static void CreateConstantBufferViews( int nConstantBufferViews, const ComPtr<ID3D12Resource>& _ConstantBuffers, UINT nStride);
	static D3D12_GPU_DESCRIPTOR_HANDLE CreateConstantBufferView(const ComPtr<ID3D12Resource>& _ConstantBuffer, UINT nStride);
	static D3D12_GPU_DESCRIPTOR_HANDLE CreateConstantBufferView(D3D12_GPU_VIRTUAL_ADDRESS d3dGpuVirtualAddress, UINT nStride);
	static void CreateShaderResourceViews( CTexture* pTexture, UINT nDescriptorHeapIndex, UINT nRootParameterStartIndex);
	static void CreateShaderResourceView(CTexture* pTexture, int nIndex, UINT nRootParameterStartIndex);
	static void CreateShaderResourceView(CTexture* pTexture, int nIndex);
};