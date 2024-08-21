// Win32_Event.ixx
//
// Toolbox\win32_utilities

module;

#include "Windows.h"

export module Win32_Event;

import EventInterface;

export class Win32_Event : public EventInterface
{
public:
	Win32_Event()
	{ 
		m_eventHandle =
			CreateEvent
			(
				nullptr, // no security attributes
				true,    // manual reset event 
				false,   // initial state nonsignaled
				nullptr  // no name for event
			);
	}

	~Win32_Event()
	{
		CloseHandle(m_eventHandle);
		m_eventHandle = nullptr;
	}

	virtual void Wait()
	{
		ResetEvent(m_eventHandle);
		WaitForSingleObject(m_eventHandle, INFINITE);
	}

	virtual void Continue()
	{
		SetEvent(m_eventHandle);
	}

private:
	HANDLE m_eventHandle { nullptr };
};
