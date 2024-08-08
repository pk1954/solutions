// NNetCommand.ixx
//
// NNetCommands

export module NNetCommands:NNetCommand;

export import IoUtil;
export import SoundInterface;
export import Types;
export import WinCommand;
export import NNetModel;
export import NNetWrapperHelpers;

import std;
import Win32_Util_Resource;
import WinManager;

export using std::make_unique;
export using std::unique_ptr;
export using std::wstring;

using std::vector;

export class NNetCommand : public WinCommand
{
public:
    ~NNetCommand() override = default;

    static void Initialize(Sound* const p) 
    { 
        m_pSound = p; 
    }

    static void SetModelInterface(NNetModelWriterInterface* const pNMWI)
    {
        m_pNMWI = pNMWI;
    }

    void UpdateUI() final
    {
        WinManager::GetRootWindow(RootWinId(IDM_MAIN_WINDOW))->Notify(false);
    };

    virtual NobId GetAffectedNob() const
    {
        return NO_NOB;
    }

protected:

    static void PlaySound(vector<unsigned char> const & sound) { m_pSound->Play(sound); }

    inline static NNetModelWriterInterface * m_pNMWI { nullptr };

private:

    inline static Sound * m_pSound { nullptr };
};
