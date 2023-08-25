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
	explicit Knot(MicroMeterPnt const&);
	explicit Knot(PosNob const&);

	~Knot() = default;

	void Check() const final;

	static bool TypeFits(NobType const type) { return type.IsKnotType(); }

	void AppendMenuItems(AddMenuFunc const&) const final;

	size_t GetNrOfInConns () const final { return 1; }
	size_t GetNrOfOutConns() const final { return 1; }

	MicroMeter    GetExtension() const       { return m_circle.GetRadius(); }
	MicroMeterPnt GetPos()       const final { return m_circle.GetPos(); }
	Radian        GetDir()       const final { return Radian::NULL_VAL(); };
	mV            GetPotential() const final { return m_mVpotential; }
	NobIoMode     GetIoMode()    const final { return NobIoMode::internal; }

	Pipe* GetIncoming() { return m_pPipeIn; }
	Pipe* GetOutgoing() { return m_pPipeOut; }

	Pipe const * GetIncoming() const { return m_pPipeIn; }
	Pipe const * GetOutgoing() const { return m_pPipeOut; }

	void Recalc     ()                                   final;
	void SetPosNoFix(MicroMeterPnt const&)               final;
	void MoveNob    (MicroMeterPnt const&)               final;
	void RotateNob  (MicroMeterPnt const&, Radian const) final;
	void Link       (Nob const&, Nob2NobFunc const&)     final;

	void AddOutgoing(Pipe* pPipe) final { m_pPipeOut = pPipe; }
	void AddIncoming(Pipe* pPipe) final { m_pPipeIn  = pPipe; }

	void SetAllIncoming(PosNob&)        final;
	void SetAllOutgoing(PosNob&)        final;
	void Reconnect()                    final;
	void SelectAllConnected(bool const) final;

	void Emphasize(bool const, bool const);

	void ReplaceIncoming(Pipe* const pDel, Pipe* const pAdd) final;
	void ReplaceOutgoing(Pipe* const pDel, Pipe* const pAdd) final;

	void Apply2AllInPipes (PipeFunc const& f) final { f(*m_pPipeIn); }
	void Apply2AllOutPipes(PipeFunc const& f) final { f(*m_pPipeOut); }

	void Apply2AllInPipesC (PipeFuncC const& f) const final { f(*m_pPipeIn); }
	void Apply2AllOutPipesC(PipeFuncC const& f) const final { f(*m_pPipeOut); }

	bool Apply2AllInPipesB (PipeCrit const& c) const final { return c(*m_pPipeIn); }
	bool Apply2AllOutPipesB(PipeCrit const& c) const final { return c(*m_pPipeOut); }

	void DrawExterior(DrawContext const&, tHighlight const) const final;
	void DrawInterior(DrawContext const&, tHighlight const) const final;

	void CollectInput()	final { m_mVpotential = m_pPipeIn->GetPotential(); }
	bool CompStep    () final { return false; }

private:
	MicroMeterCircle m_circle;

	Pipe* m_pPipeIn  { nullptr };
	Pipe* m_pPipeOut { nullptr };
};

export Knot const* Cast2Knot(Nob const*);
export Knot      * Cast2Knot(Nob      *);
