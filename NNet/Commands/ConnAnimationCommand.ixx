// ConnAnimationCommand.ixx
//
// Commands

module;

#include <vector>

export module ConnAnimationCommand;

import Types;
import NNetCommand;
import NNetModel;

using std::vector;

export class ConnAnimationCommand : public NNetCommand
{
public:
    ConnAnimationCommand();

    bool IsAsyncCommand() final { return true; };

    bool IsCmdOk() const { return m_bAllOk; }

private:

    vector<IoLine*> m_nobsAnimated{};

    bool m_bAllOk{ false };

    NobType determineNobType(UPNobList const&) const;
    void    sortNobsAnimated(MicroMeterLine const&);
};