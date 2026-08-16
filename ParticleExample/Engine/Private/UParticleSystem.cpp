#include "EngineDefines.h"
#include "UParticleSystem.h"
#include "UGameInstance.h"
#include "UMethod.h"
#include "UShaderStructedBuffer.h"
#include "UShaderConstantBuffer.h"
#include <fstream>
#include "UComputeShader.h"
#include "UComputeCommand.h"
#include "UComputeTableDescriptor.h"
#include "UDevice.h"
#include "ThreadMethods.h"

UParticleSystem::UParticleSystem(CSHPTRREF<UDevice> _spDevice) :
	UResource(_spDevice),
	m_iMaxParitcleCnt{ 1000 },
	m_stParticleParam{},
	m_spUpdateParticleConstnatBuffer{ nullptr },
	m_spParticleStructedBuffer{ nullptr },
	m_spComputeShaderStructedBuffer{ nullptr },
	m_spComputeShader{ nullptr },
	m_arrComputeCommands{ nullptr },
	m_sComputeIndex{ 0 }
{
}

UParticleSystem::UParticleSystem(const UParticleSystem& _rhs) : 
	UResource(_rhs), 
	m_iMaxParitcleCnt{ 1000 },
	m_stParticleParam{},
	m_spUpdateParticleConstnatBuffer{ nullptr },
	m_spParticleStructedBuffer{ nullptr },
	m_spComputeShaderStructedBuffer{ nullptr },
	m_spComputeShader{ nullptr },
	m_arrComputeCommands{ nullptr },
	m_sComputeIndex{ 0 }
{
}

void UParticleSystem::Free()
{
}

HRESULT UParticleSystem::NativeConstruct()
{
	RETURN_CHECK_FAILED(__super::NativeConstruct(), E_FAIL);
	return S_OK;
}

HRESULT UParticleSystem::NativeConstructClone(const VOIDDATAS& _vecDatas)
{
	RETURN_CHECK_FAILED(__super::NativeConstructClone(_vecDatas), E_FAIL);
	// Particle Param Temporary Define
	{
		if (_vecDatas.size() > 0)
		{
			m_stParticleParam = UMethod::ConvertTemplate_Index<PARTICLEPARAM>(_vecDatas, 0);
		}
	}

	m_spUpdateParticleConstnatBuffer = CreateNative<UShaderConstantBuffer>(GetDevice(), CBV_REGISTER::ALLPARTICLEBUFFER, PARTICLEPARAM_SIZE);

	m_spRenderParticleConstnatBuffer = CreateNative<UShaderConstantBuffer>(GetDevice(), CBV_REGISTER::ALLPARTICLEBUFFER, PARTICLEPARAM_SIZE);

	m_spParticleStructedBuffer = CreateNative<UShaderStructedBuffer>(GetDevice(), sizeof(PARTICLE), m_iMaxParitcleCnt);
	m_spComputeShaderStructedBuffer = CreateNative<UShaderStructedBuffer>(GetDevice(), sizeof(COMPUTESHADERINFO), 1);

	SHPTR<UGameInstance> spGameInstance = GET_INSTANCE(UGameInstance);
	{
		ComPtr<Dx12CommandQueue> cpCommandQueue = UMethod::CreateCommandQueue(GetDevice()->GetDV(), D3D12_COMMAND_LIST_TYPE_COMPUTE);

		for (auto& iter : m_arrComputeCommands)
		{
			iter = CreateNative<UComputeCommand>(GetDevice(), cpCommandQueue);
		}

		m_spComputeTableDescriptor = CreateNative<UComputeTableDescriptor>(GetDevice(), 1);
	}
	return S_OK;
}

