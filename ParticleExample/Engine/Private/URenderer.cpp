#include "EngineDefines.h"
#include "URenderer.h"
#include "UPipeLine.h"
#include "USceneManager.h"
#include "UShader.h"
#include "UGameInstance.h"
#include "URenderTargetManager.h"
#include "UShaderConstantBuffer.h"
#include "UTransform.h"
#include "UGraphicDevice.h"
#include "UVIBufferRect.h"
#include "URenderTargetGroup.h"
#include "UTransform.h"
#include "UDefferedCamera.h"
#include "URenderTargetManager.h"
#include "UGrobalConstantBuffer.h"
#include "UPawn.h"
#include "URenderObject.h"
#include "UMethod.h"
#include "UCommand.h"
#include "UGrobalConstantBuffer.h"

URenderer::URenderer(CSHPTRREF <UDevice> _spDevice, CSHPTRREF<UCommand> _spCommand,
    CSHPTRREF<UGraphicDevice> _spGraphicDevice, CSHPTRREF<UPipeLine> _spPipeLine,
    CSHPTRREF<USceneManager> _spSceneManager, CSHPTRREF<URenderTargetManager> _spRenderTargetManager,
    CSHPTRREF<UComputeManager> _spComputeManager, CSHPTRREF<URenderObject> _spRenderObject) :
UComponent(_spDevice),
m_spTransformConstantBuffer{ nullptr },
m_spDefferedCamera{ nullptr },
m_stFinalRenderTransformParam{},
m_spGraphicDevice{ _spGraphicDevice },
m_spPipeLine{ _spPipeLine },m_spSceneManager{ _spSceneManager },
m_spRenderTargetManager{ _spRenderTargetManager },
m_spComputeManager{ _spComputeManager },
m_spGraphicsRenderObject{ _spRenderObject },
m_spVIBufferRect{ nullptr },
m_sNonAlphaBlendIndex{0}
#ifdef _USE_DEBUGGING
,m_stRenderDebugging{}
#endif
{
}

void URenderer::Free()
{
}

HRESULT URenderer::NativeConstruct()
{
    SHPTR<UGameInstance> spGameInstance = GET_INSTANCE(UGameInstance);
    {
        spGameInstance->GetGrobalConstantBuffer(GROBAL_GROBALDATA, m_spGrobalBuffer);
    }
    // Rendering
    {
        m_spVIBufferRect = static_pointer_cast<UVIBufferRect>(spGameInstance->CloneResource(PROTO_RES_VIBUFFERRECT));
    }
    {
        // Add Shader 
        {
            m_ShaderObjects.insert(std::pair<_wstring, SHPTR<UShader>>(PROTO_RES_FINALDEFFEREDSHADER, static_pointer_cast<UShader>(
                spGameInstance->CloneResource(PROTO_RES_FINALDEFFEREDSHADER))));

            m_ShaderObjects.insert(std::pair<_wstring, SHPTR<UShader>>(PROTO_RES_BLENDDEFFEREDSHADER, static_pointer_cast<UShader>(
                spGameInstance->CloneResource(PROTO_RES_BLENDDEFFEREDSHADER))));
#ifdef _USE_DEBUGGING
            m_ShaderObjects.insert(std::pair<_wstring, SHPTR<UShader>>(PROTO_RES_DEBUG2DTARGETSHADER, static_pointer_cast<UShader>(
                spGameInstance->CloneResource(PROTO_RES_DEBUG2DTARGETSHADER))));
#endif
        }
    }
    {
        m_stFinalRenderTransformParam.mWorldMatrix._11 = spGameInstance->GetD3DViewport().Width;
        m_stFinalRenderTransformParam.mWorldMatrix._22 = spGameInstance->GetD3DViewport().Height ;
        m_stFinalRenderTransformParam.mWorldMatrix = m_stFinalRenderTransformParam.mWorldMatrix.Transpose();
        {

            // Create Deffered Camera (디퍼드용 카메라)
            UCamera::CAMDESC tDesc;
            tDesc.stCamProj = UCamera::CAMPROJ(UCamera::PROJECTION_TYPE::ORTHOGRAPHIC, _float3(0.f, 0.f, 0.f),
                _float3(0.f, 0.f, 0.f),
                DirectX::XMConvertToRadians(60.0f), spGameInstance->GetD3DViewport().Width, 
                spGameInstance->GetD3DViewport().Height);
            tDesc.stCamValue = UCamera::CAMVALUE(5.f, DirectX::XMConvertToRadians(90.f));
            tDesc.eCamType = CAMERATYPE::DEFFERED;

            VOIDDATAS vecDatas;
            vecDatas.push_back(&tDesc);

            m_spDefferedCamera = static_pointer_cast<UDefferedCamera>(spGameInstance->CloneActorAdd(
                PROTO_ACTOR_DEFFEREDCAMERA, vecDatas ));
        }
        m_stFinalRenderTransformParam.iCamIndex = m_spDefferedCamera->GetCamID();
        // 이미 만들어진 Shader ConstnatBuffer를 가져옴
        spGameInstance->GetPreAllocatedConstantBuffer(PREALLOCATED_TRANSFORM, m_spTransformConstantBuffer);
    }
    // Initailize 
\
    return S_OK;
}

