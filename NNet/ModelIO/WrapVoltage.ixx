// WrapVoltage.ixx
//
// ModelIO

module;

#include <iostream>
#include "NNetModelReaderInterface.h"
#include "Nob.h"
#include "NobId.h"
#include "BaseKnot.h"
#include "Pipe.h"
#include "IoConnector.h"
#include "NNetModelIO.h"
#include "Script.h"

export module WrapVoltage;

import NNetWrapperBase;

using std::wostream;

export class WrapVoltage : public NNetWrapperBase
{
public:
    using NNetWrapperBase::NNetWrapperBase;

    void operator() (Script & script) const final
    {
        setVoltage(script);
    }

    void Write(wostream & out) const final
    {
        NNetModelReaderInterface const& nmwi{ m_modelIO.GetExportNMRI() };
        nmwi.Apply2AllC<BaseKnot   >([this, &out](BaseKnot    const& s) { writeVoltage(out, s); });
        nmwi.Apply2AllC<Pipe       >([this, &out](Pipe        const& s) { writeVoltage(out, s); });
        nmwi.Apply2AllC<IoConnector>([this, &out](IoConnector const& s) { writeVoltage(out, s); });
    };

private:

    void setVoltage        (Script &)              const;
    void setBaseKnotVoltage(Script &, NobId const) const;
    void setPipeVoltage    (Script &, NobId const) const;

    void writePipeVoltage(wostream &, Pipe const &) const;
    void writeVoltage    (wostream &, Nob  const &) const;
};
