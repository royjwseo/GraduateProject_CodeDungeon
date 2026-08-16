#include "EngineDefines.h"
#include "USkyBox.h"
#include "UVIBufferSkyBox.h"
#include "UTexGroup.h"
#include "UTexture.h"
#include "UGameInstance.h"
#include "UTransform.h"
#include "UMethod.h"

USkyBox::USkyBox(CSHPTRREF<UDevice> _spDevice, const _wstring& _wstrLayer, const CLONETYPE& _eCloneType) : 
	UPawn(_spDevice, _wstrLayer, _eCloneType, BACKINGTYPE::NON), 
	m_spVIBufferSkyBox{ nullptr },
	m_spTextureGroup{ nullptr },
	m_wstrTextureName{L""}
{
}

USkyBox::USkyBox(const USkyBox& _rhs) :
	UPawn(_rhs), 
	m_spVIBufferSkyBox{ _rhs.m_spVIBufferSkyBox },
	m_spTextureGroup{ _rhs.m_spTextureGroup },
	m_wstrTextureName{ _rhs.m_wstrTextureName }
{
}

void USkyBox::Free()
{
}

HRESULT USkyBox::NativeConstruct()
{
	return __super::NativeConstruct();
}

HRESULT USkyBox::NativeConstructClone(const VOIDDATAS& _vecDatas)
{
	if (FAILED(__super::NativeConstructClone(_vecDatas)))
		return E_FAIL;

	RETURN_CHECK(_vecDatas.size() == 0, E_FAIL);

	SKYBOXDESC stSkyBox = UMethod::ConvertTemplate_Index<SKYBOXDESC>(_vecDatas, 0);

	SHPTR<UGameInstance> spGameInstance = GET_INSTANCE(UGameInstance);
	SHPTR<FILEGROUP> spFileGroup = spGameInstance->FindFolder(stSkyBox.wstrSkyBoxTexPath);
	if (nullptr != spFileGroup)
	{
		m_spTextureGroup = CreateConstructorNative<UTexGroup>(GetDevice(), spFileGroup->wstrPath, 
			stSkyBox.isFolder, false, TEXTURECREATETYPE::TEXCUBE);
	}
	else
	{
		m_spTextureGroup = CreateConstructorNative<UTexGroup>(GetDevice(),	stSkyBox.wstrSkyBoxTexPath, 
			stSkyBox.isFolder, false, TEXTURECREATETYPE::TEXCUBE);
	}

	RETURN_CHECK(nullptr == m_spTextureGroup, E_FAIL);

	m_spVIBufferSkyBox = static_pointer_cast<UVIBufferSkyBox>(spGameInstance->CloneResource(PROTO_RES_VIBUFFERSKYBOX));
	AddShader(PROTO_RES_SKYBOXSHADER);
	m_wstrTextureName = stSkyBox.wstrSkyBoxTextureName;
	return S_OK;
}

void USkyBox::TickActive(const _double& _dTimeDelta)
{
}

void USkyBox::LateTickActive(const _double& _dTimeDelta)
{
	AddRenderGroup(RI_NONALPHA_PRIORITY);
}

HRESULT USkyBox::RenderActive(CSHPTRREF<UCommand> _spCommand, CSHPTRREF<UTableDescriptor> _spTableDescriptor)
{
	__super::RenderActive(_spCommand, _spTableDescriptor);
	GetTransform()->BindTransformData(GetShader());
	m_spTextureGroup->SetUpTextureName(GetShader(), SRV_REGISTER::T9, m_wstrTextureName);
	m_spVIBufferSkyBox->Render(GetShader(), _spCommand);
	return S_OK;
}

void USkyBox::Collision(CSHPTRREF<UPawn> _pEnemy)
{
}
#ifdef _USE_IMGUI
void USkyBox::ShowObjectInfo()
{
	__super::ShowObjectInfo();

	
}
#endif