#ifndef _SERVERFRAMEWORK_CORE_PUBLIC_ASPACE_H
#define _SERVERFRAMEWORK_CORE_PUBLIC_ASPACE_H

#include "ACoreBase.h"

BEGIN(Core)
class ATransform;
class ACollider;
class ASession;

/*
@ Date: 2023-01-22, Writer: 박태현
@ Explain
- 게임상의 공간을 정의하여 시야 확보를 하기 위한 클래스이다.
전체 오브젝트를 검색하는 것은 낭비다. 그래서 논리적인 공간을 나누어서 검색을 하는 것이 합리적이라고 생각해서
해당 클래스를 정의했다.
*/
class CORE_DLL  ASpace final : public ACoreBase {
public:
	using SESSIONCONTAINER = CONLIST<SHPTR<ASession>>;
	using SPACECHILD = CONVECTOR<SHPTR<ASpace>>;
public:
	ASpace();
	NO_COPY(ASpace)
	DESTRUCTOR(ASpace)
public:
	_bool NativeConstruct(const SPACEINFO& _SpaceInfo, SHPTR<ASpace> _spParents = nullptr);
	_bool InsertSession(SHPTR<ASession> _spSession);
	void BroadCastMessage(_char* _pPacket, const PACKETHEAD& _PacketHead);
	_int GetChildIndexToPosition(const Vector3 _vPos);
	_bool IsContains(SHPTR<ACollider> _spCollider);

	SHPTR<ASpace> MakeChild(const _int _Index, const SPACEINFO& _SpaceInfo);
	void OnChildNode() { m_isChildNode = true; }
public: /* get set */
	const _int GetSpaceIndex() const { return m_SpaceIndex; }
	const _int GetDepthLevel() const { return m_DepthLevel; }
	const _bool IsChildNode() const { return m_isChildNode; }
	SHPTR<ACollider> GetCollider() const { return m_spCollider; }
	const SPACECHILD& GetSpaceChild() const { return m_SpaceChild; }
protected:
	_bool InsertSessionRecursively(CSHPTRREF<ASession> _spSession);
private:
	virtual void Free() override;
private:
	_int												m_SpaceIndex;
	_int												m_DepthLevel;
	_bool											m_isChildNode;
	// Child + Parents
	WKPTR<ASpace>					m_wpParents;
	SPACECHILD								m_SpaceChild;
	SHPTR<ACollider>					m_spCollider;
	// LIST
	SESSIONCONTAINER				m_SessionContainer;
};

END


#endif // _SERVERFRAMEWORK_CORE_PUBLIC_ASPACE_H