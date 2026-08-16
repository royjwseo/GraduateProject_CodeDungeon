#ifndef _PTH_FRAMEWORK_CLIENT_PUBLIC_CAIMINGPOINT_H
#define _PTH_FRAMEWORK_CLIENT_PUBLIC_CAIMINGPOINT_H

#include "UUserInterface.h"

BEGIN(Engine)
class URectTransform;
class UTexGroup;
END

BEGIN(Client)

class CAimingPoint final : public UUserInterface {
public:
	CAimingPoint(CSHPTRREF<UDevice> _spDevice, const _wstring& _wstrLayer, const CLONETYPE& _eCloneType);
	CAimingPoint(const CAimingPoint& _rhs);
	DESTRUCTOR(CAimingPoint)
public:
	CLONE_MACRO(CAimingPoint, "CAimingPoint::Clone To Failed")
	virtual void Free() override;
	virtual HRESULT NativeConstruct() override;
	virtual HRESULT NativeConstructClone(const VOIDDATAS& _vecDatas) override;
protected:
	// Tick, LateTick, Render
	virtual void TickActive(const _double& _dTimeDelta) override;
	virtual void LateTickActive(const _double& _dTimeDelta) override;
	virtual HRESULT RenderActive(CSHPTRREF<UCommand> _spCommand, CSHPTRREF<UTableDescriptor> _spTableDescriptor) override;
	// Damaged
	virtual void Collision(CSHPTRREF<UPawn> _pEnemy) override;
	// Ready 
	virtual HRESULT ReadyRectTransform(const VOIDDATAS& _vecDatas) override;
private:
	SHPTR<UVIBufferRect>		m_spVIBufferRect;
	SHPTR<UTexGroup>			m_spUITextureGroup;
};

END

#endif // _PTH_FRAMEWORK_CLIENT_PUBLIC_CAIMINGPOINT_H