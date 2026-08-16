#include "EngineDefines.h"
#include "UGraphicsTableDescriptor.h"
#include "UDevice.h"
#include "UGpuCommand.h"

UGraphicsTableDescriptor::UGraphicsTableDescriptor() : UTableDescriptor()
{
}

void UGraphicsTableDescriptor::Free()
{

}

void UGraphicsTableDescriptor::CommitTable(CSHPTRREF<UCommand> _spCommand)
{
	// CommitTable ->DescriptorHeapStart를 가져와서 Group만큼 Bind 하는 것이다. 
	_spCommand->GetGpuCmdList()->SetGraphicsRootDescriptorTable(GetStaticValue(), ReturnCommitTableHandle(_spCommand));
}