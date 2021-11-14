// NNetWrapperHelpers.h : 
//
// NNetWindows

#pragma once

#include "script.h"
#include "moretypes.h"
#include "NobType.h"
#include "NobIdList.h"
#include "ParameterType.h"
#include "MicroMeterPntVector.h"

extern MicroMeter            const ScrReadMicroMeter         (Script &);
extern MicroMeterPnt         const ScrReadMicroMeterPnt      (Script &);
extern MicroMeterCircle      const ScrReadMicroMeterCircle   (Script &);
extern MicroMeterPntVector   const ScrReadMicroMeterPntVector(Script &);
extern unique_ptr<NobIdList> const ScrReadNobIdList          (Script &);
extern NobId                 const ScrReadNobId              (Script &);
extern NobType               const ScrReadNobType            (Script &);
extern ParamType::Value      const ScrReadParamType          (Script &);