void UParticleSystem::Update(const _double& _dTimeDelta)
{
	RETURN_CHECK(nullptr == m_spComputeShader, ;)
	{
		m_stParticleParam.stGrobalParticleInfo.fDeltaTime = static_cast<_float>(_dTimeDelta);
		m_stParticleParam.stGrobalParticleInfo.fAccTime += m_stParticleParam.stGrobalParticleInfo.fDeltaTime;

		if (CREATE_INTERVAL < m_stParticleParam.stGrobalParticleInfo.fAccTime)
		{
			m_stParticleParam.stGrobalParticleInfo.fAccTime = m_stParticleParam.stGrobalParticleInfo.fAccTime - CREATE_INTERVAL;
			m_stParticleParam.stGrobalParticleInfo.iAddCount = 2;
		}
	}
}
 
void UParticleSystem::Render()
{
	m_spComputeShader->SetTableDescriptor(m_spComputeTableDescriptor);
	m_spComputeShader->BeginSettingDatas(m_arrComputeCommands[m_sComputeIndex]);

	m_spComputeShader->BindCBVBuffer(m_spUpdateParticleConstnatBuffer, &m_stParticleParam, PARTICLEPARAM_SIZE);
	m_spComputeShader->BindUAVBuffer(UAV_REGISTER::PARTICLEWRITEDATA, m_spParticleStructedBuffer);
	m_spComputeShader->BindUAVBuffer(UAV_REGISTER::SHRAEDDATA, m_spComputeShaderStructedBuffer);

	m_spComputeShader->CommitLocalShaderDatas(m_arrComputeCommands[m_sComputeIndex], 1, 1, 1);
	//m_sComputeIndex = (m_sComputeIndex + 1) % m_arrComputeCommands.size();
}

void UParticleSystem::BindShaderParams(CSHPTRREF<UShader> _spShader)
{
	RETURN_CHECK(nullptr == _spShader, ;);
	_spShader->BindSRVBuffer(SRV_REGISTER::T14, m_spParticleStructedBuffer);
	_spShader->BindCBVBuffer(m_spRenderParticleConstnatBuffer, &m_stParticleParam, PARTICLEPARAM_SIZE);
}

void UParticleSystem::SettingComputeShader(const _wstring& _wstrProtoName)
{
	SHPTR<UGameInstance> spGameInstance = GET_INSTANCE(UGameInstance);
	m_spComputeShader = static_pointer_cast<UComputeShader>(spGameInstance->CloneResource(_wstrProtoName));

	if (nullptr == m_spComputeShader)
	{
#ifdef _USE_DEBUGGING
		assert("Get Compute Shader Failed");
#endif 
	}
}

_bool UParticleSystem::Load(const _wstring& _wstrPath)
{
	std::ifstream load{_wstrPath, std::ios::binary};
	RETURN_CHECK(!load, false);

	_wstring wstrProtoName;
	UMethod::ReadString(load, wstrProtoName);
	load.read((_char*) & m_stParticleParam, sizeof(PARTICLEPARAM));

	SHPTR<UGameInstance> spGameInstance = GET_INSTANCE(UGameInstance);
	m_spComputeShader = dynamic_pointer_cast<UComputeShader>(spGameInstance->CloneResource(wstrProtoName));
	RETURN_CHECK(nullptr == m_spComputeShader, false);
	return true;
}

_bool UParticleSystem::Save(const _wstring& _wstrPath)
{
	std::ofstream save{_wstrPath, std::ios::binary};
	RETURN_CHECK(!save, false);

	PARTICLEPARAM stParam;
	::memcpy(&stParam, &m_stParticleParam, sizeof(PARTICLEPARAM));
	SHPTR<UGameInstance> spGameInstance = GET_INSTANCE(UGameInstance);
	_wstring _wstrProto{ L"" };
	spGameInstance->FindResourceProtoTag(static_pointer_cast<UResource>(m_spComputeShader), _wstrProto);
	// Not Find 
	RETURN_CHECK(L"" == _wstrProto, false);

	UMethod::SaveString(save, _wstrProto);
	save.write((char*)&stParam, sizeof(PARTICLEPARAM));
	return true;
}

#ifdef _USE_IMGUI
// Particle System 
void UParticleSystem::ShowObjectInfo()
{

}
#endif 