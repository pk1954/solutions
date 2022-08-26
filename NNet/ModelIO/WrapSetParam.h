// WrapParam.h 
//
// ModelIO

#pragma once

#include "NNetWrapperHelpers.h"
#include "InputConnector.h"
#include "InputLine.h"
#include "NobId.h"
#include "Script.h"

import NNetWrapperBase;
import ParamType;
import SaveCast;

class WrapSetParam : public NNetWrapperBase 
{
public:
    using NNetWrapperBase::NNetWrapperBase;

    void operator() (Script & script) const final
    {
        NobId            const id       { ScrReadNobId(script) };
        ParamType::Value const param    { ScrReadParamType(script) };
        float                  fVal     { Cast2Float(script.ScrReadFloat()) };
        if (InputConnector   * pInpConn { m_modelIO.GetImportNMWI().GetNobPtr<InputConnector *>(id) }) // Legacy
        {                                                                                              // Legacy
            pInpConn->Apply2All                                                                        // Legacy
            (                                                                                          // Legacy
                [param, fVal](IoLine & n)                                                              // Legacy
                {                                                                                      // Legacy
                    auto & inputLine { static_cast<InputLine &>(n) };                                  // Legacy
                    inputLine.GetSigGen()->SetParam(param, fVal);                                      // Legacy
                }                                                                                      // Legacy
            );                                                                                         // Legacy
        }                                                                                              // Legacy 
        else if ( InputLine * pInpNeuron { m_modelIO.GetImportNMWI().GetNobPtr<InputLine*>(id) } )     // Legacy
        {
            pInpNeuron->GetSigGen()->SetParam(param, fVal);
        }
    }
};
