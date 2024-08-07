// WrapGlobalParameter.ixx
//
// ModelIO

module;

#include <iostream>
#include <iomanip>

export module WrapGlobalParameter;

import IoUtil;
import IoConstants;
import SaveCast;
import Commands;
import RunTime;
import NNetModelIO;
import NNetModel;

using std::setprecision;
using std::wostream;
using std::endl;

export class WrapGlobalParameter : public Wrapper
{
public:
    using Wrapper::Wrapper;

    void operator() (Script& script) const final
    {
        unsigned int uiParam;
        ScriptErrorHandler::ScriptException const errInfo { script.ScrReadUint(&uiParam) };
        bool bSuccess { errInfo.m_sErrNr == 0 };
        if (! bSuccess)
        {
            ScriptErrorHandler::PrintErrorInfo(script.GetScanner(), errInfo);
            ScriptErrorHandler::PrintNL(L"ignoring parameter");
        }
        script.ScrReadSpecial(L'=');
        float const fValue{ Cast2Float(script.ScrReadFloat()) };
        if (bSuccess)
        {
           NNetModelIO::GetImportNMWI().SetParam(static_cast<ParamType::Value>(uiParam), fValue);
        }
    }

    void Write(wostream & out) const final
    {
        ParamType::Apply2GlobalParameters
        (
            [this, &out](ParamType::Value const& par)
            {
                WriteCmdName(out);
                out << par << SPACE << EQUALS << SPACE
                    << setprecision(10) << NNetModelIO::GetExportNMRI().GetParameter(par)
                    << endl;
            }
        );
    };
};
