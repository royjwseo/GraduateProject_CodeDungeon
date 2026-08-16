#ifndef _PTH_FRAMEWORK_ENGIEN_PUBLIC_UCHARACTER_H
#define _PTH_FRAMEWORK_ENGIEN_PUBLIC_UCHARACTER_H

#include "UPawn.h"

BEGIN(Engine)

class ENGINE_DLL DEF_CACHE_ALGIN UCharacter abstract : public UPawn {
public:
	UCharacter(CSHPTRREF<UDevice> _spDevice, 	const _wstring& _wstrLayer, const CLONETYPE& _eCloneType);
	UCharacter(const UCharacter& _rhs);
	DESTRUCTOR(UCharacter)
	// UPawn을(를) 통해 상속됨
	virtual SHPTR<UCloneObject> Clone(const VOIDDATAS& _vecDatas) override PURE;
	virtual void Free() override  PURE;
	virtual HRESULT NativeConstruct() override  PURE;
	virtual HRESULT NativeConstructClone(const VOIDDATAS& _vecDatas) override  PURE;
protected:
	virtual void TickActive(const _double& _dTimeDelta) override  PURE;
	virtual void LateTickActive(const _double& _dTimeDelta) override  PURE;
	virtual HRESULT RenderActive(CSHPTRREF<UCommand> _spCommand, CSHPTRREF<UTableDescriptor> _spTableDescriptor) override  PURE;
	virtual void Collision(CSHPTRREF<UPawn> _pEnemy) override  PURE;

};

END


#endif // _PTH_FRAMEWORK_ENGIEN_PUBLIC_UCHARACTER_H