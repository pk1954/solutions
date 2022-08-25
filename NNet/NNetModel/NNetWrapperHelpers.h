// NNetWrapperHelpers.h : 
//
// NNetModel

#pragma once

#include "NobId.h"
#include "SignalId.h"
#include "NobType.h"
#include "SigGenStaticData.h"
#include "Script.h"

import BasicTypes;
import MoreTypes;
import MicroMeterPntVector;
import NobIdList;
import ParamType;

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
