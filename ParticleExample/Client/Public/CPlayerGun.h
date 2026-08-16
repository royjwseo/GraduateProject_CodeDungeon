#ifndef _PTH_FRAMEWORK_CLIENT_PUBLIC_CPLAYERGUN_H
#define _PTH_FRAMEWORK_CLIENT_PUBLIC_CPLAYERGUN_H

#include "CGun.h"

BEGIN(Engine)
class UModel;
class UCollider;
END

BEGIN(Client)
class CBullet;

class CPlayerGun final : public CGun {
public:
	CPlayerGun(CSHPTRREF<UDevice> _spDevice, const _wstring& _wstrLayer, const CLONETYPE& _eCloneType);
	CPlayerGun(const CPlayerGun& _rhs);
	DESTRUCTOR(CPlayerGun)
public:
	const VECTOR < SHPTR<CBullet>>& GetBulletContainer() const { return m_BulletContainer; }
public:
	CLONE_MACRO(CPlayerGun, "CPlayerGun::Clone To Failed")
	virtual void Free() override;
	virtual HRESULT NativeConstruct() override;
	virtual HRESULT NativeConstructClone(const VOIDDATAS& _vecDatas) override;
	// Shoot Bullet 
	virtual void ShootBullet(const _float3& _vPos, const _float3& _vDir) override;
protected:
	// Tick, LateTick, Render
	virtual void TickActive(const _double& _dTimeDelta) override;
	virtual void LateTickActive(const _double& _dTimeDelta) override;
	virtual HRESULT RenderActive(CSHPTRREF<UCommand> _spCommand, CSHPTRREF<UTableDescriptor> _spTableDescriptor) override;
	// Damaged
	virtual void Collision(CSHPTRREF<UPawn> _pEnemy);
private:
	SHPTR<UModel>							m_spModel{ nullptr };
	VECTOR < SHPTR<CBullet>>		m_BulletContainer;
	constexpr static _uint					BULLET_SIZE{ 100 };
	_uint												m_iBulletIndex;
	CUSTIMER										m_ShootTimer;
};

END

#endif // _PTH_FRAMEWORK_CLIENT_PUBLIC_CPLAYERGUN_H