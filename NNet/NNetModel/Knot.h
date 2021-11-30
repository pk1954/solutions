// Knot.h
//
// NNetModel

#pragma once

#include "Geometry.h"
#include "MoreTypes.h"
#include "tHighlightType.h"
#include "BaseKnot.h"
#include "Pipe.h"

class DrawContext;
class PixelCoordsFp;
class NNetModel;

using std::unique_ptr;
using std::make_unique;

class Knot : public BaseKnot
{
public:
	explicit Knot(MicroMeterPnt const center)
		: BaseKnot(center, NobType::Value::knot, PIPE_WIDTH / 2)
	{}

	explicit Knot(BaseKnot const &);

	~Knot() override = default;

	void Check() const override;

	static bool TypeFits(NobType const type) { return type.IsKnotType(); }

	void      SetDir(Radian const r) override { /* Knot has no direction */ };
	Radian    GetDir       () const  override { return Radian::NULL_VAL(); };
	mV        GetNextOutput() const  override { return m_mVinputBuffer; }
	bool      CompStep     ()        override { return false; }
	NobIoMode GetIoMode    () const  override { return NobIoMode::internal; }

	void DrawExterior(DrawContext const &, tHighlight const) const override;
	void DrawInterior(DrawContext const &, tHighlight const) const override;
	void Recalc      ()  override { };

	virtual void DrawNeuronText(PixelCoordsFp const &) const {};

	void EvaluateSelectionStatus();
};