HRESULT URenderer::NativeConstructClone(const VOIDDATAS& _vectDatas)
{
    return __super::NativeConstructClone(_vectDatas);
}

HRESULT URenderer::AddRenderGroup(const RENDERID _eID, CSHPTRREF<UShader> _spShader, CSHPTRREF<UPawn> _spPawn)
{
    RETURN_CHECK(nullptr == _spPawn, E_FAIL);

    const auto& findIter = m_arrActiveDrawRenderList[_eID].find(_spShader->GetShaderDesc().wstrShaderName);
    if (findIter == m_arrActiveDrawRenderList[_eID].end())
    {
        LIST<SHPTR<UPawn>> PawnList;
        PawnList.push_back(_spPawn);
        m_arrActiveDrawRenderList[_eID].emplace(std::pair<_wstring, LIST<SHPTR<UPawn>>>(_spShader->GetShaderDesc().wstrShaderName, PawnList));
    }
    else
    {
        findIter->second.push_back(_spPawn);
    }
    return S_OK;
}

#ifdef _USE_DEBUGGING
HRESULT URenderer::AddDebugRenderGroup(const DEBUGRENDERID _eID, CSHPTRREF<UShader> _spShader, CSHPTRREF<UPawn> _spPawn)
{
    RETURN_CHECK(false == m_stRenderDebugging.isShowRenderTarget, E_FAIL);
    RETURN_CHECK(nullptr == _spPawn, E_FAIL);

    const auto& findIter = m_arrDrawDebugRenderList[_eID].find(_spShader->GetShaderDesc().wstrShaderName);
    if (findIter == m_arrDrawDebugRenderList[_eID].end())
    {
        LIST<SHPTR<UPawn>> PawnList;
        PawnList.push_back(_spPawn);
        m_arrDrawDebugRenderList[_eID].emplace(std::pair<_wstring, LIST<SHPTR<UPawn>>>(_spShader->GetShaderDesc().wstrShaderName, PawnList));
    }
    else
    {
        findIter->second.push_back(_spPawn);
    }

    return S_OK;
}
#endif

void URenderer::Tick(const _double& _dTimeDelta)
{
    m_stGrobalParam.stGrobalInfo.fDeltaTime = static_cast<_float>(_dTimeDelta);
}

HRESULT URenderer::Render()
{
    m_spPipeLine->UpdateViewProjMatrix();
    m_spPipeLine->BindViewProjMatrix(m_spGraphicsRenderObject->GetCommand());
    BindGrobalBuffer(m_spGraphicsRenderObject);
    // ============== Bind Static Buffer =============

    // Render 
    RenderPriority();
    RenderShadowLight();
    RenderNonAlphaBlend();
    RenderLights();
    RenderNonLight();
    RenderBlend();
    RenderAlphaBlend();
    RenderDistortion();
    Render2DUI();
    Render3DUI();
    RenderHDR();
    RenderEnd();

#ifdef _USE_DEBUGGING
    RenderDebug();

    m_spRenderTargetManager->RenderDebugObjects(FrameReadyDrawLast(PROTO_RES_DEBUG2DTARGETSHADER), m_spVIBufferRect,
        m_spGraphicsRenderObject->GetCommand(), SRV_REGISTER::T0);
#endif
    return S_OK;
}

