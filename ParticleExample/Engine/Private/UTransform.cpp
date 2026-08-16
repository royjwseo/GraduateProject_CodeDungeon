#include "EngineDefines.h"
#include "UTransform.h"
#include "UMethod.h"
#include "UShaderConstantBuffer.h"
#include "UGameInstance.h"
#include "UShader.h"

constexpr _uint							UTransform::TRANSFORMPARAM_SIZE;

UTransform::UTransform(CSHPTRREF<UDevice> _spDevice) :
	UComponent(_spDevice),
	// Shader 
m_spWaterShaderBuffer{ nullptr },
m_stTransformParam{},
m_mWorldMatrix{ _float4x4::Identity },
m_mChangeWorldMatrix{ _float4x4::Identity },
m_vQuaternion{ _float4::Zero },
m_vScale{ 1.f, 1.f, 1.f },
m_isNotApplyRotate{ false },
m_isNotApplyPos{ false },
m_isNotApplyScale{ false },
m_spParentsTransform{nullptr}
{
}

UTransform::UTransform(const UTransform& _rhs) : 
	UComponent(_rhs)
{
}

const _float3 UTransform::GetAngles() const
{
	_float3 vEuler = m_vQuaternion.ToEuler();
	vEuler.x = DirectX::XMConvertToDegrees(vEuler.x);
	vEuler.y = DirectX::XMConvertToDegrees(vEuler.y);
	vEuler.z = DirectX::XMConvertToDegrees(vEuler.z);
	return vEuler;
}

const _float UTransform::GetXAngle() const
{
	return DirectX::XMConvertToDegrees(m_vQuaternion.ToEuler().x);
}

const _float UTransform::GetYAngle() const
{
	return DirectX::XMConvertToDegrees(m_vQuaternion.ToEuler().y);
}

const _float UTransform::GetZAngle() const
{
	return DirectX::XMConvertToDegrees(m_vQuaternion.ToEuler().z);
}

void UTransform::SetScale(const _float3& _vScale)
{
	if (_vScale == m_vScale)
		return;

	m_mWorldMatrix = m_mWorldMatrix.MatrixSetScaling(_vScale);
	m_vScale = _vScale;
}

const _float4x4 UTransform::GetParentsMatrix()
{
	if (nullptr == m_spParentsTransform)
		return GetWorldMatrix();

	return _float4x4();
}

void UTransform::SetParent(CSHPTRREF<UTransform> _spTransform)
{
	RETURN_CHECK(nullptr == _spTransform, ;);
	RETURN_CHECK(this == _spTransform.get(), ;);
	m_spParentsTransform = _spTransform;
}

void UTransform::Free()
{
	m_spParentsTransform.reset();
}

HRESULT UTransform::NativeConstruct()
{
    RETURN_CHECK_FAILED(__super::NativeConstruct(), E_FAIL);
	SHPTR<UGameInstance> spGameInstance = GET_INSTANCE(UGameInstance);
	// 이미 만들어진 Shader ConstnatBuffer를 가져옴
	spGameInstance->GetPreAllocatedConstantBuffer(PREALLOCATED_TRANSFORM, m_spWaterShaderBuffer);
	return S_OK;
}

HRESULT UTransform::NativeConstructClone(const VOIDDATAS& _stDatas)
{
	return __super::NativeConstructClone(_stDatas);
}

void UTransform::TransformUpdate()
{
    if (nullptr !=  m_spParentsTransform) {

		_float4x4 Matrix = m_spParentsTransform->GetParentsMatrix();
		if (true == m_isNotApplyScale)
		{
			Matrix.MatrixSetScaling(BASIC_SCALE);
		}
		if (true == m_isNotApplyPos)
		{
			Matrix.Set_Up(_float3::Zero);
		}
		if (true == m_isNotApplyRotate)
		{
			Matrix.MatrixSetRotationFix(_float3::Zero);
		}
		m_mChangeWorldMatrix = m_mWorldMatrix * Matrix;
    }
    else {
        m_mChangeWorldMatrix = m_mWorldMatrix;
    }
}

void UTransform::MoveForward(const _double& _dTimeDelta, const _float& _fSpeed)
{
	_float3 vPosition = GetPos();
	_float3 vLook = GetLook();

	vPosition += vLook * static_cast<_float>(_dTimeDelta) * _fSpeed;
	SetPos(vPosition);
}

void UTransform::MoveBack(const _double& _dTimeDelta, const _float& _fSpeed)
{
	_float3 vPosition = GetPos();
	_float3 vLook = GetLook();

	vPosition -= vLook * static_cast<_float>(_dTimeDelta) * _fSpeed;
	SetPos(vPosition);
}

