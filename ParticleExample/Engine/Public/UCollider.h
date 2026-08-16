#ifndef _PTH_FRAMEWORK_ENGIEN_PUBLIC_UCOLLIDER_H
#define _PTH_FRAMEWORK_ENGIEN_PUBLIC_UCOLLIDER_H

#include "UComponent.h"

BEGIN(Engine)
class UTransform;
class URenderer;
class UPawn;
class UCommand;
class UTableDescriptor;
class UDefaultDebugging;

class ENGINE_DLL DEF_CACHE_ALGIN  UCollider : public UComponent {
public:
	enum TYPE { TYPE_AABB, TYPE_OBB, TYPE_SPHERE, TYPE_END };
	typedef struct tagColliderDesc
	{
		_float3		vScale;
		_float3		vTranslation;
	}COLLIDERDESC;

	UCollider(CSHPTRREF <UDevice> _pDevice, const TYPE& _eType);
	UCollider(const UCollider& _rhs);
	DESTRUCTOR(UCollider)
public:
	const _float3& GetCurPos();
	const _float3& GetScale();
	const _float3& GetTranslate();
	const _float4x4& GetTransformMatrix() const { return m_mTransformMatrix; }

	void SetScale(const _float3& _vScale);
	void SetTranslate(const _float3& _vTranslate);
	void SetTransform(const _float3& _vPos, const _float4& _vQuaternion);
	void SetTransform(CSHPTRREF< UTransform> _spTransform);
	// UComponent을(를) 통해 상속됨
	virtual void Free() override;
	CLONE_MACRO(UCollider, "UCollider::Clone To Failed")
	virtual HRESULT NativeConstruct() override;
	virtual HRESULT NativeConstructClone(const VOIDDATAS& _tDatas) override;
	_bool IsCollision(CSHPTRREF<UCollider> _pCollider);
#ifdef _USE_DEBUGGING
	void AddRenderer(RENDERID _eID);
#endif
private:
	SHPTR<DirectX::BoundingBox>					m_spAABB_Original;
	SHPTR<DirectX::BoundingOrientedBox>	m_spOBB_Original;
	SHPTR<DirectX::BoundingSphere>				m_spSphere_Original;

	SHPTR<DirectX::BoundingBox>					m_spAABB;
	SHPTR<DirectX::BoundingOrientedBox>	m_spOBB;
	SHPTR<DirectX::BoundingSphere>				m_spSphere;

	TYPE																m_eType;
	_bool																m_isCollision;
	_float4x4														m_mTransformMatrix;

	_float3															m_vScale;
	_float3															m_vTranslate;
	_float3															m_vPos;

#ifdef _USE_DEBUGGING
	SHPTR<UDefaultDebugging>					m_spDebugDrawPawn;
#endif
};

END


#endif // _PTH_FRAMEWORK_ENGIEN_PUBLIC_UCOLLIDER_H