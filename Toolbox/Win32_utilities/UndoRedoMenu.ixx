// UndoRedoMenu.ixx
//
// Win32_utilities

export module UndoRedoMenu;

import ObserverInterface;
import Win32_Util_Resource;
import Win32_Util;
import WinBasics;
import Commands;

export class UndoRedoMenu : public ObserverInterface
{
public:
	virtual ~UndoRedoMenu() = default;

	inline void Start
	(
		HWND                 hwndApp,
		CommandStack * const pStack
	)
	{
		m_hwndApp = hwndApp;
		m_hMenu   = GetMenu(hwndApp);
		m_pStack  = pStack;
		Notify(true);
	}

	virtual void Notify(bool const bImmediatelly)
	{
		::Enable(m_hMenu, IDM_UNDO, ! m_pStack->UndoStackEmpty());
		::Enable(m_hMenu, IDM_REDO, ! m_pStack->RedoStackEmpty());
		DrawMenuBar(m_hwndApp);
	}

private:

	HWND           m_hwndApp { nullptr };
	HMENU          m_hMenu   { nullptr };
	CommandStack * m_pStack  { nullptr };
};