void UTransform::MoveLeft(const _double& _dTimeDelta, const _float& _fSpeed)
{
	_float3 vPosition = GetPos();
	_float3 vLook = GetRight();

	vPosition -= vLook * static_cast<_float>(_dTimeDelta) * _fSpeed;
	SetPos(vPosition);
}

void UTransform::MoveRight(const _double& _dTimeDelta, const _float& _fSpeed)
{
	_float3 vPosition = GetPos();
	_float3 vLook = GetRight();

	vPosition += vLook * static_cast<_float>(_dTimeDelta) * _fSpeed;
	SetPos(vPosition);
}

void UTransform::MoveForwardNotY(const _double& _dTimeDelta, const _float& _fSpeed)
{
	_float3 vPosition = GetPos();
	_float3 vLook = GetLook();

	vPosition += vLook * static_cast<_float>(_dTimeDelta) * _fSpeed;
	vPosition.y = 0.f;
	SetPos(vPosition);
}

void UTransform::MoveBackNotY(const _double& _dTimeDelta, const _float& _fSpeed)
{
	_float3 vPosition = GetPos();
	_float3 vLook = GetLook();

	vPosition -= vLook * static_cast<_float>(_dTimeDelta) * _fSpeed;
	vPosition.y = 0.f;
	SetPos(vPosition);
}

void UTransform::MoveLeftNotY(const _double& _dTimeDelta, const _float& _fSpeed)
{
	_float3 vPosition = GetPos();
	_float3 vLook = GetRight();

	vPosition -= vLook * static_cast<_float>(_dTimeDelta) * _fSpeed;
	vPosition.y = 0.f;
	SetPos(vPosition);
}

void UTransform::MoveRightNotY(const _double& _dTimeDelta, const _float& _fSpeed)
{
	_float3 vPosition = GetPos();
	_float3 vLook = GetLook();

	vPosition -= vLook * static_cast<_float>(_dTimeDelta) * _fSpeed;
	vPosition.y = 0.f;
	SetPos(vPosition);
}

void UTransform::TranslatePos(const _float3& _vPos, const _double& _dTimeDelta, const _float& _fSpeed, const _float& _fLimitDistance)
{
	if (ComputeDistance(_vPos) <= _fLimitDistance * _fLimitDistance) {
		_float3 vPosition = GetPos();
		_float3 vLook = _vPos - vPosition;

		vPosition += vLook * static_cast<_float>(_dTimeDelta) * _fSpeed;
		SetPos(vPosition);
	}
}

void UTransform::TranslateDir(const _float3& _vDir, const _double& _dTimeDelta, const _float& _fSpeed)
{
	_float3 vPosition = GetPos();

	vPosition += _vDir * static_cast<_float>(_dTimeDelta) * _fSpeed;
	SetPos(vPosition);
}

void UTransform::TranslateTrans(CSHPTRREF<UTransform> _pTransform, const _double& _dTimeDelta, const _float& _fSpeed, const _float& _fLimitDistance)
{
	RETURN_CHECK(nullptr == _pTransform, ; );
	const _float3& _vPos = _pTransform->GetPos();
	if (ComputeDistance(_vPos) <= _fLimitDistance * _fLimitDistance) {
		_float3 vPosition = GetPos();
		_float3 vLook = _vPos - vPosition;

		vPosition += vLook * static_cast<_float>(_dTimeDelta) * _fSpeed;
		SetPos(vPosition);
	}
}

void UTransform::TranslatePosNotY(const _float3& _vPos, const _double& _dTimeDelta, const _float& _fSpeed, const _float& _fLimitDistance)
{
	if (ComputeDistance(_vPos) <= _fLimitDistance * _fLimitDistance) {
		_float3 vPosition = GetPos();
		_float3 vLook = _vPos - vPosition;

		vPosition += vLook * static_cast<_float>(_dTimeDelta) * _fSpeed;
		vLook.y = 0;
		SetPos(vPosition);
	}
}

void UTransform::TranslateDirNotY(const _float3& _vDir, const _double& _dTimeDelta, const _float& _fSpeed)
{
	_float3 vPosition = GetPos();

	vPosition += _vDir * static_cast<_float>(_dTimeDelta) * _fSpeed;
	vPosition.y = 0;
	SetPos(vPosition);
}

