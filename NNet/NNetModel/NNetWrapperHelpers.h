// NNetWrapperHelpers.h : 
//
// NNetWindows

#pragma once

#include "script.h"
#include "moretypes.h"
#include "NobIdList.h"
#include "MicroMeterPntVector.h"

extern MicroMeter            const ScrReadMicroMeter         (Script &);
extern MicroMeterPnt         const ScrReadMicroMeterPnt      (Script &);
extern MicroMeterCircle      const ScrReadMicroMeterCircle   (Script &);
extern MicroMeterPntVector   const ScrReadMicroMeterPntVector(Script &);
extern unique_ptr<NobIdList> const ScrReadNobIdList          (Script &);
extern NobId                 const ScrReadNobId              (Script &);
