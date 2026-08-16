#include "EngineDefines.h"
#include "URandomManager.h"

URandomManager::URandomManager() : 
	m_UniformDistribution{  }
{
}

void URandomManager::Free()
{
}

_int URandomManager::GetRandomNumber(const _int _iMaxNum)
{
	_int number = (m_UniformDistribution(m_dre)) % _iMaxNum;
	return number;
}
