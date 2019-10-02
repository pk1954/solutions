// Neuron.h
//
// NNetModel

#pragma once

#include <chrono>
#include "MoreTypes.h"
#include "Shape.h"
#include "Knot.h"

using std::chrono::microseconds;

class Neuron : public Knot
{
public:
	Neuron( MicroMeterPoint const npCenter )
	  : Knot( npCenter )
	{ 
	}

private:
};