#ifndef _PTH_FRAMEWORK_ENGIEN_PUBLIC_UGAMEINSTANCE_H
#define _PTH_FRAMEWORK_ENGIEN_PUBLIC_UGAMEINSTANCE_H

#include "EngineDefines.h"
#include "UBase.h"

BEGIN(Engine)
class UGraphicRenderManager;
class UShaderBufferManager;
class UGraphicDevice;
class UTimerManager;
class UInputManager;

class UFontManager;
class UActorManager;
class UComponentManager;
class UResourceManager;
class USceneManager;

class UPipeLine;
class URenderTargetManager;
class UComputeManager;
class UPicking;
class UFilePathManager;
class URandomManager;

class URenderer;

class URootSignature;
class UShaderConstantBuffer;
class UGrobalConstantBuffer;
class UShader;
class UTexture;
class URenderTargetGroup;

class UActor;
class UComponent;
class UResource;
class UCamera;
class UTimer;
class UVIBuffer;
class UScene;
class ULight;
class UTableDescriptor;
class UGpuCommand;

class UGrobalConstantBuffer;
class UActorGroup;
class URenderObject;
class UCommand;
class UFont;

struct PICKINGDESC;
struct WAITCHECKACTOR;

using CLONES = UNORMAP<_wstring, SHPTR<UActorGroup>>;
using CLONEARR = ARRAY<CLONES, CLONE_MAX_SIZE>;

class ENGINE_DLL  UGameInstance : public UBase {
	DECLARE_SINGLETON(UGameInstance)
public:
	UGameInstance();
	virtual ~UGameInstance();
public:
	const _bool IsGamming() const { return m_isGamming; }
	void SetGamming(const _bool _isGamming) { this->m_isGamming = _isGamming; }
public: /* GameInstance */
	// Free 
	virtual void Free() override;
	// Ready Instance
	HRESULT ReadyInstance(const GRAPHICDESC& _stDesc, OUTPUTDATA& _stOutDesc);
	// Create Shader
	HRESULT CreateGraphicsShader(const _wstring& _wstrProtoName, const CLONETYPE _eCloneType, 
		const SHADERDESC& _stShaderDesc, const GRAPHICRENDEROBJECT_TYPE _eType = GRAPHICRENDEROBJECT_TYPE::COMMON);
	HRESULT CreateComputeShader(const _wstring& _wstrProtoName, const CLONETYPE _eCloneType,
		const SHADERDESC& _stShaderDesc);

	void OtherFrame(const _double& _dTimeDelta, const WPARAM& _wParam);
	void AwakeTick();
	void Tick(const _double& _dTimeDelta);
	void LateTick(const _double& _dTimeDelta);
	void RenderBegin();
	void RenderEnd();
	HRESULT	OnWindowResize(const _uint& _iWinSizeX, const _uint& _iWinSizeY, const GRAPHICDESC::WINMODE _eWindowMode);
	// Clear Once Type data 
	void ClearOnceTypeData();
public: /*  Graphic Render */
	const ComPtr<Dx12CommandQueue>& GetGpuCommandQueue() const;
	const ComPtr<Dx12CommandQueue>& GetComputeCommandQueue() const;
	CSHPTRREF<UGpuCommand> GetGpuCommand() const;
	SHPTR<URootSignature> GetRootSignature(const ROOT_REGISTER_TYPE _eSpace);
	SHPTR<URenderObject> GetRenderObject(const GRAPHICRENDEROBJECT_TYPE _eType);
public: /* ShaderBufferManager*/
	void	 BindGrobalBuffer(const GROBAL_CBUFFERTYPE _eGrobalCBuffer, CSHPTRREF<UCommand> _spCommand, const void* _pBuffer, const _uint _iSize);
	HRESULT GetGrobalConstantBuffer(const GROBAL_CBUFFERTYPE _eGrobalCBuffer, SHPTRREF<UGrobalConstantBuffer> _spGrobalConstantBuffer);
	HRESULT GetPreAllocatedConstantBuffer(const PREALLOCATED_CBUFFERTYPE _ePreAllocatedCBufferType, SHPTRREF<UShaderConstantBuffer> _spShaderConstantBuffer);
public: /*  GraphicDevice */
	const _bool	IsGamePaused() const;
	const _bool	IsScreenMinimized() const;
	const _bool	IsScreenMaximized() const;
	const _bool	IsScreenFullScreen() const;
	const _bool	IsMssa4xState() const;
	const _uint&	GetMssa4xQuality() const;
	const D3D12_VIEWPORT&	 GetD3DViewport() const;
	const D3D12_RECT&	GetD3DWindowSizeRect() const;
	CSHPTRREF<GRAPHICDESC> GetGraphicDesc() const;
	// Ready
	SHPTR<UTexture>		GetDefaultBackTexture();
public: /* TimerManager */
	HRESULT CreateTimer(const _wstring& _wstrName);
	SHPTR<UTimer> CreateTimerAdd(const _wstring& _wstrName);
	HRESULT RemoveTimer(const _wstring& _wstrName);

public: /* InputManager*/
	void SetCursorLock(const CURSOR_LOCKMODE& _eLockMode);
	const CURSOR_LOCKMODE& GetCursorMode() const;

