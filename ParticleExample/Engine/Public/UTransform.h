#ifndef _PTH_FRAMEWORK_ENGIEN_PUBLIC_UTRANSFORM_H
#define _PTH_FRAMEWORK_ENGIEN_PUBLIC_UTRANSFORM_H

#include "UComponent.h"

BEGIN(Engine)
class UShaderConstantBuffer;
class UShader;

class ENGINE_DLL  DEF_CACHE_ALGIN  UTransform : public UComponent {
public:
	UTransform(CSHPTRREF<UDevice> _spDevice);
	UTransform(const UTransform& _rhs);
	DESTRUCTOR(UTransform)
public:
	const _float3& GetScale() { return m_vScale; }
	const _float3 GetAngles() const;
	const _float GetXAngle() const;
	const _float GetYAngle() const;
	const _float GetZAngle() const;
	const _quaternion& GetRotation() const { return m_vQuaternion; }
	const _float4x4 GetWorldMatrixTP() { TransformUpdate();  return XMMatrixTranspose(XMLoadFloat4x4(&m_mChangeWorldMatrix)); }
	const _float4x4 GetWorldMatrixInv()  { TransformUpdate();   return XMMatrixInverse(nullptr, XMLoadFloat4x4(&m_mChangeWorldMatrix)); }
	const _float4x4& GetWorldMatrix() const { return m_mWorldMatrix; }
	const _float4x4& GetChangeMatrix() { TransformUpdate();   return m_mChangeWorldMatrix; }
	// Get Parents Local Matrix
	const _float4x4 GetParentsMatrix();

	const _float3& GetRight() const { return *((_float3*)&m_mWorldMatrix.m[DirectX::PTH::MATROW_RIGHT][0]); }
	const _float3& GetUp() const { return *((_float3*)&m_mWorldMatrix.m[DirectX::PTH::MATROW_UP][0]); }
	const _float3& GetLook() const { return *((_float3*)&m_mWorldMatrix.m[DirectX::PTH::MATROW_LOOK][0]); }
	const _float3& GetPos() const { return *((_float3*)&m_mWorldMatrix.m[DirectX::PTH::MATROW_POS][0]); }

	const _float3& GetPrevPos() const { return  *((_float3*)&m_stTransformParam.mPrevWorldMatrix.m[DirectX::PTH::MATROW_POS][0]); }

	const _float3& GetChangeRight() const { return *((_float3*)&m_mChangeWorldMatrix.m[DirectX::PTH::MATROW_RIGHT][0]); }
	const _float3& GetChangeUp() const { return *((_float3*)&m_mChangeWorldMatrix.m[DirectX::PTH::MATROW_UP][0]); }
	const _float3& GetChangeLook() const { return *((_float3*)&m_mChangeWorldMatrix.m[DirectX::PTH::MATROW_LOOK][0]); }
	const _float3& GetChangePos() const { return *((_float3*)&m_mChangeWorldMatrix.m[DirectX::PTH::MATROW_POS][0]); }

	const SHPTR<UTransform>& GetParentsTransform() const { return m_spParentsTransform; }

	void SetScale(const _float3& _vScale);
	void SetParent(CSHPTRREF<UTransform> _spTransform);

	void SetRight(const _float3& _vRight) { m_mWorldMatrix.Set_Right(_vRight); }
	void SetUp(const _float3& _vUp) { m_mWorldMatrix.Set_Up(_vUp); }
	void SetLook(const _float3& _vLook) { m_mWorldMatrix.Set_Look(_vLook); }
	void SetPos(const _float3& _vPos) { m_mWorldMatrix.Set_Pos(_vPos); }

	void SetNotApplyRotate(const _bool _isActive) { this->m_isNotApplyRotate = _isActive; }
	void SetNotApplyPos(const _bool _isActive) { this->m_isNotApplyPos = _isActive; }
	void SetNotApplyScale(const _bool _isActive) { this->m_isNotApplyScale = _isActive; }

public:
	virtual void Free() override;
	CLONE_MACRO(UTransform, "UTransform::Cloen To Failed")
public:
	// Native Construct 
	virtual HRESULT NativeConstruct() override;
	// Clone 
	virtual HRESULT NativeConstructClone(const VOIDDATAS& _stDatas) override;

