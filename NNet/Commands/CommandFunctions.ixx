// CommandFunctions.ixx
//
// Commands

module;

#include <memory>

export module CommandFunctions;

import NNetCommand;
import NNetModel;

using std::unique_ptr;

export unique_ptr<NNetCommand> MakeDeleteCommand(NNetModelWriterInterface const&, Nob&);
