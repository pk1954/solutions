// NNetWrapperHelpers.h : 
//
// NNetModel

#pragma once

#include "NobType.h"
#include "SigGenStaticData.h"

import BasicTypes;
import MoreTypes;
import MicroMeterPntVector;
import NobIdList;
import ParamType;
import SignalId;
import Script;
import NobId;

using std::unique_ptr;

extern MicroMeter            ScrReadMicroMeter         (Script &);
extern MicroMeterPnt         ScrReadMicroMeterPnt      (Script &);
extern MicroMeterCircle      ScrReadMicroMeterCircle   (Script &);
extern MicroMeterPntVector   ScrReadMicroMeterPntVector(Script &);
extern unique_ptr<NobIdList> ScrReadNobIdList          (Script &);
extern NobId                 ScrReadNobId              (Script &);
extern SignalId              ScrReadSignalId           (Script &);
extern NobType               ScrReadNobType            (Script &);
extern ParamType::Value      ScrReadParamType          (Script &);
extern mV                    ScrReadVoltage            (Script &);
extern SigGenStaticData      ScrReadSigGenStaticData   (Script &);
