// NNetWrapperHelpers.ixx
//
// NNetModel

module;

#include <memory>

export module NNetWrapperHelpers;

import Types;
import Script;
import NNetModel;

using std::unique_ptr;

export MicroMeter            ScrReadMicroMeter         (Script &);
export MicroMeterPnt         ScrReadMicroMeterPnt      (Script &);
export MicroMeterCircle      ScrReadMicroMeterCircle   (Script &);
export MicroMeterPntVector   ScrReadMicroMeterPntVector(Script &);
export unique_ptr<NobIdList> ScrReadNobIdList          (Script &);
export NobId                 ScrReadNobId              (Script &);
export SignalId              ScrReadSignalId           (Script &);
export SigGenId              ScrReadSigGenId           (Script &);
export NobType               ScrReadNobType            (Script &);
export ParamType::Value      ScrReadParamType          (Script &);
export mV                    ScrReadVoltage            (Script &);
export SigGenStaticData      ScrReadSigGenStaticData   (Script &);
