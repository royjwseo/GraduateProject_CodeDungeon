#include "EngineDefines.h"
#include "URawData.h"

URawData::URawData() : 
	m_upRawImagePixel{ nullptr },
	m_iDataWidth{ 0 },
	m_iDataHeight{ 0 }
{
}

void URawData::Free()
{
}

HRESULT URawData::NativeConstruct(const _wstring& _wstrFilePath, const _uint _iWidth, const _uint _iHeight, bool _IsFilpY)
{
	m_iDataWidth = _iWidth;
	m_iDataHeight = _iHeight;

	UNPTR<BYTE> spRawImageData = std::make_unique<BYTE>(m_iDataWidth * m_iDataHeight);

	HANDLE hFile = ::CreateFile(_wstrFilePath.data(), GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL | FILE_ATTRIBUTE_READONLY, NULL);
	DWORD dwBytesRead;
	::ReadFile(hFile, m_upRawImagePixel.get(), (m_iDataWidth * m_iDataHeight), &dwBytesRead, NULL);
	::CloseHandle(hFile);

	if (_IsFilpY)
	{
		m_upRawImagePixel = std::make_unique<BYTE>(m_iDataWidth * m_iDataHeight);
		for (_uint z = 0; z < m_iDataHeight; z++)
		{
			for (_uint x = 0; x < m_iDataWidth; x++)
			{
				m_upRawImagePixel.get()[x + ((m_iDataHeight - 1 - z) * m_iDataWidth)] = spRawImageData.get()[x + (z * m_iDataHeight)];
			}
		}
	}
	else
	{
		m_upRawImagePixel.swap(spRawImageData);
	}
	spRawImageData.reset();
	return S_OK;
}
