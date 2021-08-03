// win32_messagePump.h
//
// win32_utilities

#pragma once

#include <vector>

using std::vector;

class MessagePump
{
public:
	void SetAccelTable(HINSTANCE const, int const);

	void RegisterWindow(HWND const, bool const);
	
	int Run();

private:
	struct AccEntry
	{
		HWND m_hwnd;
		bool m_bIsDialog;
	};

	vector<AccEntry> m_accEntries;
	HACCEL           m_defaultAccelTable { nullptr };

	bool accelerator(MSG &);
};