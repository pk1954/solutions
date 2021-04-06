// CalcOrthoVector.h
//
// NNetModel

#pragma once

#include "MoreTypes.h"
#include "LineType.h"
#include "ShapePtrList.h"
#include "ConnectionNeuron.h"

MicroMeterPoint const CalcOrthoVector(MicroMeterLine const &, ShapePtrList<ConnNeuron> const &);
