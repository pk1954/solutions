// NNetParameters.h
//
// NNetModel

#pragma once

#include <chrono>
#include "MoreTypes.h"

using namespace std::chrono;

static microseconds const TIME_RESOLUTION   = microseconds( 100 );
static mV           const PEAK_VOLTAGE      = 100._mV;
static microseconds const PEAK_TIME         = 2ms;
static meterPerSec  const STD_IMPULSE_SPEED = 20._meterPerSec;     // Adjustable per pipeline
static mV           const BASE_POTENTIAL    = 0.0_mV;
