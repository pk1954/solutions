// UndoRedoMenu.h
//
// NNetWindows

#pragma once

#include "ObserverInterface.h"
#include "win32_NNetAppMenu.h"

class UndoRedoMenu : public ObserverInterface
{
public:
	virtual ~UndoRedoMenu() {}

	inline void Start( NNetAppMenu * const pAppMenu )
	{
		m_pAppMenu = pAppMenu;
	}

	virtual void Notify( bool const bImmediatelly )
	{
		m_pAppMenu->AdjustUndoRedo();
	}

private:

	NNetAppMenu * m_pAppMenu  { nullptr };
};