void URenderer::ClearRenderingData()
{
    for (auto& iter : m_arrActiveDrawRenderList)
    {
        iter.clear();
    }
#ifdef  _USE_DEBUGGING
    for (auto& iter : m_arrDrawDebugRenderList)
    {
        iter.clear();
    }
#endif //  _USE_DEBUGGING

}

void URenderer::ClearAllData()
{
    m_ShaderObjects.clear();
   	m_spTransformConstantBuffer.reset();
   	m_spDefferedCamera.reset();

   	m_spGraphicDevice.reset();
   	m_spPipeLine.reset();
   	m_spSceneManager.reset();
   	m_spRenderTargetManager.reset();
   	m_spComputeManager.reset();
   	m_spGraphicsRenderObject.reset();
   	m_spVIBufferRect.reset();

    for (auto& iter : m_arrActiveDrawRenderList)
    {
        iter.clear();
    }
#ifdef  _USE_DEBUGGING
    for (auto& iter : m_arrDrawDebugRenderList)
    {
        iter.clear();
    }
#endif //  _USE_DEBUGGING
}

void URenderer::RenderPriority()
{

}

void URenderer::RenderShadowLight()
{

}

void URenderer::RenderNonAlphaBlend()
{
    SHPTR<UGameInstance> spGameInstance = GET_INSTANCE(UGameInstance);
    SHPTR<URenderObject> spRenderObject = spGameInstance->GetRenderObject(GRAPHICRENDEROBJECT_TYPE::NONALPHA);
    // Set Render Tareget
    SHPTR<URenderTargetGroup> spRenderTargetGroup{ m_spRenderTargetManager->FindRenderTargetGroup(RTGROUPID::NONALPHA_DEFFERED) };
    {
        SHPTR<UGameInstance> spGameInstance = GET_INSTANCE(UGameInstance);
        spRenderObject->BeginRenderTareget(spGameInstance->GetD3DViewport(), spGameInstance->GetD3DWindowSizeRect());
        m_spPipeLine->BindViewProjMatrix(spRenderObject->GetCommand());
        BindGrobalBuffer(spRenderObject);
    }
    {
        spRenderTargetGroup->WaitResourceToTarget(spRenderObject);
        spRenderTargetGroup->ClearRenderTargetView(spRenderObject);
        spRenderTargetGroup->OmSetRenderTargets(spRenderObject);
    }
    for (auto& iter : m_arrActiveDrawRenderList[RENDERID::RI_NONALPHA_PRIORITY])
    {
        RenderObject(iter.first, iter.second, spRenderObject);
    }
    for (auto& iter : m_arrActiveDrawRenderList[RENDERID::RI_NONALPHA_FIRST])
    {
        RenderObject(iter.first, iter.second, spRenderObject);
    }
    for (auto& iter : m_arrActiveDrawRenderList[RENDERID::RI_NONALPHA_MIDDLE])
    {
        RenderObject(iter.first, iter.second, spRenderObject);
    }
    for (auto& iter : m_arrActiveDrawRenderList[RENDERID::RI_NONALPHA_LAST])
    {
        RenderObject(iter.first, iter.second, spRenderObject);
    }
    spRenderTargetGroup->WaitTargetToResource(spRenderObject);
    spRenderObject->ExcuteCommand();
}

void URenderer::RenderLights()
{
    // Render Light
    m_spSceneManager->RenderLight( m_spPipeLine, m_spRenderTargetManager, m_stFinalRenderTransformParam);
}

