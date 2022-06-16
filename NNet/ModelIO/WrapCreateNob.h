// WrapCreateNob.h 
//
// ModelIO

#pragma once

#include "NNetWrapperBase.h"

class Script;

class WrapCreateNob : public NNetWrapperBase
{
public:
    using NNetWrapperBase::NNetWrapperBase;

    void operator() (Script & script) const final
    {   
        createNob(script);
    }

    void Write(wostream & out) const final 
    { 
        NNetModelReaderInterface const & nmri { m_modelIO.GetExportNMRI() };
        nmri.Apply2AllC<BaseKnot   >([this, &out](BaseKnot    const &s) { writeNob(out, s); });
        nmri.Apply2AllC<Pipe       >([this, &out](Pipe        const &s) { writeNob(out, s); });
        nmri.Apply2AllC<IoConnector>([this, &out](IoConnector const &s) { writeNob(out, s); });
    };

private:

    Nob * createNob        (Script &) const;
    UPNob createPipe       (Script &) const;
    UPNob createBaseKnot   (Script &, NobType const) const;
    UPNob createIoConnector(Script &, NobType const) const; 

    void writePipe       (wostream &, Pipe const &) const;
    void writeIoConnector(wostream &, IoConnector const &) const;
    void writeNob        (wostream &, Nob const &) const;
 };
