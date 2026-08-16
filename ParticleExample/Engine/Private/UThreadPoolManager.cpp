#include "EngineDefines.h"
#include "UThreadPoolManager.h"
#include "UMethod.h"
#include "UGameInstance.h"

UThreadPoolManager::UThreadPoolManager() 
{
}

void UThreadPoolManager::Free()
{
}

void UThreadPoolManager::RegisterOutSideWorkThread(void(*_Fucn)(void*), void* _pData)
{
}

void UThreadPoolManager::RegisterInsideWorkThread(const _uint _iInsideThreadCnt)
{
}

void UThreadPoolManager::StartThreads()
{
}

void UThreadPoolManager::PushInsideWorkThread(const std::function<void(const VOIDDATAS&)>& _Func, VOIDDATAS _pData)
{
}

void UThreadPoolManager::EndThreads()
{
}

void UThreadPoolManager::RunThread(void* _pThreadPoolManager)
{
}

void UThreadPoolManager::Running()
{
}


