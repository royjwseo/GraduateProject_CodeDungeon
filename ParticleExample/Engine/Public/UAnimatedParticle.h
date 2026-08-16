#ifndef _PTH_FRAMEWORK_ENGIEN_PUBLIC_UANIMATEDPARTICLE_H
#define _PTH_FRAMEWORK_ENGIEN_PUBLIC_UANIMATEDPARTICLE_H

#include "UParticle.h"

BEGIN(Engine)
class UShaderConstantBuffer;

class  ENGINE_DLL DEF_CACHE_ALGIN  UAnimatedParticle final  : public UParticle{
public:
	struct ANIMPARTICLEDESC 
	{
		_float2				vTextureSize;
		_float					fNextAnimTime;
	};
public:
	UAnimatedParticle(CSHPTRREF<UDevice> _spDevice, const _wstring& _wstrLayer, const CLONETYPE& _eCloneType);
	UAnimatedParticle(const UParticle& _rhs);
	DESTRUCTOR(UAnimatedParticle)
public:
	CLONE_MACRO(UAnimatedParticle, "UAnimatedParticle::Clone To Failed")
	virtual void Free() override;
	virtual HRESULT NativeConstruct() override;
	virtual HRESULT NativeConstructClone(const VOIDDATAS& _convecDatas) override;
protected:
	// Tick, LateTick, Render
	virtual void TickActive(const _double& _dTimeDelta) override;
	virtual void LateTickActive(const _double& _dTimeDelta) override;
	virtual HRESULT RenderActive(CSHPTRREF<UCommand> _spCommand, CSHPTRREF<UTableDescriptor> _spTableDescriptor) override;
	virtual void BindShaderBuffer() override;
	// Damaged
	virtual void Collision(CSHPTRREF<UPawn> _pEnemy) override;
	// Save Load
	virtual _bool Save(const _wstring& _wstrPath) override;
	virtual _bool Load(const _wstring& _wstrPath) override; 

	void Animated(const _double& _dTimeDelta);
#ifdef _USE_IMGUI
public:
	virtual void ShowObjectInfo() override;
#endif
private:
	_float2														m_vTextureSize;
	_float2														m_vUvPos;
	_float4x4													m_mTextureAnimation;
	CUSTIMER													m_NextAnimTimer;

	SHPTR<UShaderConstantBuffer>		m_spAnimPaticleBuffer;
	ANIMATEDPARTICLEPARAM					m_AnimParticleParam;
	constexpr static _uint								ANIMATEPARTICLE_SIZE{ sizeof(ANIMATEDPARTICLEPARAM) };
};


END

#endif // _PTH_FRAMEWORK_ENGIEN_PUBLIC_UANIMATEDPARTICLE_H