	_bool GetDIKeyDown(const _ubyte& _eKeyID);
	_bool GetDIKeyUp(const _ubyte& _eKeyID);
	_bool GetDIKeyPressing(const _ubyte& _eKeyID);
	_bool GetDIMBtnDown(const DIMOUSEBUTTON& _eMouseBtn);
	_bool GetDIMBtnUp(const DIMOUSEBUTTON& _eMouseBtn);
	_bool GetDIMBtnPressing(const DIMOUSEBUTTON& _eMouseBtn);
	_long GetDIMMoveState(const DIMOUSEMOVE& _eMouseMove);
	// Get Mouse Position
	_float2 GetMousePosition();
	// Control KeyMethods
	void AddKeyMethod(const _ubyte& _bFirstKeyID, const _wstring& _wstrSecondsKeyName, const UDelegate<void, const _double&>& _keyMethod);
	void DeleteKeyMethod(const _ubyte& _bFirstKeyID, const _wstring& _wstrSecondsKeyName);
public: /* FontManger */
	HRESULT FontCreate(const _wstring& _strFontName, const _wstring& _strPath);
	SHPTR<UFont> FontCreateAdd(const _wstring& _strFontName, const _wstring& _strPath);
	SHPTR<UFont> AddFont(const _wstring& _strFontName);
	void ReleaseOriginFont(const _wstring& _strFontName);
	void RemoveFont(CSHPTRREF<UFont> _pFont);
public: /* ActorManager */
	const  CLONEARR& GetClonesArr() const;
	HRESULT AddPrototype(const _wstring& _wstrName, CSHPTRREF<UActor> _spActor);
	void  CloneActor(const _wstring& _wstrProto, const VOIDDATAS& _stDatas = VOIDDATAS{});
	SHPTR<UActor> CloneActorAdd(const _wstring& _wstrProto, const VOIDDATAS& _stDatas = VOIDDATAS{});
	SHPTR<UActor> CloneActorAddAndNotInLayer(const _wstring& _wstrProto, const VOIDDATAS& _stDatas = VOIDDATAS{});
	void RemoveActor(CSHPTRREF<UActor> _spActor);
public: /*  ComponentManager */
	HRESULT AddPrototype(const _wstring& _wstrPrototypes, CSHPTRREF<UComponent> _spComponent);
	SHPTR<UComponent> CloneComp(const _wstring& _wstrPrototypes, const VOIDDATAS& _stDatas = VOIDDATAS{});

public: /* ResourceManager*/
	HRESULT AddPrototype(const _wstring& _wstrPrototype, const CLONETYPE& _eCloneType, CSHPTRREF<UResource> _spResource);
	SHPTR<UResource> CloneResource(const _wstring& _wstrPrototype, const VOIDDATAS& _stDatas = VOIDDATAS{});
	void		FindResourceProtoTag(CSHPTRREF<UResource> _spResource, _wstring& _wstrResourceTag);

	void AddPipeLineState(const _wstring& _wstrName, const ComPtr<Dx12PipelineState>& _cpPipeLineState);
	void SettingPipeLineState(const _wstring& _wstrName, CSHPTRREF<UCommand> _spCommand);
public: /* SceneManager*/
	void OutLightControlInfo(LIGHTPARAM& _stLightControl);
	void ChangeLightCamIndex(const CAMID& _iID);

	CSHPTRREF<UScene> GetCurScene() const;
	void RegisterScene(CSHPTRREF<UScene> _spScene);
	// Add
	HRESULT AddLight(const LIGHTINFO& _stInfo);
	// Out Light
	void OutLight(const LIGHTTYPE& _eLightType, const _uint& _iIndex, SHPTR<ULight>& _spLight);
	// Active
	HRESULT ActiveLIght(const LIGHTTYPE& _eLightType, const _uint& _iIndex, const _bool& _isActive);
	// Delete
	HRESULT DeleteLight(const LIGHTTYPE& _eLightType, const _uint& _iIndex);
	// Clear
	HRESULT ClearLight();


public: /*  RenderTargetManager */
	// OMSetRenderTarget 
	void	 OmSetRenderTargets(CSHPTRREF<URenderObject> _spRenderObject, const RTGROUPID& _eGroupID, const _uint _index, const _uint _iOffset);
	void	 OmSetRenderTargets(CSHPTRREF<URenderObject> _spRenderObject, const RTGROUPID& _eGroupID);
	// ClearRenderTargetView
	void	 ClearRenderTargetView(CSHPTRREF<URenderObject> _spRenderObject, const RTGROUPID& _eGroupID, const _uint _index);
	void	 ClearRenderTargetView(CSHPTRREF<URenderObject> _spRenderObject, const RTGROUPID& _eGroupID);
	void	 WaitTargetToResource(CSHPTRREF<URenderObject> _spRenderObject, const RTGROUPID& _eGroupID);
	void	 WaitResourceToTarget(CSHPTRREF<URenderObject> _spRenderObject, const RTGROUPID& _eGroupID);


