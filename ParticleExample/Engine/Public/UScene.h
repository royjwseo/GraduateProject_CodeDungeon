#ifndef _PTH_FRAMEWORK_ENGIEN_PUBLIC_USCENE_H
#define _PTH_FRAMEWORK_ENGIEN_PUBLIC_USCENE_H

#include "UObject.h"

BEGIN(Engine)
class ULight;
class ULoader;
class UShader;
class UVIBufferRect;
class UShaderConstantBuffer;
class URenderObject;

using LIGHTCONTAINER = VECTOR<SHPTR<ULight>>;
using LIGHTGROUP = UNORMAP<LIGHTTYPE, LIGHTCONTAINER>;
using LIGHTSHADERS = UNORMAP<LIGHTTYPE, SHPTR<UShader>>;

class ENGINE_DLL UScene abstract : public UObject {
public:
	UScene(CSHPTRREF<UDevice> _spDevice,  const _ushort _iSceneID);
	NO_COPY(UScene)
		DESTRUCTOR(UScene)
public:
	CSHPTRREF<ULoader> GetLoader() const { return m_spLoader; }

	const _ushort GetSceneID() const { return m_sSceneID; }
public:
	virtual void Free() override PURE;
	virtual HRESULT NativeConstruct() override;
	virtual HRESULT LoadSceneData() PURE;

	virtual void Tick(const _double& _dTimeDelta) PURE;
	virtual void LateTick(const _double& _dTimeDelta)PURE;
	virtual void CollisionTick(const _double& _dTimeDelta);
	// Render 
	void RenderLights(CSHPTRREF<URenderObject> _spRenderObject);

	HRESULT AddLight(const LIGHTINFO& _stLightInfo, const LIGHTCONTROL& _stLightControl = LIGHTCONTROL{});
	void OutLight(const LIGHTTYPE& _eLightType, const _uint _iIndex, SHPTR<ULight>& _spLight);
	// Active
	HRESULT ActiveLIght(const LIGHTTYPE& _eLightType, const _uint& _iIndex, const _bool& _isActive);
	// Delete
	HRESULT DeleteLight(const LIGHTTYPE& _eLightType, const _uint& _iIndex);
	// Clear
	HRESULT ClearLight();
protected:
	HRESULT StartLoadingResource(const VECTOR< UDelegate<void, _bool&>>& _vecFunctions, const _uint& _iMethodCount);
private:
	SHPTR<ULoader>			m_spLoader;

	LIGHTSHADERS				m_LightShaders;
	SHPTR<UVIBufferRect>	m_spVIBufferRect;

	_ushort								m_sSceneID;
	LIGHTGROUP						m_LightGroup;
};


END

#endif // _PTH_FRAMEWORK_ENGIEN_PUBLIC_USCENE_H
