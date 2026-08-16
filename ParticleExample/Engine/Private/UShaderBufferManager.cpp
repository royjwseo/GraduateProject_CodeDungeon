#include "EngineDefines.h"
#include "UShaderBufferManager.h"
#include "UGrobalConstantBuffer.h"
#include "UShaderConstantBuffer.h"

UShaderBufferManager::UShaderBufferManager()
{
}

void UShaderBufferManager::Free()
{
}

HRESULT UShaderBufferManager::ReadyShaderBufferManager(CSHPTRREF<UDevice> _spDevice)
{ 
	{
		RETURN_CHECK_FAILED(CreateGrobalConstantBuffer(_spDevice, GROBAL_VIEWPROJ, CBV_REGISTER::VIEWPROJ, sizeof(VIEWPROJPARAM)), E_FAIL);
		RETURN_CHECK_FAILED(CreateGrobalConstantBuffer(_spDevice, GROBAL_GROBALDATA, CBV_REGISTER::GROBALDATA, sizeof(GROBALPARAM)), E_FAIL);
		RETURN_CHECK_FAILED(CreateGrobalConstantBuffer(_spDevice, GROBAl_LIGHTSTATIC, CBV_REGISTER::STATICLIGHT, sizeof(LIGHTPARAM)), E_FAIL);
	}
	{
		RETURN_CHECK_FAILED(CreateShaderConstantBuffer(_spDevice, PREALLOCATED_TRANSFORM, CBV_REGISTER::TRANSFORM, sizeof(TRANSFORMPARAM), 50000), E_FAIL);
	}
	return S_OK;
}

void UShaderBufferManager::BindGrobalBuffer(const GROBAL_CBUFFERTYPE _eGrobalCBuffer, CSHPTRREF<UCommand> _spCommand, const void* _pBuffer, const _uint _iSize)
{
	RETURN_CHECK(m_arrGrobalConstantBufferContainer.size() <= _eGrobalCBuffer, );
	m_arrGrobalConstantBufferContainer[_eGrobalCBuffer]->SettingGrobalData(_spCommand, _pBuffer, _iSize);
}

HRESULT UShaderBufferManager::GetGrobalConstantBuffer(const GROBAL_CBUFFERTYPE _eGrobalCBuffer, SHPTRREF<UGrobalConstantBuffer> _spGrobalConstantBuffer)
{
	RETURN_CHECK(m_arrGrobalConstantBufferContainer.size() <= _eGrobalCBuffer, E_FAIL);
	_spGrobalConstantBuffer = m_arrGrobalConstantBufferContainer[_eGrobalCBuffer];
	return S_OK;
}

HRESULT UShaderBufferManager::GetPreAllocatedConstantBuffer(const PREALLOCATED_CBUFFERTYPE _ePreAllocatedCBufferType, SHPTRREF<UShaderConstantBuffer> _spShaderConstantBuffer)
{
	RETURN_CHECK(m_arrPreAllocatedCBufferContainer.size() <= _ePreAllocatedCBufferType, E_FAIL);
	_spShaderConstantBuffer = m_arrPreAllocatedCBufferContainer[_ePreAllocatedCBufferType];
	return S_OK;
}

HRESULT UShaderBufferManager::CreateGrobalConstantBuffer(CSHPTRREF<UDevice> _spDevice, const GROBAL_CBUFFERTYPE _eType, const CBV_REGISTER& _eReg, const _uint& _iBufferSize)
{
	_ubyte ByteInt = static_cast<_ubyte>(_eType);
	RETURN_CHECK(m_arrGrobalConstantBufferContainer.size() <= ByteInt || nullptr != m_arrGrobalConstantBufferContainer[ByteInt], E_FAIL);
	// Create GrobalBuffer 
	m_arrGrobalConstantBufferContainer[ByteInt] = CreateNative<UGrobalConstantBuffer>(_spDevice, _eReg, _iBufferSize);
	return S_OK;
}

HRESULT UShaderBufferManager::CreateShaderConstantBuffer(CSHPTRREF<UDevice> _spDevice, const PREALLOCATED_CBUFFERTYPE _eType, const CBV_REGISTER& _eReg, const _uint& _iBufferSize, const _uint _iCnt)
{
	_ubyte ByteInt = static_cast<_ubyte>(_eType);
	RETURN_CHECK(m_arrPreAllocatedCBufferContainer.size() <= ByteInt || nullptr != m_arrPreAllocatedCBufferContainer[ByteInt], E_FAIL);
	// Create GrobalBuffer 
	m_arrPreAllocatedCBufferContainer[ByteInt] = CreateNative<UShaderConstantBuffer>(_spDevice, _eReg, _iBufferSize, _iCnt);
	return S_OK;
}