	// OMSetRenderTarget 
	void					OmSetRenderTargets(CSHPTRREF<UCommand> _spCommand, const RTGROUPID& _eGroupID, const _uint _index, const _uint _iOffset);
	void					OmSetRenderTargets(CSHPTRREF<UCommand> _spCommand, const RTGROUPID& _eGroupID);
	// ClearRenderTargetView
	void					ClearRenderTargetView(CSHPTRREF<UCommand> _spCommand, const RTGROUPID& _eGroupID, const _uint _index);
	void					ClearRenderTargetView(CSHPTRREF<UCommand> _spCommand, const RTGROUPID& _eGroupID);
	// Waiting
	void					WaitTargetToResource(CSHPTRREF<UCommand> _spCommand, const RTGROUPID& _eGroupID);
	void					WaitResourceToTarget(CSHPTRREF<UCommand> _spCommand, const RTGROUPID& _eGroupID);

	// Get RT Group 
	SHPTR<URenderTargetGroup>	 FindRenderTargetGroup(const RTGROUPID& _eGroupID);
	SHPTR<UTexture> FindRenderTargetTexture(const RTGROUPID _eGroupID, const RTOBJID _eObjID);

public: /* PipeLine */
	void RegisterCameraInPipeline(CSHPTRREF<UCamera> _spCamera, CAMID& _iCamID,
		const VIEWPROJMATRIX& _stViewProjMatrix, const CAMERATYPE _eType = CAMERATYPE::OTEHR);

	_bool IsFrustomContains(const _float3& _vPos, const _float _fRadius, const CAMID& _iCamID = MAIN_CAMERA_ID);

	void CancelCameraInPipeLine(const CAMID& _iCamID);
	void ChangeMainCamera(const CAMID& _iCamID);

	const _float4x4 GetMainCamViewMatrix() const;
	const _float4x4& GetMainCamProjMatrix() const;
	const _float3& GetMainCamPosition();

	const _float4x4 GetCamViewMatrix(const CAMID& _iID) const;
	const _float4x4& GetCamProjMatrix(const CAMID& _iID) const;
	const _float3& GetCameraPosition(const CAMID& _iID);
	const _float GetCamFar(const CAMID& _iID);

public: /* Picking */
	void AddPickingObject(CSHPTRREF<UActor> _spActor, CSHPTRREF<UVIBuffer> _spVIBuffer);
	SHPTR<UActor> GetPickingActor();
	const PICKINGDESC& GetPickDesc();
	_bool PickingMesh(CSHPTRREF<UActor> _spActor, CSHPTRREF<UVIBuffer> _spVIBuffer,
		_float* _pDist, _float3* _pOut);

public: /* FilePath Manager*/
	SHPTR<FILEGROUP> FindFolder(const PATHS& _vecFolderList);
	SHPTR<FILEGROUP>	FindFolder(const _wstring& _wstrFindName, const _wstring& _wstrParentsFolderName = L"");

	HRESULT LoadFirstFilder(const _wstring& _wstrFilePath);

public:  /* randomManager */
	_int GetRandomNumber(const _int _iMaxNum);

private: /* Ready Datas */
	HRESULT ReadyResource(const OUTPUTDATA& _stData);
	HRESULT ReadyComp(const OUTPUTDATA& _stData);
	HRESULT ReadyActor(const OUTPUTDATA& _stData);
	HRESULT ReadyRenderTarget(const OUTPUTDATA& _stData);
#ifdef _USE_IMGUI 
public:
	void SetCurrentContext(ImGuiContext* _pContext);
#endif
private:
	//Game Start
	_bool															m_isGamming;

	SHPTR<UGraphicRenderManager>	m_spGraphicRenderManager;
	SHPTR<UShaderBufferManager>		m_spShaderBufferManager;
	SHPTR<UGraphicDevice>						m_spGraphicDevice;
	SHPTR<UTimerManager>						m_spTimerManager;
	SHPTR<UInputManager>						m_spInputManager;

	SHPTR< UFontManager>						m_spFontMananger;
	SHPTR<UActorManager>						m_spActorManager;
	SHPTR<UComponentManager>			m_spComponentManager;
	SHPTR<UResourceManager>				m_spResourceManager;
	SHPTR<USceneManager>						m_spSceneManager;

	SHPTR<URenderTargetManager>		m_spRenderTargetManager;
	SHPTR<UComputeManager>				m_spComputeManager;

	SHPTR<UPipeLine>									m_spPipeLine;
	SHPTR<UPicking>									m_spPicking;
	SHPTR<UFilePathManager>					m_spFilePathManager;
	SHPTR<URandomManager>					m_spRandomManager;

	SHPTR<URenderer>								m_spRenderer;
	SHPTR<URenderObject>						m_spGraphicRenderObject;
};

END

#endif // _PTH_FRAMEWORK_ENGIEN_PUBLIC_UGAMEINSTANCE_H