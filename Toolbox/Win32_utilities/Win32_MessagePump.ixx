// Win32_MessagePump.ixx
//
// win32_utilities

module;

#include "Windows.h"
#include <vector>

export module MessagePump;

using std::vector;

export class MessagePump
{
public:
	//void SetAccelTable(HACCEL const);
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