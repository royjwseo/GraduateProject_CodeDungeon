#include "EngineDefines.h"
#include "UCharacter.h"

UCharacter::UCharacter(CSHPTRREF<UDevice> _spDevice, const _wstring& _wstrLayer, const CLONETYPE& _eCloneType) :
	UPawn(_spDevice, _wstrLayer, _eCloneType, BACKINGTYPE::DYNAMIC)
{
}

UCharacter::UCharacter(const UCharacter& _rhs) :
	UPawn(_rhs)
{
}

void UCharacter::Free()
{
}

HRESULT UCharacter::NativeConstruct()
{
	return __super::NativeConstruct();
}

HRESULT UCharacter::NativeConstructClone(const VOIDDATAS& _vecDatas)
{
	return __super::NativeConstructClone(_vecDatas);
}

void UCharacter::TickActive(const _double& _dTimeDelta)
{
}

void UCharacter::LateTickActive(const _double& _dTimeDelta)
{
}

HRESULT UCharacter::RenderActive(CSHPTRREF<UCommand> _spCommand, CSHPTRREF<UTableDescriptor> _spTableDescriptor)
{
	__super::RenderActive(_spCommand, _spTableDescriptor);
	return S_OK;
}

void UCharacter::Collision(CSHPTRREF<UPawn> _pEnemy)
{
}
