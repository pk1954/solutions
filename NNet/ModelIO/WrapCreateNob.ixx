// WrapCreateNob.ixx
//
// ModelIO

module;

#include <iostream>
#include "NNetModelReaderInterface.h"
#include "BaseKnot.h"
#include "Pipe.h"
#include "IoConnector.h"
#include "NobType.h"
#include "Nob.h"
#include "Script.h"

export module WrapCreateNob;

import NNetWrapperBase;

using std::wostream;

export class WrapCreateNob : public NNetWrapperBase
{
public:
    using NNetWrapperBase::NNetWrapperBase;

    void operator() (Script& script) const final
    {
        createNob(script);
    }

    void Write(wostream& out) const final
    {
        NNetModelReaderInterface const& nmri{ m_modelIO.GetExportNMRI() };
        nmri.Apply2AllC<BaseKnot   >([this, &out](BaseKnot    const& s) { writeNob(out, s); });
        nmri.Apply2AllC<Pipe       >([this, &out](Pipe        const& s) { writeNob(out, s); });
        nmri.Apply2AllC<IoConnector>([this, &out](IoConnector const& s) { writeNob(out, s); });
    };

private:

    Nob* createNob(Script&) const;
    UPNob createPipe(Script&) const;
    UPNob createBaseKnot(Script&, NobType const) const;
    UPNob createIoConnector(Script&, NobType const) const;

    void writePipe(wostream&, Pipe const&) const;
    void writeIoConnector(wostream&, IoConnector const&) const;
    void writeNob(wostream&, Nob const&) const;
};
