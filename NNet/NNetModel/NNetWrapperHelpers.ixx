// NNetWrapperHelpers.ixx
//
// NNetModel

module;

#include "NobType.h"

export module NNetWrapperHelpers;

import BasicTypes;
import MoreTypes;
import MicroMeterPntVector;
import NobIdList;
import ParamType;
import SignalId;
import Script;
import NobId;
import SigGenStaticData;

using std::unique_ptr;

export MicroMeter            ScrReadMicroMeter         (Script &);
export MicroMeterPnt         ScrReadMicroMeterPnt      (Script &);
export MicroMeterCircle      ScrReadMicroMeterCircle   (Script &);
export MicroMeterPntVector   ScrReadMicroMeterPntVector(Script &);
export unique_ptr<NobIdList> ScrReadNobIdList          (Script &);
export NobId                 ScrReadNobId              (Script &);
export SignalId              ScrReadSignalId           (Script &);
export NobType               ScrReadNobType            (Script &);
export ParamType::Value      ScrReadParamType          (Script &);
export mV                    ScrReadVoltage            (Script &);
export SigGenStaticData      ScrReadSigGenStaticData   (Script &);
