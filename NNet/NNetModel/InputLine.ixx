// InputLine.ixx
//
// NNetModel

module;

#include <cassert>

export module NNetModel:InputLine;

import Direct2D;
import Types;
import DrawContext;
import Signals;
import :tHighlight;
import :Nob;
import :NobType;
import :Neuron;
import :IoLine;
import :PosNob;
import :SignalGenerator;
import :StdSigGen;

export class InputLine : public IoLine
{
public:

	explicit InputLine(MicroMeterPnt const&);
	explicit InputLine(PosNob&);
	explicit InputLine(Pipe&);

	~InputLine() final = default;

	void Check() const final;

	void SetSigGen(SignalGenerator* const p) { m_pSigGen = p; }

	SignalGenerator      * GetSigGen()        { return m_pSigGen; }
	SignalGenerator const* GetSigGenC() const { return m_pSigGen; }

	static bool TypeFits(NobType const type) { return type.IsInputLineType(); }

	void CollectInput() final;

	void ReplaceIncoming(Pipe* const pDel, Pipe* const pAdd) final { assert(false); };
	void ReplaceOutgoing(Pipe* const pDel, Pipe* const pAdd) final;

	void Apply2AllInPipes (PipeFunc  const& f)       final {};
	void Apply2AllInPipesC(PipeFuncC const& f) const final {};
	bool Apply2AllInPipesB(PipeCrit  const& c) const final { return false; }

	void Apply2AllOutPipes (PipeFunc  const& f)       final { f(*GetPipe ()); }
	void Apply2AllOutPipesC(PipeFuncC const& f) const final { f(*GetPipeC()); }
	bool Apply2AllOutPipesB(PipeCrit  const& c) const final { return c(*GetPipeC());	}

	void SetAllIncoming(PosNob&) final;
	void SetAllOutgoing(PosNob&) final;
	void SetAllOutgoing(Neuron&);

	void AddOutgoing(Pipe * pPipe) final { SetPipe(pPipe); }

	void DrawExterior(DrawContext const&, tHighlight const) const final;
	void DrawInterior(DrawContext const&, tHighlight const) const final;

	NobIoMode GetIoMode() const final { return NobIoMode::input; }

	void Reconnect() final;

	void AppendMenuItems(AddMenuFunc const&) const final;

	size_t GetNrOfInConns () const final { return 0; }
	size_t GetNrOfOutConns() const final { return 1; }

private:

	SignalGenerator* m_pSigGen { StdSigGen::Get() };

	MicroMeterPnt getOffset() const;
	MicroMeterPnt getCenter() const;

	void drawSocket
	(
		DrawContext const&,
		float       const,
		float       const,
		Color       const
	) const;
};

export InputLine const* Cast2InputLine(Nob const*);
export InputLine      * Cast2InputLine(Nob      *);
