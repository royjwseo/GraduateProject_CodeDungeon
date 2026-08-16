#include "EngineDefines.h"
#include "UTexGroup.h"
#include "UTexture.h"
#include "UShader.h"
#include <filesystem>
#include "UGameInstance.h"

namespace fs = std::filesystem;


UTexGroup::UTexGroup(CSHPTRREF<UDevice> _spDevice)
	: UResource(_spDevice),
	m_vecTextureContainer{},
	m_TextureNames{}
{
}

UTexGroup::UTexGroup(const UTexGroup& _rhs)
	: UResource(_rhs),
	m_vecTextureContainer{ _rhs.m_vecTextureContainer },
	m_TextureNames{ _rhs.m_TextureNames }
{
}

SHPTR<UTexture> UTexGroup::GetTexture(const _wstring& _wstrFileName)
{
	const auto& iter = m_TextureNames.find(_wstrFileName);
	if (iter == m_TextureNames.end())
		return nullptr;

	RETURN_CHECK(iter == m_TextureNames.end(), nullptr);

	return m_vecTextureContainer[iter->second];
}

SHPTR<UTexture> UTexGroup::GetTexture(const _uint& _iIndex)
{
	RETURN_CHECK(_iIndex >= m_vecTextureContainer.size(), nullptr);
	return m_vecTextureContainer[_iIndex];
}

void UTexGroup::Free()
{
	m_vecTextureContainer.clear();
}

HRESULT UTexGroup::NativeConstruct()
{
	RETURN_CHECK_FAILED(__super::NativeConstruct(), E_FAIL);
	return   S_OK;
}

HRESULT UTexGroup::NativeConstruct(const _wstring& _wstrPath, const _bool& _isUnderLoadFolder, 
	const _bool _isAllLoadFolder, const TEXTURECREATETYPE _eTextureCreateType)
{
	RETURN_CHECK_FAILED(NativeConstruct(), E_FAIL);
	return LoadTexture(_wstrPath, _isUnderLoadFolder, _isAllLoadFolder, _eTextureCreateType);
}

HRESULT UTexGroup::NativeConstructClone(const VOIDDATAS& _datas)
{
	RETURN_CHECK_FAILED(__super::NativeConstructClone(_datas), E_FAIL);
	return S_OK;
}

HRESULT UTexGroup::SetUpTextureIndex(CSHPTRREF<UShader> _spShader, const SRV_REGISTER& _eReg, const _uint _iIndex)
{
	if (m_vecTextureContainer.size() <= _iIndex)
		return E_FAIL;

	return _spShader->BindSRVBuffer(_eReg, m_vecTextureContainer[_iIndex]);
}

HRESULT UTexGroup::SetUpTextureName(CSHPTRREF<UShader> _spShader, const SRV_REGISTER& _eReg, const _wstring& _wstrName)
{
	if (m_vecTextureContainer.size() <= 0)
		return E_FAIL;

	const auto& iter = m_TextureNames.find(_wstrName);
	if (iter == m_TextureNames.end())
	{
		return _spShader->BindSRVBuffer(_eReg, m_vecTextureContainer[0]);
	}

	return _spShader->BindSRVBuffer(_eReg, m_vecTextureContainer[iter->second]);
}

const _wstring& UTexGroup::GetTextureName(const _uint _iIndex)
{
	for (auto& iter : m_TextureNames) {
		if (iter.second == _iIndex)
			return iter.first;
	}
	return EMPTY_WSTR;
}

const _uint UTexGroup::GetTextureIndex(const _wstring& _wstrTextureName)
{
    const auto& iter =  	m_TextureNames.find(_wstrTextureName);
	RETURN_CHECK(iter == m_TextureNames.end(), 0);
	return iter->second;
}

HRESULT UTexGroup::LoadTexture(const _wstring& _wstrPath, const _bool& _isUnderLoadFolder, 
	const _bool _isAllLoadFolder, const TEXTURECREATETYPE _eTextureCreateType)
{
	if(false == _isUnderLoadFolder)
	{
		CreateTexture(_wstrPath, _eTextureCreateType);
	}
	else
	{
		LoadFolder(_wstrPath, _isUnderLoadFolder, _isAllLoadFolder, _eTextureCreateType);
	}

	RETURN_CHECK(m_TextureNames.size() <= 0, E_FAIL);
	return S_OK;
}

void UTexGroup::LoadFolder(const _wstring& _wstrPath, const _bool& _isUnderLoadFolder, 
	const _bool _isAllLoadFolder, const TEXTURECREATETYPE _eTextureCreateType)
{
	fs::directory_iterator end;
	for (fs::directory_iterator iter(_wstrPath); iter != end; ++iter)
	{
		// RegularFile
		if (fs::is_regular_file(iter->status()))
		{
			CreateTexture(iter->path(), _eTextureCreateType);
		}
		else if (true == _isAllLoadFolder && fs::is_directory(iter->status()))
		{
			LoadFolder(iter->path(), _isUnderLoadFolder, _isAllLoadFolder, _eTextureCreateType);
		}
	}
}

void UTexGroup::CreateTexture(const _wstring& _wstrPath, const TEXTURECREATETYPE _eTextureCreateType)
{
	SHPTR<UTexture> pTexture = CreateNativeNotMsg<UTexture>(GetDevice(), _wstrPath, _eTextureCreateType);
	if (nullptr != pTexture)
	{
		_wstring wstrPath = _wstrPath;
		wstrPath = wstrPath.substr(wstrPath.rfind(L"\\"), wstrPath.length());
		wstrPath = wstrPath.substr(1, wstrPath.rfind(L".") - 1);

		m_TextureNames.insert(std::pair<_wstring, _uint>(wstrPath, (_uint)m_vecTextureContainer.size()));
		m_vecTextureContainer.push_back(pTexture);
	}
}
#ifdef _USE_IMGUI
#include "UMethod.h"

void UTexGroup::ShowObjectInfo()
{
	static const char* TexGroupName{ "TexGroup" };
	if (ImGui::TreeNode("TexGroup"))
	{
		static VECTOR<_string> vecTextureName;
		if (vecTextureName.size() == 0)
		{
			for (auto& iter : m_TextureNames)
			{
				vecTextureName.push_back(UMethod::ConvertWToS(iter.first));
			}
		}
		ImGui::TreePop();
	}
}
#endif