void UTransform::TranslateTransNotY(CSHPTRREF<UTransform> _pTransform, const _double& _dTimeDelta, const _float& _fSpeed, const _float& _fLimitDistance)
{
	RETURN_CHECK(nullptr == _pTransform, ; );
	const _float3& _vPos = _pTransform->GetPos();
	if (ComputeDistance(_vPos) <= _fLimitDistance * _fLimitDistance) {
		_float3 vPosition = GetPos();
		_float3 vLook = _vPos - vPosition;

		vPosition += vLook * static_cast<_float>(_dTimeDelta) * _fSpeed;
		vPosition.y = 0.f;
		SetPos(vPosition);
	}
}

void UTransform::RotateFix(const _float3& _vStandardAngle, const _float _fTurnAnge)
{
	m_vQuaternion = _quaternion::CreateFromAxisAngle(_vStandardAngle, DirectX::XMConvertToRadians(_fTurnAnge));

	_float4x4 RotationMatrix = _float4x4::CreateFromQuaternion(m_vQuaternion);

	SetRight(DirectX::XMVector3TransformNormal(_float3{1.f, 0.f, 0.f} *m_vScale.x, RotationMatrix));
	SetUp(DirectX::XMVector3TransformNormal(_float3{ 0.f, 1.f, 0.f } *m_vScale.y, RotationMatrix));
	SetLook(DirectX::XMVector3TransformNormal(_float3{0.f, 0.f, 1.f } *m_vScale.z, RotationMatrix));
}

void UTransform::RotateFix(const _float3& _vAngle)
{
	_float3 vAngle;
	vAngle.x = DirectX::XMConvertToRadians(_vAngle.x);
	vAngle.y = DirectX::XMConvertToRadians(_vAngle.y);
	vAngle.z = DirectX::XMConvertToRadians(_vAngle.z);
	m_vQuaternion = _quaternion::CreateFromYawPitchRoll(vAngle);
	
	_float4x4 RotationMatrix = _float4x4::CreateFromQuaternion(m_vQuaternion);

	SetRight(DirectX::XMVector3TransformNormal(_float3{ 1.f, 0.f, 0.f } *m_vScale.x, RotationMatrix));
	SetUp(DirectX::XMVector3TransformNormal(_float3{ 0.f, 1.f, 0.f } *m_vScale.y, RotationMatrix));
	SetLook(DirectX::XMVector3TransformNormal(_float3{ 0.f, 0.f, 1.f } *m_vScale.z, RotationMatrix));
}

void UTransform::RotateFix(const _float4& _vAngle)
{
	m_vQuaternion = _vAngle;

	_float4x4 RotationMatrix = _float4x4::CreateFromQuaternion(m_vQuaternion);

	SetRight(DirectX::XMVector3TransformNormal(_float3{ 1.f, 0.f, 0.f } *m_vScale.x, RotationMatrix));
	SetUp(DirectX::XMVector3TransformNormal(_float3{ 0.f, 1.f, 0.f } *m_vScale.y, RotationMatrix));
	SetLook(DirectX::XMVector3TransformNormal(_float3{ 0.f, 0.f, 1.f } *m_vScale.z, RotationMatrix));
}

void UTransform::RotateTurn(const _float3& _vAxis, const _float& _fAngleSpeed, const _double& _dTimeDelta)
{
	m_vQuaternion = DirectX::XMQuaternionRotationAxis(_vAxis, (_float)(DirectX::XMConvertToRadians(_fAngleSpeed) * _dTimeDelta));
	_float4x4 RotationMatrix = DirectX::XMMatrixRotationQuaternion(m_vQuaternion);

	SetRight(DirectX::XMVector3TransformNormal(GetRight(), RotationMatrix));
	SetUp(DirectX::XMVector3TransformNormal(GetUp(), RotationMatrix));
	SetLook(DirectX::XMVector3TransformNormal(GetLook(), RotationMatrix));
}

void UTransform::RotateTurn(const _float3& _vAxis, const _float& _fAngle)
{
	m_vQuaternion = DirectX::XMQuaternionRotationAxis(_vAxis, (_float)(DirectX::XMConvertToRadians(_fAngle)));
	_float4x4 RotationMatrix = DirectX::XMMatrixRotationQuaternion(m_vQuaternion);

	SetRight(DirectX::XMVector3TransformNormal(GetRight(), RotationMatrix));
	SetUp(DirectX::XMVector3TransformNormal(GetUp(), RotationMatrix));
	SetLook(DirectX::XMVector3TransformNormal(GetLook(), RotationMatrix));
}

