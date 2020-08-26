// win32_messagePump.h
//
// win32_utilities

#pragma once

#include <vector>
#include "minwindef.h"

using std::vector;

class MessagePump
{
public:
	void DefaultAccelTable( HACCEL const haccel )
	{
		m_defaultAccelTable = haccel;
	}

	void InstallAccelTable( HWND const, HACCEL const );
	
	int  Run( );

private:
	struct AccEntry
	{
		HWND   m_hwnd;
		HACCEL m_hAccelTable;
	};

	vector<AccEntry> m_accEntries;
	HACCEL           m_defaultAccelTable { nullptr };

	bool translateAcceleratorForWindow( AccEntry const &, MSG & );
};