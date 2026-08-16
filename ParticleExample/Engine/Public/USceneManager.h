#ifndef _PTH_FRAMEWORK_ENGIEN_PUBLIC_USCENEMANAGER_H
#define _PTH_FRAMEWORK_ENGIEN_PUBLIC_USCENEMANAGER_H

#include "UBase.h"

BEGIN(Engine)
class UDevice;
class UGpuCommand;
class UScene;
class ULight;
class URenderObject;
class URenderer;
class UShaderConstantBuffer;
class URenderCommand;
class UGrobalConstantBuffer;
class UPipeLine;
class UGameInstance;
class URenderTargetManager;


class USceneManager : public UBase {
public:
	USceneManager();
	NO_COPY(USceneManager)
	DESTRUCTOR(USceneManager)
public:
	CSHPTRREF<UScene> GetScene() const { return m_spCurScene; }
public:
	virtual void Free() override;
	HRESULT ReadySceneManager(UGameInstance* _pGameInstance, CSHPTRREF<URenderObject> _spRenderObject);

	void RegisterScene(CSHPTRREF<UScene> _spScene);
	void Tick(const _double& _dTimeDelta);
	void LateTick(const _double& _dTimeDelta);
	void CollisionTick(const _double& _dTimeDelta);

	void OutLightControlInfo(LIGHTPARAM& _stLightControl);
	void ChangeLightCamIndex(const CAMID& _iID);
	// Render Light
	void RenderLight(CSHPTRREF<UPipeLine> _spPipeLine, CSHPTRREF<URenderTargetManager> _spRenderTargetManager,
		const TRANSFORMPARAM& _stDefferendParam);

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
private:
	const _uint											m_c_iLightParamSize;
	_uint														m_iLightCamIndex;

	SHPTR<URenderObject>					m_spLightRenderObject;
	SHPTR<UScene>									m_spCurScene;

	LIGHTPARAM											m_stLightParam; 
	SHPTR< UGrobalConstantBuffer>	m_spLightGrobalBuffer;
	// Render Command
	SHPTR<URenderCommand>				m_spRenderCommand;
};

END

#endif // _PTH_FRAMEWORK_ENGIEN_PUBLIC_USCENEMANAGER_H