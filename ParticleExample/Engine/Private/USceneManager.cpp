#include "EngineDefines.h"
#include "USceneManager.h"
#include "UGraphicDevice.h"
#include "ULight.h"
#include "UScene.h"
#include "UGameInstance.h"
#include "URenderer.h"
#include "UShader.h"
#include "URenderObject.h"
#include "UCommand.h"
#include "UGrobalConstantBuffer.h"
#include "UPipeLine.h"
#include "URenderTargetManager.h"
#include "URenderTargetGroup.h"
#include "ThreadMethods.h"

USceneManager::USceneManager() :
m_c_iLightParamSize{ (_uint)sizeof(LIGHTPARAM) },
m_iLightCamIndex{0},
m_spLightRenderObject{nullptr},
m_stLightParam{},
m_spLightGrobalBuffer{nullptr},
m_spCurScene{nullptr}
{
}

void USceneManager::Free()
{

}

HRESULT USceneManager::ReadySceneManager(UGameInstance* _pGameInstance, CSHPTRREF<URenderObject> _spRenderObject)
{
	RETURN_CHECK(nullptr == _spRenderObject , E_FAIL);
	m_spLightRenderObject = _spRenderObject;

	_pGameInstance->GetGrobalConstantBuffer(GROBAl_LIGHTSTATIC, m_spLightGrobalBuffer);
	return S_OK;
}

void USceneManager::RegisterScene(CSHPTRREF<UScene> _spScene)
{
	RETURN_CHECK(nullptr == _spScene, ;);

	m_spCurScene.reset();

	SHPTR<UGameInstance> spGameInstance = GET_INSTANCE(UGameInstance);
	spGameInstance->ClearOnceTypeData();

	m_spCurScene = _spScene;
	m_spCurScene->LoadSceneData();
}

void USceneManager::Tick(const _double& _dTimeDelta)
{
	RETURN_CHECK(nullptr == m_spCurScene, ;);
	m_spCurScene->Tick(_dTimeDelta);
}

void USceneManager::LateTick(const _double& _dTimeDelta)
{
	RETURN_CHECK(nullptr == m_spCurScene, ;);
	m_spCurScene->LateTick(_dTimeDelta);
}

void USceneManager::CollisionTick(const _double& _dTimeDelta)
{
	RETURN_CHECK(nullptr == m_spCurScene, ;);
	m_spCurScene->CollisionTick(_dTimeDelta);
}

void USceneManager::OutLightControlInfo(LIGHTPARAM& _stLightControl)
{
	_stLightControl = m_stLightParam;
}

void USceneManager::ChangeLightCamIndex(const CAMID& _iID)
{
	m_iLightCamIndex = _iID;
}

void USceneManager::RenderLight(CSHPTRREF<UPipeLine> _spPipeLine, CSHPTRREF<URenderTargetManager> _spRenderTargetManager,
	const TRANSFORMPARAM& _stDefferendParam)
{
	SHPTR<UGameInstance> spGameInstance = GET_INSTANCE(UGameInstance);
	{
		SHPTR<URenderTargetGroup> spRenderTargetGroup {_spRenderTargetManager->FindRenderTargetGroup(RTGROUPID::LIGHTSHADE_DEFFERED)};
		m_spLightRenderObject->BeginRenderTareget(spGameInstance->GetD3DViewport(), spGameInstance->GetD3DWindowSizeRect());
		spRenderTargetGroup->WaitResourceToTarget(m_spLightRenderObject);
		spRenderTargetGroup->ClearRenderTargetView(m_spLightRenderObject);
		spRenderTargetGroup->OmSetRenderTargets(m_spLightRenderObject);


		if (nullptr != m_spCurScene)
		{
			m_stLightParam.mScreenWorldMatrix = _stDefferendParam.mWorldMatrix;
			{
				// View Proj
				m_stLightParam.mScreenViewmatrix = _spPipeLine->GetCamViewMatrix(_stDefferendParam.iCamIndex);
				m_stLightParam.mScreenProjMatrix = _spPipeLine->GetCamProjMatrix(_stDefferendParam.iCamIndex);
				// Far
				m_stLightParam.fCamFar = _spPipeLine->GetCamFar(m_iLightCamIndex);
				// View Proj
				m_stLightParam.mViewMatrix = _spPipeLine->GetCamViewMatrix(m_iLightCamIndex);
				m_stLightParam.mProjMatrix = _spPipeLine->GetCamProjMatrix(m_iLightCamIndex);
				//CamPos
				m_stLightParam.vCamPos = _spPipeLine->GetCameraPosition(m_iLightCamIndex);
			}

			m_stLightParam.mScreenViewmatrix = m_stLightParam.mScreenViewmatrix.Transpose();
			m_stLightParam.mScreenProjMatrix = m_stLightParam.mScreenProjMatrix.Transpose();

			m_stLightParam.mViewMatrixInv = m_stLightParam.mViewMatrix;
			m_stLightParam.mProjMatrixInv = m_stLightParam.mProjMatrix;

			m_stLightParam.mViewMatrix = m_stLightParam.mViewMatrix.Transpose();
			m_stLightParam.mProjMatrix = m_stLightParam.mProjMatrix.Transpose();
			// View Proj Inverse
			m_stLightParam.mViewMatrixInv = m_stLightParam.mViewMatrixInv.Invert();
			m_stLightParam.mViewMatrixInv = m_stLightParam.mViewMatrixInv.Transpose();

			m_stLightParam.mProjMatrixInv = m_stLightParam.mProjMatrixInv.Invert();
			m_stLightParam.mProjMatrixInv = m_stLightParam.mProjMatrixInv.Transpose();
			// Setting Grobal 
			{
				m_spLightGrobalBuffer->SettingGrobalData(m_spLightRenderObject->GetCommand(),
					&m_stLightParam, m_c_iLightParamSize);
			}

			m_spCurScene->RenderLights(m_spLightRenderObject);
		}
		spRenderTargetGroup->WaitTargetToResource(m_spLightRenderObject);
		m_spLightRenderObject->ExcuteCommand();
	}
}

HRESULT USceneManager::AddLight(const LIGHTINFO& _stInfo)
{
	RETURN_CHECK(nullptr == m_spCurScene, E_FAIL);
	return m_spCurScene->AddLight(_stInfo);
}

void USceneManager::OutLight(const LIGHTTYPE& _eLightType, const _uint& _iIndex, SHPTR<ULight>& _spLight)
{
	RETURN_CHECK(nullptr == m_spCurScene, ;);
	m_spCurScene->OutLight(_eLightType, _iIndex, _spLight);
}

HRESULT USceneManager::ActiveLIght(const LIGHTTYPE& _eLightType, const _uint& _iIndex, const _bool& _isActive)
{
	RETURN_CHECK(nullptr == m_spCurScene, E_FAIL);
	return m_spCurScene->ActiveLIght(_eLightType, _iIndex, _isActive);
}

HRESULT USceneManager::DeleteLight(const LIGHTTYPE& _eLightType, const _uint& _iIndex)
{
	RETURN_CHECK(nullptr == m_spCurScene, E_FAIL);
	return m_spCurScene->DeleteLight(_eLightType, _iIndex);
}

HRESULT USceneManager::ClearLight()
{
	RETURN_CHECK(nullptr == m_spCurScene, E_FAIL);
	return m_spCurScene->ClearLight();
}