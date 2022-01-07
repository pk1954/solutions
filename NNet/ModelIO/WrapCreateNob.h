// WrapCreateNob.h 
//
// ModelIO

#pragma once

#include "SCRIPT.H"
#include "NNetWrapperHelpers.h"
#include "WrapperBase.h"

class WrapCreateNob : public WrapperBase
{
public:
    using WrapperBase::WrapperBase;

    void operator() (Script & script) const final
    {   
        createNob(script);
    }

private:

    Nob * createNob        (Script &) const;
    UPNob createPipe       (Script &) const;
    UPNob createBaseKnot   (Script &, NobType const) const;
    UPNob createIoConnector(Script &, NobType const) const; 
};
