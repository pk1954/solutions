// UndoRedoMenu.ixx
//
// NNetWindows

export module NNetWin32:UndoRedoMenu;

import ObserverInterface;
import :NNetAppMenu;

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

	NNetAppMenu * m_pAppMenu { nullptr };
};