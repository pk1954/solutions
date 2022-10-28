// Knot.ixx  
//
// NNetModel

module;

#include <memory>

export module NNetModel:Knot;

import DrawContext;
import Geometry;
import Types;
import :tHighlight;
import :BaseKnot;

using std::unique_ptr;
using std::make_unique;

export class Knot : public BaseKnot
{
public:
	Knot(MicroMeterPnt const center, NobType const type = NobType::Value::knot)
		: BaseKnot(center, type, KNOT_WIDTH)
	{}

	~Knot() override = default;

	void Check() const override;

	static bool TypeFits(NobType const type) { return type.IsKnotType(); }

	void      SetDir(Radian const r) override { /* Knot has no direction */ };
	Radian    GetDir()        const  override { return Radian::NULL_VAL(); };
	mV        GetNextOutput() const  override { return m_mVinputBuffer; }
	NobIoMode GetIoMode()     const  override { return NobIoMode::internal; }

	void Emphasize(bool const, bool const);

	void DrawExterior(DrawContext const&, tHighlight const) const final;
	void DrawInterior(DrawContext const&, tHighlight const) const final;

	void CollectInput()	override { m_mVinputBuffer = GetFirstIncoming().GetNextOutput(); }
	bool CompStep    () override { return false; }

	void Recalc() override { };
};
