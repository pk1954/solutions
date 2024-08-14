// Neuron.ixx
//
// NNetModel

export module NNetModel:Neuron;

import std;
import BoolOp;
import Types;
import DrawContext;
import :tHighlight;
import :NobType;
import :PipeList;
import :OutputLine;
import :PosNob;

using std::wstring;
using std::unique_ptr;
using std::make_unique;

export class Neuron : public PosNob
{
public:
	Neuron(MicroMeterPnt const&);

	~Neuron() override = default;

	void CollectInput()  final;
	bool CompStep()      final;
	void Reconnect()     final;

	void AppendMenuItems(AddMenuFunc const &) const override;

	static bool TypeFits(NobType const type) { return type.IsNeuronType(); }

	void StopOnTrigger(tBoolOp const op) { ApplyOp(m_bStopOnTrigger, op); }

	void          ClearDynamicData()   final;
	mV            GetPotential() const final;
	MicroMeter    GetExtension() const final { return NEURON_RADIUS; }
	MicroMeterPnt GetPos()       const final { return m_pos; }
	Radian        GetDir()       const final { return Radian::NULL_VAL(); };
	NobIoMode     GetIoMode()    const final { return NobIoMode::internal; }
	NobType       GetNobType()   const final { return NobType::Value::neuron; }

	void Recalc     ()                                   final;
	void SetPosNoFix(MicroMeterPnt const&)               final;
	void MoveNob    (MicroMeterPnt const&)               final;
	void RotateNob  (MicroMeterPnt const&, Radian const) final;
	void Link(Nob const&, Nob2NobFunc const&)            final;
	void SelectAllConnected(bool const)                  final;

	size_t GetNrOfOutConns() const final { return 1; }
	size_t GetNrOfInConns () const final { return m_inPipes.Size(); }

	void Apply2AllInPipes (PipeFunc  const& f)       final { m_inPipes.Apply2All(f); }
	void Apply2AllInPipesC(PipeFuncC const& f) const final { m_inPipes.Apply2All(f); }
	bool Apply2AllInPipesB(PipeCrit  const& c) const final { return m_inPipes.Apply2AllB(c); }

	void Apply2AllOutPipes (PipeFunc  const& f)       final { f(*m_pPipeAxon); }
	void Apply2AllOutPipesC(PipeFuncC const& f) const final { f(*m_pPipeAxon); }
	bool Apply2AllOutPipesB(PipeCrit  const& c) const final { return c(*m_pPipeAxon); }

	void RemoveIncoming(Pipe* const);

	void ReplaceIncoming(Pipe* const, Pipe* const) final;
	void ReplaceOutgoing(Pipe* const, Pipe* const) final;

	void AddOutgoing(Pipe* pPipe) final { SetAxon(pPipe); }
	void AddIncoming(Pipe* pPipe) final { m_inPipes.Add(*pPipe); }

	void AddIncoming(Neuron const *);

	void DrawExterior(DrawContext const&, tHighlight const) const final;
	void DrawInterior(DrawContext const&, tHighlight const) const final;

	void SetAxon(Pipe* pAxon) { m_pPipeAxon = pAxon; }

	void SetAllIncoming(PosNob &) final;
	void SetAllOutgoing(PosNob &) final;

	Pipe const * GetAxon() const { return m_pPipeAxon; }
	Pipe       * GetAxon()       { return m_pPipeAxon; }

	Pipe* GetFirstIncoming() { return &m_inPipes.GetFirst(); }

	Pipe const * GetIncoming(size_t i) const { return &m_inPipes.Get(i); }

    static unsigned int Size() { return sizeof(Neuron); }

private:
//	MicroMeterCircle m_circle;
	MicroMeterPnt m_pos;

	PipeList m_inPipes;
	Pipe   * m_pPipeAxon { nullptr };

	mutable bool m_bTriggered { false };

	bool       m_bStopOnTrigger { false };
	fMicroSecs m_usSpikeTime    { 0._MicroSecs };

	MicroMeterPnt getAxonHillockPos() const;

	void init(const Neuron&);
};

export Neuron const* Cast2Neuron(Nob const* pNob)
{
	Assert(pNob);
	Assert(pNob->IsNeuron());
	return static_cast<Neuron const*>(pNob);
}

export Neuron* Cast2Neuron(Nob* pNob)
{
	Assert(pNob);
	Assert(pNob->IsNeuron());
	return static_cast<Neuron*>(pNob);
}