	void TransformUpdate();

	// Move 
	void MoveForward(const _double& _dTimeDelta, const _float& _fSpeed);
	void MoveBack(const _double& _dTimeDelta, const _float& _fSpeed);
	void MoveLeft(const _double& _dTimeDelta, const _float& _fSpeed);
	void MoveRight(const _double& _dTimeDelta, const _float& _fSpeed);
	// Move Not Y
	void MoveForwardNotY(const _double& _dTimeDelta, const _float& _fSpeed);
	void MoveBackNotY(const _double& _dTimeDelta, const _float& _fSpeed);
	void MoveLeftNotY(const _double& _dTimeDelta, const _float& _fSpeed);
	void MoveRightNotY(const _double& _dTimeDelta, const _float& _fSpeed);
	// Translate To Pos
	void TranslatePos(const _float3& _vPos, const _double& _dTimeDelta, const _float& _fSpeed,
		const _float& _fLimitDistance = 0.1f);
	void TranslateDir(const _float3& _vDir, const _double& _dTimeDelta, const _float& _fSpeed);
	void TranslateTrans(CSHPTRREF<UTransform> _pTransform, const _double& _dTimeDelta, const _float& _fSpeed,
		const _float& _fLimitDistance = 0.1f);
	// Translate To Pos Not Y
	void TranslatePosNotY(const _float3& _vPos, const _double& _dTimeDelta, const _float& _fSpeed,
		const _float& _fLimitDistance = 0.1f);
	void TranslateDirNotY(const _float3& _vDir, const _double& _dTimeDelta, const _float& _fSpeed);
	void TranslateTransNotY(CSHPTRREF<UTransform> _pTransform, const _double& _dTimeDelta, const _float& _fSpeed,
		const _float& _fLimitDistance = 0.1f);

	void RotateFix(const _float3& _vStandardAngle, const _float _fTurnAnge);
	// Just Angle
	void RotateFix(const _float3& _vAngle);
	// Quaternion
	void RotateFix(const _float4& _vAngle);
	void RotateTurn(const _float3& _vAxis, const _float& _fAngleSpeed, const _double& _dTimeDelta);
	void RotateTurn(const _float3& _vAxis, const _float& _fAngle);
	void RotateTurn(const _float3& _vAngle);
	void LookAt(const _float3& _vTargetPos);
	// Compute Distance
	const _float ComputeDistance(const _float3& _vPos);
	const _float ComputeDistanceSq(const _float3& _vPos);
	// Shader 파일을 불러와서 바인드 하는 함수
	HRESULT BindTransformData(CSHPTRREF< UShader> _spShader, const CAMID _iCamID = MAIN_CAMERA_ID);
	// Shader 파일을 불러와서 바인드 하는 함수
	HRESULT BindTransformData(CSHPTRREF< UShader> _spShader, const _float4x4& _mTransform, const CAMID _iCamID = MAIN_CAMERA_ID);
#ifdef _USE_IMGUI
public:
	virtual void ShowObjectInfo() override;
#endif
public:
	// Transform Param의 Static value
	static constexpr _uint							TRANSFORMPARAM_SIZE{sizeof(TRANSFORMPARAM)};
private:
	// Shader 
	SHPTR<UShaderConstantBuffer>	m_spWaterShaderBuffer{ nullptr };
	TRANSFORMPARAM							m_stTransformParam{};
	// Needs Value 
	_float4x4												m_mWorldMatrix{ _float4x4::Identity };
	_float4x4												m_mChangeWorldMatrix{ _float4x4::Identity };
	_quaternion											m_vQuaternion{ _float4::Zero };
	_float3													m_vScale{ _float3::Zero };
	// Parents Location
	_bool														m_isNotApplyRotate{ false };
	_bool														m_isNotApplyPos{ false };
	_bool														m_isNotApplyScale{ false };

	SHPTR<UTransform>							m_spParentsTransform{nullptr};
};

END

#endif // _PTH_FRAMEWORK_ENGIEN_PUBLIC_UTRANSFORM_H