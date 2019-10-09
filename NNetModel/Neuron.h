// Neuron.h
//
// NNetModel

#pragma once

#include <chrono>
#include "MoreTypes.h"
#include "Shape.h"
#include "BaseKnot.h"

using std::chrono::microseconds;

class Neuron : public BaseKnot
{
public:
	Neuron( MicroMeterPoint const npCenter )
	  : BaseKnot( tShapeType::neuron, npCenter, 50.0_MicroMeter )
	{ 
	}

private:
};
