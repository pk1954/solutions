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
	Knot(MicroMeterPnt const center)
		: BaseKnot(center, NobType::Value::knot, PIPE_WIDTH / 2)
	{}

	Knot(BaseKnot const &);

	virtual ~Knot() {}

	void Check() const;

	static bool const TypeFits(NobType const type) { return type.IsKnotType(); }

	virtual void            SetDir(Radian const r) { };
	virtual Radian    const GetDir       () const { return Radian::NULL_VAL(); };
	virtual mV        const GetNextOutput() const { return m_mVinputBuffer; }
	virtual bool      const CompStep     ()       { return false; }
	virtual NobIoMode const GetIoMode    () const { return NobIoMode::internal; }

	virtual void DrawExterior(DrawContext const &, tHighlight const) const;
	virtual void DrawInterior(DrawContext const &, tHighlight const) const;
	virtual void Recalc      () { };

	virtual void DrawNeuronText(PixelCoordsFp const &) const {};

	void EvaluateSelectionStatus();

private: 
};
