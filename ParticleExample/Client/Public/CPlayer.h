#ifndef _PTH_FRAMEWORK_CLIENT_PUBLIC_CPLAYER_H
#define _PTH_FRAMEWORK_CLIENT_PUBLIC_CPLAYER_H

#include "UCharacter.h"

BEGIN(Engine)
class UVIBufferCube;
class UTexGroup;
class UTerrain;
class UShaderConstantBuffer;
class UCollider;
END

BEGIN(Client)
class CMainCamera;
class CPlayerGun;
class CMonster;

class CPlayer final : public UCharacter {
public:
	CPlayer(CSHPTRREF<UDevice> _spDevice, const _wstring& _wstrLayer, const CLONETYPE& _eCloneType);
	CPlayer(const CPlayer& _rhs);
	DESTRUCTOR(CPlayer)
public:
	CLONE_MACRO(CPlayer, "Player::Clone To Failed")
	virtual void Free() override;
	virtual HRESULT NativeConstruct() override ;
	virtual HRESULT NativeConstructClone(const VOIDDATAS& _vecDatas) override ;
	void CollisionOthers(CSHPTRREF< CMonster> _spMonster);
protected:
	// Tick, LateTick, Render
	virtual void TickActive(const _double& _dTimeDelta) override ;
	virtual void LateTickActive(const _double& _dTimeDelta) override ;
	virtual HRESULT RenderActive(CSHPTRREF<UCommand> _spCommand, CSHPTRREF<UTableDescriptor> _spTableDescriptor) override ;
	// Damaged
	virtual void Collision(CSHPTRREF<UPawn> _pEnemy) ;
private:
	NORMALCUBEPARAM							m_stNormalCubeParam;
	_uint														m_iNormalCubeParamSize;
	SHPTR<UShaderConstantBuffer>	m_spNormalCubeShaderConstnatBuffer;
	// Cube TexGroup
	SHPTR<UVIBufferCube>					m_spVIBufferCube;
	SHPTR<UTexGroup>							m_spTexGroup;
	SHPTR<UTerrain>								m_spTerrain;
	SHPTR<CMainCamera>						m_spMainCamera;
	// 순환 참조 오류 때문에 Weak_Ptr
	WKPTR<CPlayerGun>							m_wpPlayerGun;

	_float3													m_vPrevPos;

	SHPTR< UCollider>								m_spCollider;
	SHPTR<UCollider>								m_spFirstCheckCollider;
};

END

#endif // _PTH_FRAMEWORK_CLIENT_PUBLIC_CPLAYER_H