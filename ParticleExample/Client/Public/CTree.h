#ifndef _PTH_FRAMEWORK_CLIENT_PUBLIC_CTREE_H
#define _PTH_FRAMEWORK_CLIENT_PUBLIC_CTREE_H

#include "UPawn.h"

BEGIN(Engine)
class UModel;
END

BEGIN(Client)

class CTree final  : public UPawn {
public:
	CTree(CSHPTRREF<UDevice> _spDevice, const _wstring& _wstrLayer, const CLONETYPE& _eCloneType);
	CTree(const CTree& _rhs);
	DESTRUCTOR(CTree)
public:
	CLONE_MACRO(CTree, "CTree::Clone To Failed")
		virtual void Free() override;
	virtual HRESULT NativeConstruct() override;
	virtual HRESULT NativeConstructClone(const VOIDDATAS& _vecDatas) override;
protected:
	// Tick, LateTick, Render
	virtual void TickActive(const _double& _dTimeDelta) override;
	virtual void LateTickActive(const _double& _dTimeDelta) override;
	virtual HRESULT RenderActive(CSHPTRREF<UCommand> _spCommand, CSHPTRREF<UTableDescriptor> _spTableDescriptor) override;
	// Damaged
	virtual void Collision(CSHPTRREF<UPawn> _pEnemy);
private:
	SHPTR<UModel> m_spModel;
};

END

#endif // _PTH_FRAMEWORK_CLIENT_PUBLIC_CTREE_H