// Neuron.ixx
//
// NNetModel

module;

#include <cassert>
#include <string>
#include <chrono>

export module NNetModel:Neuron;

import BoolOp;
import Types;
import DrawContext;
import :tHighlight;
import :Spike;
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
	Neuron(Neuron const&);

	Neuron& operator=(Neuron const&); // copy assignment operator

	~Neuron() override = default;

	void CollectInput()  final;
	bool CompStep()      final;
	void Reconnect()     final;

	void AppendMenuItems(AddMenuFunc const &) const override;

	static bool TypeFits(NobType const type) { return type.IsNeuronType(); }

	void StopOnTrigger(tBoolOp const op) { ApplyOp(m_bStopOnTrigger, op); }

	void          SetDir(Radian const)  final { /* empty */ };
	void          ClearDynamicData()    final;
	mV            GetNextOutput() const final;
	MicroMeter    GetExtension () const final { return m_circle.GetRadius(); }
	MicroMeterPnt GetPos()        const final { return m_circle.GetPos(); }
	Radian        GetDir()        const final { return Radian::NULL_VAL(); };
	NobIoMode     GetIoMode()     const final { return NobIoMode::internal; }

	void SetPos   (MicroMeterPnt const&)               final;
	void MoveNob  (MicroMeterPnt const&)               final;
	void RotateNob(MicroMeterPnt const&, Radian const) final;
	void Link(Nob const&, Nob2NobFunc const&)          final;
	void SelectAllConnected(bool const bFirst)         final
	{
		if (!IsSelected() || bFirst)
		{
			Nob::Select(true);
			m_inPipes.SelectAllConnected();
			m_pPipeAxon->SelectAllConnected(false);
		}
	}

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

protected:

	void DisplayText(DrawContext const&, MicroMeterRect const&, wstring const&) const;

private:
	MicroMeterCircle m_circle;

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
	assert(pNob);
	assert(pNob->IsNeuron());
	return static_cast<Neuron const*>(pNob);
}

export Neuron* Cast2Neuron(Nob* pNob)
{
	assert(pNob);
	assert(pNob->IsNeuron());
	return static_cast<Neuron*>(pNob);
}
