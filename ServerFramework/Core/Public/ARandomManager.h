#ifndef _SERVERFRAMEWORK_CORE_PUBLIC_ARANDOMMANAGER_H
#define _SERVERFRAMEWORK_CORE_PUBLIC_ARANDOMMANAGER_H

#include "ACoreBase.h"

BEGIN(Core)
/*
@ Date: 2023-01-22, Writer: 박태현
@ Explain
- Random한 값들을 추출하는 클래스이다. 
*/
class  ARandomManager final : public ACoreBase {
public:
	ARandomManager();
	NO_COPY(ARandomManager)
	DESTRUCTOR(ARandomManager)
public:
	_int		ReturnRadomNumber(const _int _iMinNum, const _int _iMaxNum);
	_int		ReturnRadomNumber(const _int _iMaxNum);
private:
	virtual void Free() override;
private:
	std::uniform_int_distribution<_int>			m_UniformDistribution;
	std::default_random_engine						m_dre;
};

END

#endif // _SERVERFRAMEWORK_CORE_PUBLIC_ARANDOMMANAGER_H