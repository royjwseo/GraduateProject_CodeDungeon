#ifndef _PTH_FRAMEWORK_ENGIEN_PUBLIC_EMGINE_ULIGHT_H
#define _PTH_FRAMEWORK_ENGIEN_PUBLIC_EMGINE_ULIGHT_H

#include "UObject.h"

BEGIN(Engine)
class UShaderConstantBuffer;
class UShader;
class UVIBufferRect;
class URenderObject;

class ENGINE_DLL ULight : public UObject {
public:
	ULight(CSHPTRREF<UDevice> _spDevice, 	const LIGHTINFO& _stLightInfo, const LIGHTCONTROL& _stLightControl);
	NO_COPY(ULight)
	DESTRUCTOR(ULight)
public:
	// Get 
	const LIGHTINFO& GetLightInfo() const { return m_stLightParam.tLightInfo; }
	const LIGHTCONTROL& GetLightControl() const { return m_stLightParam.tLightControl; }
	const _bool IsActive() const { return m_isActive; }
	const LIGHTTYPE GetLightType() const { return m_stLightParam.tLightInfo.eLightType; }
	// set 
	void SetLightType(const LIGHTTYPE _eLightType) { this->m_stLightParam.tLightInfo.eLightType = _eLightType; }
	void SetActive(const _bool& _bIsActive) { this->m_isActive = _bIsActive; }
	// Set Diffuse Ambient Specular
	void SetDiffuse(const _float3& _vDiffuse) { this->m_stLightParam.tLightInfo.vDiffuse = _vDiffuse; }
	void SetAmbient(const _float3& _vAmbient) { this->m_stLightParam.tLightInfo.vAmbient = _vAmbient; }
	void SetSpecular(const _float3& _vSpecular) { this->m_stLightParam.tLightInfo.vSpecular = _vSpecular; }
	// Direction Light Range Angle
	void SetDirection(const _float3& _v3Direction) { this->m_stLightParam.tLightInfo.vDirection = _v3Direction; }
	void SetLightPos(const _float3& _v3LightPos) { this->m_stLightParam.tLightInfo.vPosition = _v3LightPos; }
	void SetRange(const _float& _fRange) { this->m_stLightParam.tLightInfo.fRange = _fRange; }
	void SetAngle(const _float& _fAngle) { this->m_stLightParam.tLightInfo.fAngle = _fAngle; }
	void SetLightPower(const _float& _fPower) { this->m_stLightParam.tLightInfo.fLightPower = _fPower; }
	void SetSpecularPowValue(const _float _fPowValue) { this->m_stLightParam.tLightInfo.fSpecularPowValue = _fPowValue; }
public:
	// Free
	virtual void Free() override;
	virtual HRESULT NativeConstruct() override;

	// Render
	void Render(CSHPTRREF<URenderObject> _spRenderObject, CSHPTRREF<UVIBufferRect> _spVIBufferRect, CSHPTRREF<UShader> _spShader);
	void OutLightInfo(LIGHTINFO& _stLightInfo);
	void OutLightControl(LIGHTCONTROL& _stLightControl);
private:
	static constexpr _uint								LIGHTCONTROL_SIZE{sizeof(LIGHTCONTROLPARAM)};

	SHPTR<UShaderConstantBuffer>		m_spLightConstantBuffer;
	LIGHTCONTROLPARAM							m_stLightParam;
	_bool															m_isActive{ false };
};

END

#endif // _PTH_FRAMEWORK_ENGIEN_PUBLIC_EMGINE_ULIGHT_H