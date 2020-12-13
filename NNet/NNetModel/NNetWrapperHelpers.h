// NNetWrapperHelpers.h : 
//
// NNetWindows

#pragma once

#include "script.h"
#include "ShapeId.h"
#include "moretypes.h"

extern ShapeId          ScrReadShapeId         ( Script & );
extern MicroMeter       ScrReadMicroMeter      ( Script & );
extern MicroMeterPoint  ScrReadMicroMeterPoint ( Script & );
extern MicroMeterCircle ScrReadMicroMeterCircle( Script & );
