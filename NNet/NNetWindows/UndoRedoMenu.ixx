// UndoRedoMenu.ixx
//
// NNetWindows

module;

#include "win32_NNetAppMenu.h"

export module UndoRedoMenu;

import ObserverInterface;

export class UndoRedoMenu : public ObserverInterface
{
public:
	virtual ~UndoRedoMenu() = default;

	inline void Start(NNetAppMenu * const pAppMenu)
	{
		m_pAppMenu = pAppMenu;
	}

	virtual void Notify(bool const bImmediatelly)
	{
		m_pAppMenu->AdjustUndoRedo();
	}

private:

	NNetAppMenu * m_pAppMenu  { nullptr };
};