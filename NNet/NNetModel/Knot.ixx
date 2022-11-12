// Knot.ixx  
//
// NNetModel

module;

#include <memory>
#include <cassert>

export module NNetModel:Knot;

import DrawContext;
import Geometry;
import Types;
import :Nob;
import :PosNob;
import :tHighlight;
import :NobType;

using std::unique_ptr;
using std::make_unique;
 
export class Knot : public PosNob
{
public:
	Knot(MicroMeterPnt const center, NobType const type = NobType::Value::knot)
	  : PosNob(NobType::Value::knot),
		m_circle(center, KNOT_WIDTH)
	{}

	~Knot() = default;

	void Check() const final;

	static bool TypeFits(NobType const type) { return type.IsKnotType(); }

	void SetDir(Radian const r) final { /* Knot has no direction */ };

	size_t GetNrOfInConns () const final { return 1; }
	size_t GetNrOfOutConns() const final { return 1; }

	MicroMeter       GetExtension()  const       { return m_circle.GetRadius(); }
	MicroMeterPnt    GetPos()        const final { return m_circle.GetPos(); }
	Radian           GetDir()        const final { return Radian::NULL_VAL(); };
	mV               GetNextOutput() const final { return m_mVinputBuffer; }
	NobIoMode        GetIoMode()     const final { return NobIoMode::internal; }

	void SetPos(MicroMeterPnt const& pos) final { m_circle.SetPos(pos); }

	void MoveNob  (MicroMeterPnt  const&)               final;
	void RotateNob(MicroMeterPnt  const&, Radian const) final;
	void Link     (Nob const&, Nob2NobFunc const&)      final;

	void AddOutgoing(Pipe& pipe) final { m_pPipeOut = &pipe; }
	void AddIncoming(Pipe& pipe) final { m_pPipeIn  = &pipe; }

	void SetIncoming(Pipe* pPipe) { m_pPipeIn  = pPipe; }
	void SetOutgoing(Pipe* pPipe) { m_pPipeOut = pPipe; }

	void SetAllIncoming(PosNob & src) final { assert(src.IsKnot()); m_pPipeIn  = static_cast<Knot &>(src).m_pPipeIn; }
	void SetAllOutgoing(PosNob & src) final { assert(src.IsKnot()); m_pPipeOut = static_cast<Knot &>(src).m_pPipeOut; }

	void Reconnect() final
	{
		m_pPipeIn ->SetEndPnt  (this);
		m_pPipeOut->SetStartPnt(this);
	};

	void Emphasize(bool const, bool const);

	void ReplaceIncoming(Pipe* const pDel, Pipe* const pAdd) final;
	void ReplaceOutgoing(Pipe* const pDel, Pipe* const pAdd) final;

	void Apply2AllInPipes (PipeFunc const& f) const final { f(*m_pPipeIn); }
	void Apply2AllOutPipes(PipeFunc const& f) const final { f(*m_pPipeOut); }

	bool Apply2AllInPipesB (PipeCrit const& c) const final { return c(*m_pPipeIn); }
	bool Apply2AllOutPipesB(PipeCrit const& c) const final { return c(*m_pPipeOut); }

	void DrawExterior(DrawContext const&, tHighlight const) const final;
	void DrawInterior(DrawContext const&, tHighlight const) const final;

	void CollectInput()	final { m_mVinputBuffer = m_pPipeIn->GetNextOutput(); }
	bool CompStep    () final { return false; }

	void Recalc() final { };

private:
	MicroMeterCircle m_circle;

	Pipe* m_pPipeIn  { nullptr };
	Pipe* m_pPipeOut { nullptr };
};
