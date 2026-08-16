#include "pch.h"
#include "CloneObject.h"
#include "DeviceAndFactory.h"
#include "CommandQueue.h"

CloneObject::CloneObject(const shared_ptr<DeviceAndFactory>& _spDevice)
	: Object(_spDevice), m_isClone{ false }
{
}

CloneObject::CloneObject(const CloneObject& _rhs)
	: Object(_rhs)
{
	m_isClone = true;
}

HRESULT CloneObject::NativeConstruct()
{
	return __super::NativeConstruct();
}

HRESULT CloneObject::NativeConstructClone(const vector<void*>& _stDatas)
{
	return S_OK;
}
