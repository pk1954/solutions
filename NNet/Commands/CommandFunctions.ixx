// CommandFunctions.ixx
//
// Commands

module;

#include <memory>
#include "NNetModelWriterInterface.h"
#include "Nob.h"

export module CommandFunctions;

import NNetCommand;

using std::unique_ptr;

export unique_ptr<NNetCommand> MakeDeleteCommand(NNetModelWriterInterface const&, Nob&);
