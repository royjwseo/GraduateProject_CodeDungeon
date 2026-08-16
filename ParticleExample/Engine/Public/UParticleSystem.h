#ifndef _PTH_FRAMEWORK_ENGIEN_PUBLIC_UPARTICLESYSTEM_H
#define _PTH_FRAMEWORK_ENGIEN_PUBLIC_UPARTICLESYSTEM_H

#include "UResource.h"

BEGIN(Engine)
class UShaderStructedBuffer;
class UShaderConstantBuffer;
class UComputeShader;
class UShader;
class UComputeCommand;
class UTableDescriptor;
class UCommand ;

class ENGINE_DLL DEF_CACHE_ALGIN  UParticleSystem : public UResource {
public:
	UParticleSystem(CSHPTRREF<UDevice> _spDevice);
	UParticleSystem(const UParticleSystem& _rhs);
	DESTRUCTOR(UParticleSystem)
public:
	 const _uint GetMaxParticleCnt()  { return m_iMaxParitcleCnt; }
public:
	CLONE_MACRO(UParticleSystem, "UParticleSystem::Clone To Failed")
	virtual void Free() override;
	virtual HRESULT NativeConstruct() override;
	virtual HRESULT NativeConstructClone(const VOIDDATAS& _convecDatas) override;
public:
	void Update(const _double& _dTimeDelta);
	void Render();
	void BindShaderParams(CSHPTRREF<UShader> _spShader);
	// Bind Compute shader
	void SettingComputeShader(const _wstring& _wstrProtoName);
	// SaveLoada
	_bool Load(const _wstring& _wstrPath);
	_bool Save(const _wstring& _wstrPath);
#ifdef _USE_IMGUI
public:
	virtual void ShowObjectInfo() override;
#endif 
private:
	// static 
	static constexpr _uint										PARTICLEPARAM_SIZE{sizeof(PARTICLEPARAM)};
	static constexpr _float									CREATE_INTERVAL{0.05f};
	static constexpr _ushort								COMPUTE_MAX_INDEX{3};
	using COMPUTECOMMANDCONTAINER = ARRAY<SHPTR<UComputeCommand>, COMPUTE_MAX_INDEX>;

	_uint																m_iMaxParitcleCnt;
	PARTICLEPARAM											m_stParticleParam;
	PARTICLEPARAM											m_stRenderParticleParam;
	SHPTR< UShaderConstantBuffer>			m_spUpdateParticleConstnatBuffer;
	SHPTR< UShaderConstantBuffer>			m_spRenderParticleConstnatBuffer;
	SHPTR<UShaderStructedBuffer>			m_spParticleStructedBuffer;
	SHPTR<UShaderStructedBuffer>			m_spComputeShaderStructedBuffer;
	// ComputeShader
	SHPTR<UComputeShader>						m_spComputeShader;
	COMPUTECOMMANDCONTAINER				m_arrComputeCommands;
	SHPTR< UTableDescriptor>						m_spComputeTableDescriptor;
	_ushort															m_sComputeIndex;
};

END

#endif // _PTH_FRAMEWORK_ENGIEN_PUBLIC_UPARTICLESYSTEM_H