void URenderer::RenderBlend()
{
    // Set Render Tareget
    SHPTR<URenderTargetGroup> spRenderTargetGroup {m_spRenderTargetManager->FindRenderTargetGroup(RTGROUPID::BLEND_DEFFERED)};
    spRenderTargetGroup->WaitResourceToTarget(m_spGraphicsRenderObject);
    spRenderTargetGroup->ClearRenderTargetView(m_spGraphicsRenderObject);
    spRenderTargetGroup->OmSetRenderTargets(m_spGraphicsRenderObject);
    // Bind Shader 
    SHPTR<UShader> spShader = FrameReadyDrawLast(PROTO_RES_BLENDDEFFEREDSHADER);
    spShader->BindCBVBuffer(m_spTransformConstantBuffer, &m_stFinalRenderTransformParam, UTransform::TRANSFORMPARAM_SIZE);
    {
        SHPTR<URenderTargetGroup> spNonAlpha = m_spRenderTargetManager->FindRenderTargetGroup(RTGROUPID::NONALPHA_DEFFERED);
        spShader->BindSRVBuffer(SRV_REGISTER::T0, spNonAlpha->GetRenderTargetTexture(RTOBJID::NONALPHA_DIFFUSE_DEFFERED));
    }
    {
        SHPTR<URenderTargetGroup> spLightGroup = m_spRenderTargetManager->FindRenderTargetGroup(RTGROUPID::LIGHTSHADE_DEFFERED);
        spShader->BindSRVBuffer(SRV_REGISTER::T1, spLightGroup->GetRenderTargetTexture(RTOBJID::LIGHTSHADE_AMBIENT_DEFFERED));
        spShader->BindSRVBuffer(SRV_REGISTER::T2, spLightGroup->GetRenderTargetTexture(RTOBJID::LIGHTSHADE_SHADE_DEFFERED));
        spShader->BindSRVBuffer(SRV_REGISTER::T3, spLightGroup->GetRenderTargetTexture(RTOBJID::LIGHTSHADE_SPECULAR_DEFFERED));
    }
    m_spVIBufferRect->Render(spShader, m_spGraphicsRenderObject->GetCommand());
    spRenderTargetGroup->WaitTargetToResource(m_spGraphicsRenderObject);
}

void URenderer::RenderNonLight()
{
    for (auto& iter : m_arrActiveDrawRenderList[RENDERID::RI_NOLIGHT])
    {
        RenderObject(iter.first, iter.second);
    }
}

void URenderer::RenderAlphaBlend()
{
    SHPTR<UGameInstance> spGameInstance = GET_INSTANCE(UGameInstance);
    SHPTR<URenderObject> spRenderObject = spGameInstance->GetRenderObject(GRAPHICRENDEROBJECT_TYPE::ALPHA);
    // Set Render Tareget
    SHPTR<URenderTargetGroup> spRenderTargetGroup{ m_spRenderTargetManager->FindRenderTargetGroup(RTGROUPID::ALPHA_DEFFERED) };
    spRenderObject->BeginRenderTareget(spGameInstance->GetD3DViewport(), spGameInstance->GetD3DWindowSizeRect());
    m_spPipeLine->BindViewProjMatrix(spRenderObject->GetCommand());
    BindGrobalBuffer(spRenderObject);
    spRenderTargetGroup->WaitResourceToTarget(spRenderObject);
    spRenderTargetGroup->ClearRenderTargetView(spRenderObject);
    spRenderTargetGroup->OmSetRenderTargets(spRenderObject);
    {
        for (auto& iter : m_arrActiveDrawRenderList[RENDERID::RI_ALPHA])
        {
            RenderObject(iter.first, iter.second, spRenderObject);
        }
    }
    spRenderTargetGroup->WaitTargetToResource(spRenderObject);
    spRenderObject->ExcuteCommand();
}

void URenderer::RenderDistortion()
{

}

void URenderer::Render3DUI()
{
}

void URenderer::Render2DUI()
{
    // Set Render Tareget
    SHPTR<URenderTargetGroup> spRenderTargetGroup{ m_spRenderTargetManager->FindRenderTargetGroup(RTGROUPID::UI2D_DEFFERED) };
    spRenderTargetGroup->WaitResourceToTarget(m_spGraphicsRenderObject);
    spRenderTargetGroup->ClearRenderTargetView(m_spGraphicsRenderObject);
    spRenderTargetGroup->OmSetRenderTargets(m_spGraphicsRenderObject);
    for (auto& iter : m_arrActiveDrawRenderList[RENDERID::RI_2DUI])
    {
        RenderObject(iter.first, iter.second);
    }
    spRenderTargetGroup->WaitTargetToResource(m_spGraphicsRenderObject);
}

void URenderer::RenderHDR()
{

}

