#include "ClientDefine.h"
#include "CLogoScene.h"

CLogoScene::CLogoScene(CSHPTRREF<UDevice> _spDevice)
	: UScene(_spDevice, 0)
{
}

void CLogoScene::Free()
{
}

HRESULT CLogoScene::LoadSceneData()
{
	return S_OK;
}

void CLogoScene::Tick(const _double& _dTimeDelta)
{
}

void CLogoScene::LateTick(const _double& _dTimeDelta)
{
}
