// NNetParameters.h
//
// NNetModel

#pragma once

#include <chrono>
#include "d2d1helper.h"
#include "MoreTypes.h"

using namespace std::chrono;

// dynamic model 
static fHertz          const STD_PULSE_FREQ    { 50.0_fHertz };         // Input neurons

// geometry
static mV              const BASE_POTENTIAL    { 0.0_mV };
static float           const NEURON_INTERIOR   { 0.8f };                // Neurons of all kinds have 80% interior, 20 % border 
static float           const PIPELINE_INTERIOR { 0.6f };                // PIPELINES have 60% interior, 40 % border 
static MicroMeter      const NEURON_RADIUS     { 50.0_MicroMeter };    
static MicroMeter      const PIPELINE_WIDTH    { 20.0_MicroMeter };
static MicroMeterPoint const STD_OFFSET        { MicroMeterPoint( 0._MicroMeter, NEURON_RADIUS * 2.0 ) };

// colors

static D2D1::ColorF const EXT_COLOR_NORMAL         { 0.0f, 0.5f, 1.0f, 1.0f };
static D2D1::ColorF const EXT_COLOR_HIGHLIGHT      { 0.0f, 0.8f, 0.8f, 1.0f };
static D2D1::ColorF const EXT_COLOR_SUPER_HIGHLIGHT{ 1.0f, 0.0f, 0.0f, 1.0f };
static D2D1::ColorF const EXT_COLOR_LOW_KEY        { 0.5f, 0.5f, 0.5f, 1.0f };
static D2D1::ColorF const INT_COLOR_LOW_KEY        { 0.9f, 0.9f, 0.9f, 1.0f };
static D2D1::ColorF const INT_COLOR_EMPHASIZED     { 1.0f, 0.0f, 0.0f, 1.0f };