void URenderer::RenderEnd()
{
    // Wait AlphaBlend
    // Is Alpha
    {  
        m_spGraphicDevice->OmSetDefaultRenderTarget();
        SHPTR<UShader> spDefferedShader = FindShader(PROTO_RES_FINALDEFFEREDSHADER);
        spDefferedShader->SettingPipeLineState(m_spGraphicsRenderObject->GetCommand());
        spDefferedShader->SetTableDescriptor(m_spGraphicsRenderObject->GetTableDescriptor());
        spDefferedShader->BindCBVBuffer(m_spTransformConstantBuffer, &m_stFinalRenderTransformParam, UTransform::TRANSFORMPARAM_SIZE);
        //  Diffuse Texture 가져와서 Bind 
        spDefferedShader->BindSRVBuffer(SRV_REGISTER::T0, m_spRenderTargetManager->
            FindRenderTargetTexture(RTGROUPID::BLEND_DEFFERED,
                RTOBJID::BLEND_SCREEN_DEFFERED));
        //  Diffuse Texture 가져와서 Bind 
        spDefferedShader->BindSRVBuffer(SRV_REGISTER::T1, m_spRenderTargetManager->
            FindRenderTargetTexture(RTGROUPID::ALPHA_DEFFERED,
                RTOBJID::ALPHA_DIFFUSE_DEFFERED));
        //  Diffuse Texture 가져와서 Bind 
        spDefferedShader->BindSRVBuffer(SRV_REGISTER::T2, m_spRenderTargetManager->
            FindRenderTargetTexture(RTGROUPID::UI2D_DEFFERED,
                RTOBJID::UI2D_SCREEN_DEFFERED));
        //  Render
        m_spVIBufferRect->Render(spDefferedShader, m_spGraphicsRenderObject->GetCommand());
    }
}

#ifdef _USE_DEBUGGING
void URenderer::RenderDebug()
{
    // Render Debugging
    for (auto& Debugging : m_arrDrawDebugRenderList)
    {
        for (auto& iter : Debugging)
        {
           RenderObject(iter.first, iter.second);
        }
    }
}
#endif

SHPTR<UShader> URenderer::FindShader(const _wstring& _wstrProto)
{
    const SHADERODERS::iterator& ShaderIter = m_ShaderObjects.find(_wstrProto);
    RETURN_CHECK(m_ShaderObjects.end() == ShaderIter, nullptr);
    return ShaderIter->second;
}


void URenderer::RenderObject(const _wstring& _wstrShaderName, PAWNLIST& _PawnList)
{
    SHPTR<UGameInstance> spGameInstance = GET_INSTANCE(UGameInstance);
    spGameInstance->SettingPipeLineState(_wstrShaderName, m_spGraphicsRenderObject->GetCommand());
    for (auto& iter : _PawnList)
    {
        iter->Render(m_spGraphicsRenderObject->GetCommand(), m_spGraphicsRenderObject->GetTableDescriptor());
    }
    _PawnList.clear();
}

void URenderer::RenderObject(const _wstring& _wstrShaderName, PAWNLIST& _PawnList, CSHPTRREF<URenderObject> _spRenderObject)
{
    SHPTR<UGameInstance> spGameInstance = GET_INSTANCE(UGameInstance);
    spGameInstance->SettingPipeLineState(_wstrShaderName, _spRenderObject->GetCommand());
    for (auto& iter : _PawnList)
    {
        iter->Render(_spRenderObject->GetCommand(), _spRenderObject->GetTableDescriptor());
    }
    _PawnList.clear();
}

SHPTR<UShader> URenderer::FrameReadyDrawFinalRenderTarget(const _wstring& _wstrShaderName, const RTGROUPID _eGroupID)
{
    m_spRenderTargetManager->OmSetRenderTargets(m_spGraphicsRenderObject, _eGroupID);
    SHPTR<UShader> spShader = FindShader(_wstrShaderName);
    spShader->SettingPipeLineState(m_spGraphicsRenderObject->GetCommand());
    return spShader;
}

SHPTR<UShader> URenderer::FrameReadyDrawLast(const _wstring& _wstrShaderName)
{
    SHPTR<UShader> spShader = FindShader(_wstrShaderName);
    spShader->SettingPipeLineState(m_spGraphicsRenderObject->GetCommand());
    spShader->SetTableDescriptor(m_spGraphicsRenderObject->GetTableDescriptor());
    return spShader;
}

void URenderer::BindGrobalBuffer(CSHPTRREF<URenderObject> _spRenderObject)
{
    RETURN_CHECK(nullptr == _spRenderObject, ;);
    // Setting Grobal Data
    m_spGrobalBuffer->SettingGrobalData(_spRenderObject->GetCommand(), &m_stGrobalParam, GROBALBUFFER_SIZE);
}
