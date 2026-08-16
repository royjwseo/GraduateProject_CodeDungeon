#ifndef _PTH_FRAMEWORK_ENGIEN_PUBLIC_ULOADER_H
#define _PTH_FRAMEWORK_ENGIEN_PUBLIC_ULOADER_H

#include "UBase.h"
#include "UResourceLoader.h"
#include <thread>

BEGIN(Engine)

class ENGINE_DLL ULoader : public UBase {
public:
	ULoader();
	NO_COPY(ULoader)
	DESTRUCTOR(ULoader)
public:
	const VECTOR<SHPTR<UResourceLoader>>& GetResourceLoaders() const { return m_vecResourcLoaders; }
public:
	// Free
	virtual void Free() override;
	HRESULT NativeConstruct(const VECTOR< UDelegate<void, _bool&>>& _vecFunctions, const _uint& _iMethodCount);
	_bool IsLoadEnd();
private:
	VECTOR<std::thread>									m_vecThreads;
	VECTOR<SHPTR<UResourceLoader>>		m_vecResourcLoaders;
};

END

#endif // _PTH_FRAMEWORK_ENGIEN_PUBLIC_ULOADER_H

