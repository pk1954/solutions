// WrapVoltage.ixx
//
// ModelIO

module;

#include <iostream>

export module WrapVoltage;

import Wrapper;
import Script;
import NNetModelIO;
import NNetModel;

using std::wostream;

export class WrapVoltage : public Wrapper
{
public:
    using Wrapper::Wrapper;

    void operator() (Script & script) const final
    {
        setVoltage(script);
    }

    void Write(wostream & out) const final
    {
        NNetModelReaderInterface const& nmwi{ NNetModelIO::GetExportNMRI() };
        nmwi.Apply2AllC<PosNob     >([this, &out](PosNob      const& s) { writeVoltage(out, s); });
        nmwi.Apply2AllC<Pipe       >([this, &out](Pipe        const& s) { writeVoltage(out, s); });
        nmwi.Apply2AllC<IoConnector>([this, &out](IoConnector const& s) { writeVoltage(out, s); });
    };

private:

    void setVoltage      (Script &)              const;
    void setPosNobVoltage(Script &, NobId const) const;
    void setPipeVoltage  (Script &, NobId const) const;

    void writePipeVoltage(wostream &, Pipe const &) const;
    void writeVoltage    (wostream &, Nob  const &) const;
};
