// WrapGlobalParameter.ixx
//
// ModelIO

module;

#include <iostream>
#include <iomanip>

export module WrapGlobalParameter;

import SaveCast;
import NNetWrapperBase;
import Script;
import ErrHndl;
import NNetModelIO;
import NNetModel;

using std::setprecision;
using std::wostream;
using std::endl;

export class WrapGlobalParameter : public NNetWrapperBase
{
public:
    using NNetWrapperBase::NNetWrapperBase;

    void operator() (Script& script) const final
    {
        unsigned int uiParam;
        ScriptErrorHandler::ScriptException const errInfo { script.ScrReadUint(&uiParam) };
        script.ScrReadSpecial(L'=');
        float const fValue{ Cast2Float(script.ScrReadFloat()) };
        if (errInfo.m_sErrNr)
        {
            ScriptErrorHandler::PrintErrorInfo(script.GetScanner(), errInfo);
            ScriptErrorHandler::PrintNL(L"ignoring parameter");
        }
        else
        {
           m_modelIO.GetImportNMWI().SetParam(static_cast<ParamType::Value>(uiParam), fValue);
        }
    }

    void Write(wostream & out) const final
    {
        ParamType::Apply2GlobalParameters
        (
            [this, &out](ParamType::Value const& par)
            {
                if (par != ParamType::Value::synapseThreshold)   // legacy
                {
                    out << L"GlobalParameter" << par << L" = "
                        << setprecision(10) << m_modelIO.GetExportNMRI().GetParameter(par)
                        << endl;
                }
            }
        );
    };
};
