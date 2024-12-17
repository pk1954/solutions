// BlokusCommand.ixx
//
// BlokusCommands

export module BlokusCommands:BlokusCommand;

import std;

export import WinCommand;
export import BlokusCore;

import Win32_Util_Resource;
import WinManager;

export using std::make_unique;

export class BlokusCommand: public WinCommand
{
public:

    static void SetMatchInterface(MatchWriterInterface* const pMWI)
    {
        m_pMWI = pMWI;
    }

    virtual void UpdateUI()
    {
        WinManager::GetRootWindow(RootWinId(IDM_MAIN_WINDOW))->Notify(false);
    };

protected:

    inline static MatchWriterInterface * m_pMWI { nullptr };

private:

};