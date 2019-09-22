// NNetPoint.h : 
//
// NNetModel

#pragma once

#include <functional>
#include "NNetCoord.h"
#include "PointType.h"

using NNetPoint = PosType< nm >;

static NNetPoint const NP_NULL( NNetPoint::NULL_VAL() );   // compiler generates call!
static NNetPoint const NP_ZERO( NNetPoint::ZERO_VAL() );   // compiler generates call!

using NNetPointFunc     = std::function<void (NNetPoint const)>;
using NNetPointBoolFunc = std::function<bool (NNetPoint const)>;
