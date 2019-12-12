// NNetParameters.h
//
// NNetModel

#pragma once

#include <chrono>
#include "MoreTypes.h"

using namespace std::chrono;

static MicroSecs       const TIME_RESOLUTION   { 100._MicroSecs };

// dynamic model 
static fHertz          const STD_PULSE_FREQ    { 50.0_fHertz };         // Input neurons

// geometry
static mV              const BASE_POTENTIAL    { 0.0_mV };
static float           const NEURON_INTERIOR   { 0.8f };                // Neurons of all kinds have 80% interior, 20 % border 
static float           const PIPELINE_INTERIOR { 0.6f };                // PIPELINES have 60% interior, 40 % border 
static MicroMeter      const NEURON_RADIUS     { 50.0_MicroMeter };    
static MicroMeter      const PIPELINE_WIDTH    { 10.0_MicroMeter };
static MicroMeterPoint const STD_OFFSET        { MicroMeterPoint( 0._MicroMeter, NEURON_RADIUS * 2.0 ) };

// colors
static COLORREF const EXT_COLOR_NORMAL         { RGB(   0, 127, 255 ) };
static COLORREF const EXT_COLOR_HIGHLIGHT      { RGB(   0, 200, 200 ) };
static COLORREF const EXT_COLOR_SUPER_HIGHLIGHT{ RGB( 255,   0,   0 ) };