void UTransform::RotateTurn(const _float3& _vAngle)
{
	_float3 vAngle;
	vAngle.x = DirectX::XMConvertToRadians(_vAngle.x);
	vAngle.y = DirectX::XMConvertToRadians(_vAngle.y);
	vAngle.z = DirectX::XMConvertToRadians(_vAngle.z);
	m_vQuaternion = _quaternion::CreateFromYawPitchRoll(vAngle);

	_float4x4 RotationMatrix = _float4x4::CreateFromQuaternion(m_vQuaternion);

	SetRight(DirectX::XMVector3TransformNormal(GetRight(), RotationMatrix));
	SetUp(DirectX::XMVector3TransformNormal(GetUp(), RotationMatrix));
	SetLook(DirectX::XMVector3TransformNormal(GetLook(), RotationMatrix));
}

void UTransform::LookAt(const _float3& _vTargetPos)
{
	_float3 vPosition = GetPos();
	_float4 vLook = XMVector3Normalize(_vTargetPos - vPosition);

	_float4 vRight = DirectX::XMVector3Normalize(XMVector3Cross(_float3::Up, vLook));
	_float4 vUp = DirectX::XMVector3Normalize(XMVector3Cross(vLook, vRight));

	// 회전 행렬 계산
	_float4x4 rotationMatrix(
		vRight,
		vUp,
		vLook,
		_float4(0.0f, 0.0f, 0.0f, 1.0f)
	);

	m_vQuaternion = DirectX::XMQuaternionRotationMatrix(rotationMatrix);
	_float4x4 RotationMatrix = DirectX::XMMatrixRotationQuaternion(m_vQuaternion);

	SetRight(DirectX::XMVector3TransformNormal(_float3::Right * m_vScale.x, RotationMatrix));
	SetUp(DirectX::XMVector3TransformNormal(_float3::Up * m_vScale.y, RotationMatrix));
	SetLook(DirectX::XMVector3TransformNormal(_float3::Forward * m_vScale.z, RotationMatrix));
}

const _float UTransform::ComputeDistance(const _float3& _vPos)
{
	return _float3::Length(GetPos() - _vPos);
}

const _float UTransform::ComputeDistanceSq(const _float3& _vPos)
{
	return _float3::LengthSquared(GetPos() - _vPos);
}

HRESULT UTransform::BindTransformData(CSHPTRREF< UShader> _spShader, const CAMID _iCamID)
{
	RETURN_CHECK(nullptr == _spShader, E_FAIL);
	RETURN_CHECK(nullptr == m_spWaterShaderBuffer, E_FAIL);
	TransformUpdate();
	m_stTransformParam.iCamIndex = _iCamID;
	m_stTransformParam.mPrevWorldMatrix = m_stTransformParam.mWorldMatrix;
	m_stTransformParam.mWorldMatrix = m_mChangeWorldMatrix.Transpose();
	return _spShader->BindCBVBuffer(m_spWaterShaderBuffer, &m_stTransformParam, TRANSFORMPARAM_SIZE);
}

HRESULT UTransform::BindTransformData(CSHPTRREF<UShader> _spShader, const _float4x4& _mTransform, const CAMID _iCamID)
{
	RETURN_CHECK(nullptr == _spShader, E_FAIL);
	RETURN_CHECK(nullptr == m_spWaterShaderBuffer, E_FAIL);
	m_stTransformParam.iCamIndex = _iCamID;
	m_stTransformParam.mPrevWorldMatrix = _mTransform;
	m_stTransformParam.mWorldMatrix = m_mChangeWorldMatrix.Transpose();
	return _spShader->BindCBVBuffer(m_spWaterShaderBuffer, &m_stTransformParam, TRANSFORMPARAM_SIZE);
}

#ifdef _USE_IMGUI
void UTransform::ShowObjectInfo()
{
	static const char* TransformName{ "Transform" };
	static const char* PositionName{ "Position" };
	static const char* ScaleName{ "Scale" };
	static const char* Rotation{ "Rotation" };
	static const char* ParentsName{ "Parents_Transform" };
	if (ImGui::TreeNode(TransformName))
	{
		_float3 vPos = GetPos();
		ImGui::DragFloat3(PositionName, &vPos.x, 0.1f);
		if (true == ImGui::DragFloat3(ScaleName, &m_vScale.x, 0.1f))
		{
			m_mWorldMatrix.MatrixSetScaling(m_vScale);
		}
		_float3 vRotation = m_vQuaternion.ToEuler();
		if (true == ImGui::DragFloat3(Rotation, &vRotation.x, 0.1f))
		{
			RotateFix(vRotation);
		}
		if (nullptr != m_spParentsTransform)
		{
			ImGui::Text(ParentsName);
			m_spParentsTransform->ShowObjectInfo();
		}
		ImGui::TreePop();
	}
}
#endif