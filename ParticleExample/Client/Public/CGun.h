#ifndef _PTH_FRAMEWORK_CLIENT_PUBLIC_CGUN_H
#define _PTH_FRAMEWORK_CLIENT_PUBLIC_CGUN_H

#include "UPawn.h"

BEGIN(Engine)
class UModel;
class UShaderConstantBuffer;
END

BEGIN(Client)
class CPlayer;
class CBullet;

class CGun : public UPawn {
public:
	CGun(CSHPTRREF<UDevice> _spDevice, const _wstring& _wstrLayer, const CLONETYPE& _eCloneType);
	CGun(const CGun& _rhs);
	DESTRUCTOR(CGun)
public:
	CSHPTRREF<UPawn> GetOwner() const { return m_spOwner; }
	void SetOwner(CSHPTRREF<UPawn> _spPawn);
public:
	CLONE_MACRO(CGun, "CGun::Clone To Failed")
	virtual void Free() override;
	virtual HRESULT NativeConstruct() override;
	virtual HRESULT NativeConstructClone(const VOIDDATAS& _vecDatas) override;
	// Shoot Bullet 
	virtual void ShootBullet(const _float3& _vPos, const _float3& _vDir);
protected:
	// Tick, LateTick, Render
	virtual void TickActive(const _double& _dTimeDelta) override;
	virtual void LateTickActive(const _double& _dTimeDelta) override;
	virtual HRESULT RenderActive(CSHPTRREF<UCommand> _spCommand, CSHPTRREF<UTableDescriptor> _spTableDescriptor) override;
	// Damaged
	virtual void Collision(CSHPTRREF<UPawn> _pEnemy);
	HRESULT CreateSocketMatirxBuffer(const _uint _iSize);
	void BindSocketMatrixBuffer();
private:
	constexpr static _uint							SOCKETMATRIXPARAM_SIZE{ sizeof(SOCKETMATRIXPARAM) };
	SOCKETMATRIXPARAM						m_SocketMatrixParam;

	SHPTR<UShaderConstantBuffer>	m_spSocketMatrixBuffer;
	SHPTR<UPawn>									m_spOwner;
};

END

#endif // _PTH_FRAMEWORK_CLIENT_PUBLIC_CGUN_H