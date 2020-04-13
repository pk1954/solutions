// NNetParameters.h
//
// NNetModel

#pragma once

#include <chrono>
#include "d2d1helper.h"
#include "MoreTypes.h"

using namespace std::chrono;

// dynamic model 
static fHertz          const STD_PULSE_FREQ  { 50.0_fHertz };         // Input neurons

// geometry
static mV              const BASE_POTENTIAL  { 0.0_mV };
static float           const NEURON_INTERIOR { 0.8f };                // Neurons of all kinds have 80% interior, 20 % border 
static float           const PIPE_INTERIOR   { 0.6f };                // Pipes have 60% interior, 40 % border 
static MicroMeter      const NEURON_RADIUS   { 50.0_MicroMeter };    
static MicroMeter      const PIPE_WIDTH      { 20.0_MicroMeter };
static MicroMeterPoint const STD_OFFSET      { MicroMeterPoint( 0._MicroMeter, NEURON_RADIUS * 2.0 ) };
