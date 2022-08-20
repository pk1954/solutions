// ConnAnimationCommand.ixx
//
// Commands

module;

#include <vector>
#include "UPNobList.h"
#include "win32_rootWindow.h"
#include "NobType.h"
#include "IoLine.h"
#include "NNetCommand.h"

export module ConnAnimationCommand;

using std::vector;

export class ConnAnimationCommand : public NNetCommand
{
public:
    ConnAnimationCommand();

    bool IsAsyncCommand() final { return true; };

    bool IsCmdOk() const { return m_bAllOk; }

private:

    vector<IoLine *> m_nobsAnimated {};

    bool m_bAllOk { false };

    NobType determineNobType(UPNobList const &) const;
    void    sortNobsAnimated(MicroMeterLine const &);
};