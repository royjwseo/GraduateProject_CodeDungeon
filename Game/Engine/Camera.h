#pragma once

class CPlayer;

struct VS_CB_CAMERA_INFO {
	XMFLOAT4X4						m_xmf4x4View;
	XMFLOAT4X4						m_xmf4x4Projection;
};

class CCamera
{
protected:
	//카메라 변환 행렬
	XMFLOAT4X4						m_xmf4x4View;
	//투영 변환 행렬
	XMFLOAT4X4						m_xmf4x4Projection;

	//뷰포트와 씨저 사각형
	D3D12_VIEWPORT					m_d3dViewport;
	D3D12_RECT						m_d3dScissorRect;

	XMFLOAT3						m_xmf3Position;
	XMFLOAT3						m_xmf3Right;
	XMFLOAT3						m_xmf3Up;
	XMFLOAT3						m_xmf3Look;
	XMFLOAT3						m_xmf3LookAtWorld;
	XMFLOAT3						m_xmf3Offset;

	CameraMode						m_enCameraMode;
	float           				m_fTimeLag;
	shared_ptr<CPlayer>				m_pPlayer;
protected:
	ComPtr<ID3D12Resource>			m_pd3dcbCamera;
	VS_CB_CAMERA_INFO*				m_pcbMappedCamera = NULL;
public:
	CCamera();
	CCamera(shared_ptr<CCamera> pCamera);
	virtual ~CCamera();
	XMFLOAT4X4			GetViewMatrix() { return m_xmf4x4View; }
	XMFLOAT4X4			GetProjectionMatrix() { return m_xmf4x4Projection; }
	D3D12_VIEWPORT		GetViewPort() { return m_d3dViewport; }
	D3D12_RECT			GetScissorRect() { return m_d3dScissorRect; }
	XMFLOAT3			GetCameraPosition() { return m_xmf3Position;  }
	XMFLOAT3			GetCameraRight() { return m_xmf3Right;}
	XMFLOAT3			GetCameraUp() { return m_xmf3Up;}
	XMFLOAT3			GetCameraLook() { return m_xmf3Look;}
	XMFLOAT3			GetCameraLookWorld() { return(m_xmf3LookAtWorld); }
	XMFLOAT3			GetOffset() { return(m_xmf3Offset); }
	CameraMode			GetCameraMode() { return m_enCameraMode;}
	float				GetTimeLag() { return(m_fTimeLag); }
	shared_ptr<CPlayer> GetCameraPlayer() { return m_pPlayer;}


	void SetCameraPosition(const XMFLOAT3& position) { m_xmf3Position = position; }
	void SetCameraRight(const XMFLOAT3& right) { m_xmf3Right = right; }
	void SetCameraUp(const XMFLOAT3& up) { m_xmf3Up = up; }
	void SetCameraLook(const XMFLOAT3& look) { m_xmf3Look = look; }
	void SetCameraMode(CameraMode mode) { m_enCameraMode = mode; }
	void SetCameraLookWorld(XMFLOAT3 xmf3LookAtWorld) { m_xmf3LookAtWorld = xmf3LookAtWorld; }
	void SetOffset(XMFLOAT3 xmf3Offset) { m_xmf3Offset = xmf3Offset; m_xmf3Position.x += xmf3Offset.x; m_xmf3Position.y += xmf3Offset.y; m_xmf3Position.z += xmf3Offset.z; }
	void SetTimeLag(float fTimeLag) { m_fTimeLag = fTimeLag; }
	void SetCameraPlayer(const shared_ptr<CPlayer> player) { m_pPlayer = player; }

	virtual void CreateShaderVariables(const ComPtr<ID3D12Device>& _Device, const ComPtr<ID3D12GraphicsCommandList>& _CommandList);
	virtual void ReleaseShaderVariables();
	virtual void UpdateShaderVariables(const ComPtr<ID3D12GraphicsCommandList>& _CommandList);

	void GenerateViewMatrix();
	void GenerateViewMatrix(XMFLOAT3 xmf3Position, XMFLOAT3 xmf3Look, XMFLOAT3 xmf3Up);
	void RegenerateViewMatrix();

	void GenerateProjectionMatrix(float fNearPlaneDistance, float fFarPlaneDistance, float fAspectRatio, float fFOVAngle);


	void SetViewport(int xTopLeft, int yTopLeft, int nWidth, int nHeight, float fMinZ = 0.0f, float fMaxZ = 1.0f);
	void SetScissorRect(LONG xLeft, LONG yTop, LONG xRight, LONG yBottom);

	virtual void SetViewportsAndScissorRects(const ComPtr<ID3D12GraphicsCommandList>& _CommandList);
	
	virtual void Move(const XMFLOAT3& xmf3Shift) { m_xmf3Position.x += xmf3Shift.x; m_xmf3Position.y += xmf3Shift.y; m_xmf3Position.z += xmf3Shift.z; }
	virtual void Rotate(float fPitch = 0.0f, float fYaw = 0.0f, float fRoll = 0.0f) { }
	virtual void Update(XMFLOAT3& xmf3LookAt, float fTimeElapsed) { }
	virtual void SetLookAt(XMFLOAT3& xmf3LookAt) { }
};

class CDebugCamera : public CCamera
{
public:
	CDebugCamera(shared_ptr<CCamera> pCamera);
	virtual ~CDebugCamera() { }

	virtual void Rotate(float fPitch, float fYaw, float fRoll);
	virtual void Update(XMFLOAT3& xmf3LookAt, float fTimeElapsed);
	virtual void SetLookAt(XMFLOAT3& xmf3LookAt);
};