#ifndef _PTH_FRAMEWORK_ENGIEN_PUBLIC_UTHREADPOOLMANAGER_H
#define _PTH_FRAMEWORK_ENGIEN_PUBLIC_UTHREADPOOLMANAGER_H

#include "UBase.h"

BEGIN(Engine)
class UPopThreadObject;


class UThreadPoolManager : public UBase {
public:
	UThreadPoolManager();
	NO_COPY(UThreadPoolManager)
	DESTRUCTOR(UThreadPoolManager)
public:
	virtual void Free() override;

	void RegisterOutSideWorkThread(void (*_Fucn)(void*), void* _pData);
	void RegisterInsideWorkThread(const _uint _iInsideThreadCnt);

	void StartThreads();

	// Push Inside WorkThread
	void PushInsideWorkThread(const std::function<void(const VOIDDATAS&)>& _Func, VOIDDATAS _pData);

	void EndThreads();
private:
	static void RunThread(void* _pThreadPoolManager);
	void Running();
private:

};

END

#endif // _PTH_FRAMEWORK_ENGIEN_PUBLIC_UTHREADPOOLMANAGER_H