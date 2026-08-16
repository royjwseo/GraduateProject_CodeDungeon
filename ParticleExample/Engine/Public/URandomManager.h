#ifndef _PTH_FRAMEWORK_ENGIEN_PUBLIC_URANDOMMANAGER_H
#define _PTH_FRAMEWORK_ENGIEN_PUBLIC_URANDOMMANAGER_H

#include "UBase.h"

BEGIN(Engine)

class DEF_CACHE_ALGIN URandomManager  final : public UBase {
public:
	URandomManager();
	NO_COPY(URandomManager)
	DESTRUCTOR(URandomManager)
public:
	virtual void Free() override;

	_int GetRandomNumber(const _int _iMaxNum);
private:
	std::uniform_int_distribution<_int>			m_UniformDistribution;
	std::default_random_engine						m_dre;
};

END

#endif // _PTH_FRAMEWORK_ENGIEN_PUBLIC_URANDOMMANAGER_H