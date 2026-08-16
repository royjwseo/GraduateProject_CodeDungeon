#include "EngineDefines.h"
#include "UScene.h"
#include "ULight.h" 
#include "ULoader.h"
#include "UShader.h"
#include "UVIBufferRect.h"
#include "UGameInstance.h"
#include "UTransform.h"
#include "URenderTargetGroup.h"
#include "URenderObject.h"
#include "UCommand.h"

UScene::UScene(CSHPTRREF<UDevice> _spDevice, const _ushort _iSceneID)
    : UObject(_spDevice),
    m_spLoader{ nullptr },
    m_sSceneID{ _iSceneID },
    m_LightGroup{}
{
}

void UScene::Free()
{
}

HRESULT UScene::NativeConstruct()
{
    if (FAILED(__super::NativeConstruct()))
        return E_FAIL;

    SHPTR<UGameInstance> spGameInstance = GET_INSTANCE(UGameInstance);
    // Direction 
    m_LightShaders.insert(std::make_pair(LIGHTTYPE::TYPE_DIRECTIONAL, static_pointer_cast<UShader>(
        spGameInstance->CloneResource(PROTO_RES_LIGHTDIRECTIONSHADER)
    )));
    SHPTR<UShader> spShader = static_pointer_cast<UShader>( spGameInstance->CloneResource(PROTO_RES_LIGHTPOINTSHADER) );
    // Point
    m_LightShaders.insert(std::make_pair(LIGHTTYPE::TYPE_POINT, spShader));
    // Spot
    m_LightShaders.insert(std::make_pair(LIGHTTYPE::TYPE_SPOT, spShader));
    // FLASH LIGHT
    m_LightShaders.insert(std::make_pair(LIGHTTYPE::TYPE_FLASHLIGHT, spShader));
    // Rect
    m_spVIBufferRect = static_pointer_cast<UVIBufferRect>(spGameInstance->CloneResource(PROTO_RES_VIBUFFERRECT));
    return S_OK;
}


void UScene::CollisionTick(const _double& _dTimeDelta)
{

}

void UScene::RenderLights(CSHPTRREF<URenderObject> _spRenderObject)
{
    SHPTR<UGameInstance> spGameInstnace = GET_INSTANCE(UGameInstance);
    SHPTR<URenderTargetGroup> spGroup{ spGameInstnace->FindRenderTargetGroup(RTGROUPID::NONALPHA_DEFFERED)};
    RETURN_CHECK(nullptr == spGroup, ;);

    for (const std::pair<LIGHTTYPE, SHPTR<UShader>>& LightPair : m_LightShaders)
    {
        const auto& LightIterator = m_LightGroup.find(LightPair.first);
       
        if (m_LightGroup.end() == LightIterator)
            continue;
        {
     //       SMART_LOCK LL{ UGameInstance::GetResourceMutex() };
            LightPair.second->SettingPipeLineState(_spRenderObject->GetCommand());
            LightPair.second->SetTableDescriptor(_spRenderObject->GetTableDescriptor());
        }
        
        // Bind Transform Param
        LightPair.second->BindSRVBuffer(SRV_REGISTER::T0, spGroup->GetRenderTargetTexture(RTOBJID::NONALPHA_NOMRAL_DEFFERED));
        LightPair.second->BindSRVBuffer(SRV_REGISTER::T1, spGroup->GetRenderTargetTexture(RTOBJID::NONALPHA_DEPTH_DEFFERED));
        LightPair.second->BindSRVBuffer(SRV_REGISTER::T2, spGroup->GetRenderTargetTexture(RTOBJID::NONALPHA_POSITION_DEFFERED));

        // Draw Light
        for (CSHPTRREF<ULight> spLight : LightIterator->second)
        {
            spLight->Render(_spRenderObject, m_spVIBufferRect, LightPair.second);
        }
    }
}

HRESULT UScene::AddLight(const LIGHTINFO& _stLightInfo, const LIGHTCONTROL& _stLightControl)
{
    SHPTR<ULight> pLight = CreateConstructorToNative<ULight>(GetDevice(), _stLightInfo, _stLightControl);

    const LIGHTGROUP::iterator& it = m_LightGroup.find(_stLightInfo.eLightType);
    if (it == m_LightGroup.end()) {
        LIGHTCONTAINER LightContainer{  };
        LightContainer.reserve(10);
        LightContainer.push_back(pLight);
        m_LightGroup.insert(std::make_pair(_stLightInfo.eLightType, LightContainer));
    }
    else {
        it->second.push_back(pLight);
    }
    return S_OK;
}

void UScene::OutLight(const LIGHTTYPE& _eLightType, const _uint _iIndex, SHPTR<ULight>& _spLight)
{
    if (_eLightType <= LIGHTTYPE::LIGHT_END && _eLightType >= LIGHTTYPE::TYPE_DIRECTIONAL)
    {
        if (_iIndex >= m_LightGroup[_eLightType].size())
            return;

        LIGHTCONTAINER::iterator it = m_LightGroup[_eLightType].begin();

        it = it + _iIndex;
        if (it != m_LightGroup[_eLightType].end())
            _spLight = (*it);
    }
}

HRESULT UScene::ActiveLIght(const LIGHTTYPE& _eLightType, const _uint& _iIndex, const _bool& _isActive)
{
    if (_eLightType <= LIGHTTYPE::LIGHT_END && _eLightType >= LIGHTTYPE::TYPE_DIRECTIONAL)
    {
        RETURN_CHECK(_iIndex >= m_LightGroup[_eLightType].size(), E_FAIL);

        LIGHTCONTAINER::iterator it = m_LightGroup[_eLightType].begin();

        it = it + _iIndex;
        if (it != m_LightGroup[_eLightType].end())
            (*it)->SetActive(_isActive);
    }
    return S_OK;
}

HRESULT UScene::DeleteLight(const LIGHTTYPE& _eLightType, const _uint& _iIndex)
{
    if (_eLightType <= LIGHTTYPE::LIGHT_END && _eLightType >= LIGHTTYPE::TYPE_DIRECTIONAL)
    {
        RETURN_CHECK(_iIndex >= m_LightGroup[_eLightType].size(), E_FAIL);

        LIGHTCONTAINER::iterator it = m_LightGroup[_eLightType].begin();

        it = it + _iIndex;
        if (it != m_LightGroup[_eLightType].end())
        {
            m_LightGroup[_eLightType].erase(it);
        }
    }
    return S_OK;
}

HRESULT UScene::ClearLight()
{
    m_LightGroup.clear();
    return S_OK;
}

HRESULT UScene::StartLoadingResource(const VECTOR<UDelegate<void, _bool&>>& _vecFunctions, const _uint& _iMethodCount)
{
    m_spLoader = CreateNative<ULoader>(_vecFunctions, _iMethodCount);
    return S_OK;
}
