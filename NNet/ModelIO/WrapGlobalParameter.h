// WrapGlobalParameter.h 
//
// ModelIO

#pragma once

#include "ParameterType.h"
#include "NNetModelWriterInterface.h"
#include "NNetWrapperBase.h"

class WrapGlobalParameter : public NNetWrapperBase
{
public:
	using NNetWrapperBase::NNetWrapperBase;

    void operator() (Script & script) const final
    {
        auto const param(static_cast<ParamType::Value>(script.ScrReadUint()));
        script.ScrReadSpecial(L'=');
        float const fValue { Cast2Float(script.ScrReadFloat()) };
        m_modelIO.GetImportNMWI().SetParam(param, fValue);
    }

	void Write(wostream & out) const final 
	{
        ParamType::Apply2GlobalParameters
        (
            [this, &out](ParamType::Value const & par) 
            {
                out << L"GlobalParameter" << par << L" = "
                    << m_modelIO.GetExportNMWI().GetParameter(par) 
                    << endl; 
            }
        );
    };
};
