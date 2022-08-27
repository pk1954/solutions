// Knot.h
//
// NNetModel

#pragma once

#include "Nob.h"
#include "BaseKnot.h"
#include "Pipe.h"

import Geometry;
import BasicTypes;
import MoreTypes;
import AngleTypes;
import tHighlight;

class DrawContext;
class NNetModel;

using std::unique_ptr;
using std::make_unique;

class Knot : public BaseKnot
{
public:
	explicit Knot(MicroMeterPnt const center)
	  : BaseKnot(center, NobType::Value::knot, KNOT_WIDTH)
	{}

	explicit Knot(BaseKnot const &);

	~Knot() override = default;

	void Check() const override;

	void AppendMenuItems(AddMenuFunc const &) const final;

	static bool TypeFits(NobType const type) { return type.IsKnotType(); }

	void      SetDir(Radian const r) final { /* Knot has no direction */ };
	Radian    GetDir       () const  final { return Radian::NULL_VAL(); };
	mV        GetNextOutput() const  final { return m_mVinputBuffer; }
	bool      CompStep     ()        final { return false; }
	NobIoMode GetIoMode    () const  final { return NobIoMode::internal; }

	void DrawExterior(DrawContext const &, tHighlight const) const final;
	void DrawInterior(DrawContext const &, tHighlight const) const final;
	void Emphasize   (bool const, bool const);

	void Recalc() final { };

	void EvaluateSelectionStatus();

private:

	inline static MicroMeter const KNOT_WIDTH { PIPE_WIDTH / 2 };
};
