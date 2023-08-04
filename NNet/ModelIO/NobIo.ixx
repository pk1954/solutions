// NobIo.ixx
//
// ModelIO

module;

#include <iostream>
#include <memory>

export module NobIo;

import WrapBase;
import Types;
import Script;
import NNetModel;
import NNetModelIO;

using std::wostream;
using std::unique_ptr;
using std::make_unique;

export class NobIo : public WrapBase
{
public:
    using WrapBase::WrapBase;

    void operator() (Script& script) const final
    {
        createNob(script);
    }

    void Write(wostream& out) const final
    {
        NNetModelReaderInterface const& nmri { NNetModelIO::GetExportNMRI() };
        nmri.Apply2AllC<IoLine>     ([this, &out](Nob const& n) { writeNob(out, n); });
        nmri.Apply2AllC<IoConnector>([this, &out](Nob const& n) { writeNob(out, n); });
        nmri.Apply2AllC<Synapse>    ([this, &out](Nob const& n) { writeNob(out, n); });
        nmri.Apply2AllC<Neuron>     ([this, &out](Nob const& n) { writeNob(out, n); });
        nmri.Apply2AllC<Knot>       ([this, &out](Nob const& n) { writeNob(out, n); });
        nmri.Apply2AllC<Fork>       ([this, &out](Nob const& n) { writeNob(out, n); });
        nmri.Apply2AllC<Pipe>       ([this, &out](Nob const& n) { writeNob(out, n); });
    }

private:

    Pipe* getPipePtr(NobId const) const;

    Nob * createNob        (Script&)                const;
    Pipe* createPipe       (Script&, NobId const)   const;
    UPNob createSynapse    (Script&)                const;
    UPNob createNeuron     (Script&)                const;
    UPNob createKnot       (Script&)                const;
    UPNob createFork       (Script&)                const;
    UPNob createInputLine  (Script&)                const;
    UPNob createOutputLine (Script&)                const;
    UPNob createIoConnector(Script&, NobType const) const;

    void writePipe       (wostream&, Pipe        const&) const;
    void writeSynapse    (wostream&, Synapse     const&) const;
    void writeIoConnector(wostream&, IoConnector const&) const;
    void writeNeuron     (wostream&, Neuron      const&) const;
    void writeInputLine  (wostream&, InputLine   const&) const;
    void writeOutputLine (wostream&, OutputLine  const&) const;
    void writeKnot       (wostream&, Knot        const&) const;
    void writeFork       (wostream&, Fork        const&) const;
    void writeNob        (wostream&, Nob         const&) const;
};