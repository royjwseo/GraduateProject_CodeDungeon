
#ifndef _PTH_FRAMEWORK_ENGIEN_PUBLIC_UEVENT_H
#define _PTH_FRAMEWORK_ENGIEN_PUBLIC_UEVENT_H

BEGIN(Engine)

/*

Event 객체를 정의한 클래스 

이 클래스를 활용하면 해당 이벤트가 발생할 때 까지 대기할 수 있도록 만들 예정이다. 
이벤트의 복제를 금한다. 

*/

// 이벤트를 정의한 enum class 
enum EVENT : _ushort 
{
	EVENT_NON = 0,
	EVENT_ACTIVE = 1,

};

class ENGINE_DLL UEvent {
public:
	UEvent();
	UEvent(const _ushort  _sEvent);
	UEvent(const UEvent& _rhs);
	~UEvent();

public:
	static void AllEventClear();

	// 해당 이벤트를 잠근다. 
	void WaitEvent();
	void CountDown();
private:
	static VECTOR<UEvent*>				s_m_vecEventContainer;

	//  조건 분기 
	 std::condition_variable				m_ConditionVariable;
	atomic_short								m_sEventCountSaves;
	atomic_short								m_sEventCountIndex;
	// 이벤트 잠금
	std::mutex										m_EventMutex;
};


END

#endif //_PTH_FRAMEWORK_ENGIEN_PUBLIC_UEVENT_H