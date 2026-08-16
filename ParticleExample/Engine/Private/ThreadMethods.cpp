#include "EngineDefines.h"
#include "ThreadMethods.h"

#include "UCommand.h"
#include "UGameInstance.h"

// Gpu Cpu Synchronization

void Engine::THREADMETHOD::GpuCpuSynchronization(const VOIDDATAS& _vecDatas)
{
	RETURN_CHECK(_vecDatas.size() <= 0, ;);
	UCommand* pCommand{ nullptr };
	ARRAY<ID3D12CommandList*, MAX_THREAD_COMMAND_CNT>* CommandList{ nullptr };
	pCommand = static_cast<UCommand*>(_vecDatas[0]);
	pCommand->GpuCpuSynchronization();
}
