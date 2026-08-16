#ifndef _PTH_FRAMEWORK_ENGIEN_PUBLIC_URAWDATA_H
#define _PTH_FRAMEWORK_ENGIEN_PUBLIC_URAWDATA_H

#include "UBase.h"

BEGIN(Engine)

class ENGINE_DLL URawData : public UBase {
public:
	URawData();
	NO_COPY(URawData)
	DESTRUCTOR(URawData)
public:
	const BYTE* GetRawImagePixel() const { return m_upRawImagePixel.get(); }
	const _uint& GetDataWidth() const { return m_iDataWidth; }
	const _uint& GetDataHeight() const { return m_iDataHeight; }
public:
	virtual void Free() override;
	HRESULT NativeConstruct(const _wstring& _wstrFilePath, const _uint _iWidth, const _uint _iHeight, bool _IsFilpY = false);

private:
	UNPTR<BYTE>		m_upRawImagePixel;
	_uint						m_iDataWidth;
	_uint						m_iDataHeight;
};

END

#endif // _PTH_FRAMEWORK_ENGIEN_PUBLIC_URAWDATA_H
