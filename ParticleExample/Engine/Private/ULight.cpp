#include "EngineDefines.h"
#include "ULight.h"
#include "UGameInstance.h"
#include "UShaderConstantBuffer.h"
#include "UShader.h"
#include "UVIBufferRect.h"
#include "URenderTargetManager.h"
#include "URenderObject.h"

ULight::ULight(CSHPTRREF<UDevice> _spDevice, const LIGHTINFO& _stLightInfo, const LIGHTCONTROL& _stLightControl)
	: UObject(_spDevice),
	m_spLightConstantBuffer{nullptr}, 
	m_stLightParam{ _stLightInfo, _stLightControl }
{
}

void ULight::Free()
{
} 

HRESULT ULight::NativeConstruct()
{
	if (m_stLightParam.tLightInfo.eLightType == LIGHTTYPE::TYPE_POINT)
		m_stLightParam.tLightInfo.vDirection = _float4::Zero;

	m_isActive = true;

	m_spLightConstantBuffer = CreateNative<UShaderConstantBuffer>(GetDevice(), CBV_REGISTER::LIGHTCONTROL, LIGHTCONTROL_SIZE);
	return S_OK;
}

void ULight::Render(CSHPTRREF<URenderObject> _spRenderObject, CSHPTRREF<UVIBufferRect> _spVIBufferRect, CSHPTRREF<UShader> _spShader)
{
	RETURN_CHECK(nullptr == _spShader || nullptr == _spVIBufferRect, ;);

	_spShader->BindCBVBuffer(m_spLightConstantBuffer,	&m_stLightParam, LIGHTCONTROL_SIZE);

	_spVIBufferRect->Render( _spShader, _spRenderObject->GetCommand());
}

void ULight::OutLightInfo(LIGHTINFO& _stLightInfo)
{
	if (true == m_isActive) {
		_stLightInfo = m_stLightParam.tLightInfo;
	}
}

void ULight::OutLightControl(LIGHTCONTROL& _stLightControl)
{
}
