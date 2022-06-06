// WrapSignalGenerator.h 
//
// ModelIO

#pragma once

#include "NNetWrapperHelpers.h"
#include "NNetModelIO.h"
#include "NNetWrapperBase.h"

class WrapSignalGenerator : public NNetWrapperBase 
{
public:
    using NNetWrapperBase::NNetWrapperBase;

    void operator() (Script & script) const final
    {
        wstring          const name       { script.ScrReadString() };
        SigGenStaticData const sigGenData { ScrReadSigGenStaticData(script) };
        SigGenId         const sigGenId   { m_modelIO.GetImportNMWI().FindSigGen(name) };
        if (m_modelIO.GetImportNMWI().IsValid(sigGenId))
        {
            SignalGenerator * pSigGen { m_modelIO.GetImportNMWI().GetSigGen(sigGenId) };
            pSigGen->SetStaticData(sigGenData);
        }
        else
        {
            UPSigGen upSigGen { m_modelIO.GetImportNMWI().NewSigGen(name) };
            upSigGen->SetStaticData(sigGenData);
            m_modelIO.GetImportNMWI().PushSigGen(move(upSigGen));
        }
    }

    void Write(wostream & out) const final 
    {
        m_modelIO.GetExportNMWI().GetSigGenList().Apply2AllC
        (
            [this, &out](auto const & upSigGen)
            { 
                WriteCmdName(out);    
                out << L"\"" << upSigGen->GetName() << "\" " 
                    << upSigGen->GetStaticData() 
                    << endl;
            }
        );

    };
};
