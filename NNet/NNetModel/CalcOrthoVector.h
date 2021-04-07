// CalcOrthoVector.h
//
// NNetModel

#pragma once

#include "MoreTypes.h"
#include "LineType.h"
#include "BaseKnot.h"
#include "ShapePtrList.h"

MicroMeterPoint const CalcOrthoVector(MicroMeterLine const &, ShapePtrList<BaseKnot> const &);
