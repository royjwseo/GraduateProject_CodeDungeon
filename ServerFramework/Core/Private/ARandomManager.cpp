#include "CoreDefines.h"
#include "ARandomManager.h"

namespace Core
{
	ARandomManager::ARandomManager() :
		m_UniformDistribution{  }
	{
	}

	_int ARandomManager::ReturnRadomNumber(const _int _iMinNum, const _int _iMaxNum)
	{
		_int number = _iMinNum + (m_UniformDistribution(m_dre)) % _iMaxNum;
		return number;
	}

	_int ARandomManager::ReturnRadomNumber(const _int _iMaxNum)
	{
		_int number =  (m_UniformDistribution(m_dre)) % _iMaxNum;
		return number;
	}

	void ARandomManager::Free()
	{
	}

}