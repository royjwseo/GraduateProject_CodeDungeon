#ifndef _PTH_FRAMEWORK_CLIENT_PUBLIC_CBULLET_H
#define _PTH_FRAMEWORK_CLIENT_PUBLIC_CBULLET_H

#include "UPawn.h"

BEGIN(Engine)
class UModel;
class UCollider;
END

BEGIN(Client)
class CGun;
class CMonster;

class CBullet : public UPawn{
public:
	struct BULLETDESC
	{
		_float		fSpeed{ 2000.f };
		_float		fLifeTime{ 3.f };
	};
public:
	CBullet(CSHPTRREF<UDevice> _spDevice, const _wstring& _wstrLayer, const CLONETYPE& _eCloneType);
	CBullet(const CBullet& _rhs);
	DESTRUCTOR(CBullet)
public:
	void SetDir(const _float3& _vDir) { this->m_vDir = _vDir; }
public:
	CLONE_MACRO(CBullet, "CBullet::Clone To Failed")
	virtual void Free() override;
	virtual HRESULT NativeConstruct() override;
	virtual HRESULT NativeConstructClone(const VOIDDATAS& _vecDatas) override;
	bool Hit(CSHPTRREF<CMonster> _pEnemy);

	void Reset();
protected:
	// Tick, LateTick, Render
	virtual void TickActive(const _double& _dTimeDelta) override;
	virtual void LateTickActive(const _double& _dTimeDelta) override;
	virtual HRESULT RenderActive(CSHPTRREF<UCommand> _spCommand, CSHPTRREF<UTableDescriptor> _spTableDescriptor) override;
	// Damaged
	virtual void Collision(CSHPTRREF<UPawn> _pEnemy);
private:
	SHPTR<UModel>			m_spModel;
	// Bullet Desc 
	BULLETDESC				m_BulletDesc;
	CUSTIMER						m_CusTimer;

	_float3							m_vDir;

	SHPTR< UCollider>		m_spCollider;
	SHPTR<UCollider>		m_spFirstCheckCollider;
};

END

#endif // _PTH_FRAMEWORK_CLIENT_PUBLIC_CBULLET_H