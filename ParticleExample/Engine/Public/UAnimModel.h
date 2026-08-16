#ifndef _PTH_FRAMEWORK_ENGIEN_PUBLIC_UANIMMODEL_H
#define _PTH_FRAMEWORK_ENGIEN_PUBLIC_UANIMMODEL_H

#include "UModel.h"

BEGIN(Engine)
class UShaderConstantBuffer;
class UAnimation;
class UTexture;

class ENGINE_DLL DEF_CACHE_ALGIN  UAnimModel : public UModel {
	using ANIMATIONS = VECTOR<SHPTR<UAnimation>>;
	using SETUPBONEMATRIXES = ARRAY<_float4x4, MAX_BONE_SIZE>;
	using ANIMSTRINGS = UNORMAP<_wstring, _uint>;
public:
	UAnimModel(CSHPTRREF<UDevice> _spDevice);
	UAnimModel(const UAnimModel& _rhs);
	DESTRUCTOR(UAnimModel)
public:
	const _float GetSupplyLerpValue() const { return m_fSupplyLerpValue; }
	ANIMATIONS& GetAnimations() { return m_vecAnimations; }
	const ANIMATIONPARAM& GetAnimParam() const { return m_stAnimParam; }

	void SetSupplyLerpValue(const _float _fSupplyLerpValue) { this->m_fSupplyLerpValue = _fSupplyLerpValue; }
	void SetAnimParam(const ANIMATIONPARAM& _stAnimParam){this->m_stAnimParam = _stAnimParam;}
public:
	CLONE_MACRO(UAnimModel, "UAnimModel::Clone To Failed")
	virtual void Free() override;
	virtual HRESULT NativeConstruct() override;
	using UModel::NativeConstruct;
	virtual HRESULT NativeConstructClone(const VOIDDATAS& _vecDatas) override;
	void TickAnimation(const _double& _dDeltaTime);
	virtual HRESULT Render(const _uint _iMeshIndex, CSHPTRREF<UShader> _spShader, CSHPTRREF<UCommand> _spCommand) override;
	// Set Animation
	void SetAnimation(const _uint& _iAnimIndex);
	void SetAnimation(const _wstring& _wstrAnimName);
	// Change Animation
	void ChangeAnimation(const _uint& _iAnimIndex);
	void ChangeAnimation(const _wstring& _wstrAnimName);
private:
	// CreateAnimation
	HRESULT CreateAnimation(const VECTOR<ANIMDESC>& _convecAnimDesc, const _wstring& _wstrPath);
	// Load To Data
	virtual void LoadToData(const _wstring& _wstrPath) override;
	void LoadAnimMeshData(std::ifstream& _ifRead, VECTOR<ANIMMESHDESC>& _convecMeshes);
	void LoadAnimationData(std::ifstream& _ifRead, VECTOR<ANIMDESC>& _convecAnimes);

	void ChangeAnimIndex(const _uint& _iAnimIndex, _uint& _iIndex);
	void ChangeAnimIndex(const _wstring& _wstrAnimName, _uint& _iIndex);
	void SettingCurAnimSituation();
	void SettingNextAnimSituation();
	HRESULT CreateShaderConstantBuffer();
private:
	static	const inline _wstring					SECTION_FOLDEDER_NAME{ L"Section\\"};
	static constexpr _uint								ANIMPARAM_SIZE{sizeof(ANIMATIONPARAM)};
	static constexpr _uint								BONEMATRIXPARA_SIZE{sizeof(BONEMATRIXPARAM)};

	ANIMATIONS															m_vecAnimations;
	ANIMSTRINGS														m_AnimNamesGroup;
	VECTOR<SETUPBONEMATRIXES>			m_vecSetupBonMatrix;
	// Animations
	SHPTR< UShaderConstantBuffer>					m_spAnimShaderConstantBuffer;
	ANIMATIONPARAM												m_stAnimParam;
	// Bone, PrevBone
	SHPTR<UShaderConstantBuffer>					m_spBoneMatrixShaderConstantBuffer;
	SHPTR<UShaderConstantBuffer>					m_spPrevBoneMatrixShaderConstantBuffer;
	//UpLoadResource
	ComPtr<Dx12Resource>									m_cpUpLoadResource;

	SHPTR<UAnimation>											m_spCurAnimation;
	SHPTR<UAnimation>											m_spNextAnimation;

	_uint																		m_iCurAnimIndex;
	_uint																		m_iNextAnimIndex;
	_float																		m_fSupplyLerpValue;
	_bool																		m_isChangeAnim;
};

END

#endif // _PTH_FRAMEWORK_ENGIEN_PUBLIC_UANIMMODEL_H
