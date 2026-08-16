#include "EngineDefines.h"
using namespace std; 
#include "UEvent.h"

VECTOR<UEvent*> UEvent::s_m_vecEventContainer;

UEvent::UEvent() : m_sEventCountIndex(1), m_sEventCountSaves(1)
{
	s_m_vecEventContainer.push_back(this);
}

UEvent::UEvent(const _ushort _sEvent) : m_sEventCountIndex(_sEvent), m_sEventCountSaves(_sEvent)
{
	s_m_vecEventContainer.push_back(this);
}

UEvent::UEvent(const UEvent& _rhs) : 
	m_sEventCountIndex(_rhs.m_sEventCountIndex.load()), m_sEventCountSaves(_rhs.m_sEventCountSaves.load())
{
}

UEvent::~UEvent()
{
}

void UEvent::AllEventClear()
{
	for (auto& iter : s_m_vecEventContainer)
	{
		iter->m_sEventCountIndex = 0;
	}

	for (auto& iter : s_m_vecEventContainer)
	{
		iter->m_ConditionVariable.notify_one();
	}
}

void UEvent::WaitEvent()
{
	// Event CountIndex가 0보다 작을 경우 
	std::unique_lock<std::mutex> LL(m_EventMutex);
	m_ConditionVariable.wait(LL, [this] { return m_sEventCountIndex <= 0; });
	m_sEventCountIndex.store( m_sEventCountSaves);
}


void UEvent::CountDown()
{
	--m_sEventCountIndex;
	if(m_sEventCountIndex <= 0)
	{
		m_ConditionVariable.notify_one();
	}
}