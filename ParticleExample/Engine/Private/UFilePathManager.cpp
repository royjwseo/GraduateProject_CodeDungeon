#include "EngineDefines.h"
#include "UFilePathManager.h"
#include <filesystem>

namespace fs = std::filesystem;

UFilePathManager::UFilePathManager() : m_spFileGroup(nullptr)
{
}

void UFilePathManager::Free()
{
	LIST<SHPTR<FILEGROUP>> FILEGROUPS;
	ClearLoader(FILEGROUPS, m_spFileGroup);
	FILEGROUPS.clear();
}

SHPTR<FILEGROUP> UFilePathManager::FindFolder(const PATHS& _vecFolderList)
{
	RETURN_CHECK(nullptr == m_spFileGroup, nullptr);
	RETURN_CHECK(0 >= _vecFolderList.size(), nullptr);

	UNORMAP<_wstring, SHPTR<FILEGROUP>>::iterator it;
	_uint i = 0;
	_uint iMax = (_uint)_vecFolderList.size() - 1;
	_bool bRun{ true };
	SHPTR<FILEGROUP> pFindGroup = m_spFileGroup;
	while (bRun)
	{
		pFindGroup = pFindGroup->FindGroup(_vecFolderList[i]);
		RETURN_CHECK(nullptr == pFindGroup, nullptr);
		if (i++ == iMax)
			break;
	}

	return pFindGroup;
}

SHPTR<FILEGROUP> UFilePathManager::FindFolder(const _wstring& _wstrFindName, const _wstring& _wstrParentsFolderName)
{
	const auto& iter = m_FileFolderGroup.find(_wstrFindName);
	if (iter == m_FileFolderGroup.end())
		return nullptr;

	if (L"" == _wstrParentsFolderName)
		return iter->second.front();
	else
	{
		for (auto& value : iter->second)
		{
			if (value->wstrFolderName == _wstrParentsFolderName)
				return value;
		}
	}
	return iter->second.front();
}

HRESULT UFilePathManager::LoadFirstFilder(const _wstring& _wstrFilePath)
{
	LIST<SHPTR<FILEGROUP>> FILEGROUPS; 
	ClearLoader(FILEGROUPS, m_spFileGroup);
	FILEGROUPS.clear();
	m_spFileGroup = std::make_shared<FILEGROUP>();
	fs::path p {_wstrFilePath};
	m_spFileGroup->wstrPath = _wstrFilePath;
	m_spFileGroup->wstrFolderName = p.filename();
	VECTOR<SHPTR<FILEGROUP>> vecFileGroup;
	vecFileGroup.push_back(m_spFileGroup);
	m_FileFolderGroup.insert(std::pair<_wstring, VECTOR<SHPTR<FILEGROUP>>>(m_spFileGroup->wstrFolderName, vecFileGroup));
	LoadUpperFolder(m_spFileGroup->wstrFolderName, _wstrFilePath, m_spFileGroup);
	return S_OK;
}

HRESULT UFilePathManager::ClearLoader(LIST<SHPTR<FILEGROUP>>& _lsFileGroup, CSHPTRREF<FILEGROUP> _spFileGroup)
{
	RETURN_CHECK(nullptr == _spFileGroup, E_FAIL);

	SHPTR<FILEGROUP> spFileGroup = _spFileGroup;

	for (auto& iter : spFileGroup->conuomapUnderFileGroupList)
	{
		_lsFileGroup.push_back(iter.second);
		ClearLoader(_lsFileGroup, iter.second);
	}
	_spFileGroup->FileDataList.clear();
	_spFileGroup->conuomapUnderFileGroupList.clear();
	return S_OK;
}

HRESULT UFilePathManager::LoadUpperFolder(const _wstring& _wstrFirstFolderName, 
	const _wstring& _wstrPath, CSHPTRREF<FILEGROUP> _spFileGroup)
{
	fs::directory_iterator end;
	_ushort sIndex{ 0 };
	for (fs::directory_iterator iter(_wstrPath); iter != end; ++iter)
	{
		if (fs::is_regular_file(iter->status()))
		{
			SHPTR<FILEDATA> spFileData = std::make_shared<FILEDATA>();
			spFileData->wstrfileName = iter->path().filename();
			spFileData->wstrfilePath = iter->path();
			spFileData->sindex = sIndex;
			_spFileGroup->AddFileData(spFileData->wstrfileName, spFileData);
		}
		else if (fs::is_directory(iter->status()))
		{
			SHPTR<FILEGROUP> spFileGroup = std::make_shared<FILEGROUP>();
			spFileGroup->wstrFolderName = iter->path().filename();
			spFileGroup->wstrPath = iter->path();
			spFileGroup->spParentsUpper = _spFileGroup;
			_spFileGroup->AddFileGroup(spFileGroup->wstrFolderName, spFileGroup);
		
			const auto& iter = m_FileFolderGroup.find(spFileGroup->wstrFolderName);
			if (iter == m_FileFolderGroup.end())
			{
				VECTOR<SHPTR<FILEGROUP>> vecFileGroup;
				vecFileGroup.push_back(spFileGroup);
				m_FileFolderGroup.insert(std::pair<_wstring, VECTOR<SHPTR<FILEGROUP>>>(spFileGroup->wstrFolderName, vecFileGroup));
			}
			else
			{
				iter->second.push_back(spFileGroup);
			}

			LoadUpperFolder(spFileGroup->wstrFolderName, spFileGroup->wstrPath, spFileGroup);
		}
	}
	return S_OK;
}
