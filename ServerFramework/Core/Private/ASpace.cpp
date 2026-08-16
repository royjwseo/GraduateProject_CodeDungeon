#include "CoreDefines.h"
#include "ASpace.h"
#include "ASession.h"
#include "ACollider.h"

namespace Core {

	ASpace::ASpace() : m_SpaceIndex{0}, m_DepthLevel{0}, m_isChildNode{false}, m_wpParents{}, m_SpaceChild{},
	m_spCollider{ nullptr }, m_SessionContainer{}
	{
		m_SessionContainer.Initialize(TLS::MAX_THREAD, TLS::MAX_THREAD * 5);
	}

	_bool ASpace::NativeConstruct(const SPACEINFO& _SpaceInfo, SHPTR<ASpace> _spParents)
	{
		m_wpParents = _spParents;
		m_spCollider = Create<ACollider>(ACollider::TYPE_AABB, ACollider::COLLIDERDESC{ _SpaceInfo.vCenter, _SpaceInfo.vExtents });
		m_DepthLevel = _SpaceInfo.Depths;
		m_SpaceIndex = _SpaceInfo.Index;
		return true;
	}

	_bool ASpace::InsertSession(SHPTR<ASession> _spSession)
	{
		return InsertSessionRecursively(_spSession);
	}

	void ASpace::BroadCastMessage(_char* _pPacket, const PACKETHEAD& _PacketHead)
	{
		for (auto& iter : m_SessionContainer) {
			iter->SendData(_pPacket, _PacketHead);
		}
	}

	_int ASpace::GetChildIndexToPosition(const Vector3 _vPos)
	{
		_int index = 0;
		index |= (m_spCollider->GetCurPos().x > _vPos.x) ? 1 : 0;
		index |= (m_spCollider->GetCurPos().y > _vPos.y) ? 2 : 0;
		index |= (m_spCollider->GetCurPos().z > _vPos.z) ? 4 : 0;
		return index;
	}

	_bool ASpace::IsContains(SHPTR<ACollider> _spCollider)
	{
		RETURN_CHECK(nullptr == _spCollider, false);
		return m_spCollider->IsCollision(_spCollider);
	}

	SHPTR<ASpace> ASpace::MakeChild(const _int _Index, const SPACEINFO& _SpaceInfo)
	{
		RETURN_CHECK(MAX_OCTREENODE_LENGTH <= _Index, nullptr);
		if (m_SpaceChild.size() <= 0) {
			m_SpaceChild.resize(MAX_OCTREENODE_LENGTH);
		}
		m_SpaceChild[_Index] = CreateInitNative<ASpace>(_SpaceInfo, ThisShared<ASpace>());
		return m_SpaceChild[_Index];
	}

	_bool ASpace::InsertSessionRecursively(CSHPTRREF<ASession> _spSession)
	{
//		RETURN_CHECK(false == m_spCollider->IsCollision(_spSession->GetCollider()), false);
		// 재귀 함수로 Insert 한다. 
		if (true == m_isChildNode) {
			_spSession->BringSpaceIndex(ThisShared<ASpace>());
			m_SessionContainer.Insert(_spSession);
		}
		else
		{
			return InsertSessionRecursively(_spSession);
		}
		return true;
	}

	void ASpace::Free()
	{
	}

}