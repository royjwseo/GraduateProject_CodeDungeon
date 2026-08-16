#ifndef _PTH_FRAMEWORK_CLIENT_PUBLIC_CMONSTER_H
#define _PTH_FRAMEWORK_CLIENT_PUBLIC_CMONSTER_H

#include "UCharacter.h"

BEGIN(Engine)
class UModel;
class UShaderConstantBuffer;
class UTerrain;
class UCollider;
class UAnimatedParticle;
END

BEGIN(Client)
class CPlayer;

class CMonster final : public UCharacter {
public:
	CMonster(CSHPTRREF<UDevice> _spDevice, const _wstring& _wstrLayer, const CLONETYPE& _eCloneType);
	CMonster(const CMonster& _rhs);
	DESTRUCTOR(CMonster)
public:
	CSHPTRREF<UCollider> GetCollider() const { return m_spCollider; }
	CSHPTRREF<UCollider> GetFirstCheckCollider() const { return m_spFirstCheckCollider; }
public:
	CLONE_MACRO(CMonster, "CMonster::Clone To Failed")
	virtual void Free() override;
	virtual HRESULT NativeConstruct() override;
	virtual HRESULT NativeConstructClone(const VOIDDATAS& _vecDatas) override;
	virtual bool IsHit(CSHPTRREF<UPawn> _spEnemy) override;
protected:
	// Tick, LateTick, Render
	virtual void TickActive(const _double& _dTimeDelta) override;
	virtual void LateTickActive(const _double& _dTimeDelta) override;
	virtual HRESULT RenderActive(CSHPTRREF<UCommand> _spCommand, CSHPTRREF<UTableDescriptor> _spTableDescriptor) override;
	// Damaged
	virtual void Collision(CSHPTRREF<UPawn> _pEnemy) override;
private:
	SHPTR<UModel>							m_spModel;
	SHPTR< UTerrain>						m_spTerrain;
	SHPTR<CPlayer>							m_spTarget;
	SHPTR<UAnimatedParticle>		m_spAnimateParticle;
	// Prev Pos
	_float3											m_vPrevPos;
	_float												m_fSpeed;

	_float3											m_vForwardPos;
	_float3											m_vBackPos;

	SHPTR< UCollider>						m_spCollider;
	SHPTR<UCollider>						m_spFirstCheckCollider;
	_uint												m_iHitCount;
};

END

#endif // _PTH_FRAMEWORK_CLIENT_PUBLIC_CMONSTER_H