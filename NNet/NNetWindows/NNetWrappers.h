// NNetWrappers.h : wrapper functions for unit tests and scripting of NNet
//
// NNetWindows

class NNetModelReaderInterface;
class NNetModelCommands;
class NNetModelIO;

void NNetWrappersSetModelInterface(NNetModelReaderInterface * const);

void InitializeNNetWrappers
(
    NNetModelCommands * const,
    NNetModelIO       * const
);
