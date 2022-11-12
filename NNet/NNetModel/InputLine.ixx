// InputLine.ixx
//
// NNetModel

module;

#include <cassert>

export module NNetModel:InputLine;

import Direct2D;
import Types;
import DrawContext;
import :tHighlight;
import :SigGenDynamicData;
import :Nob;
import :NobType;
import :IoLine;
import :PosNob;
import :SignalGenerator;

export class InputLine : public IoLine
{
public:

	InputLine(MicroMeterPnt const &);
	~InputLine() final = default;

	void Check() const final;

	void SetSigGen(SignalGenerator* const p) { m_pSigGen = p; }

	SignalGenerator       * GetSigGen()        { return m_pSigGen; }
	SignalGenerator const * GetSigGenC() const { return m_pSigGen; }

	static bool TypeFits(NobType const type) { return type.IsInputLineType(); }

	void CollectInput() final;

	void ReplaceIncoming(Pipe* const pDel, Pipe* const pAdd) final { assert(false); };
	void ReplaceOutgoing(Pipe* const pDel, Pipe* const pAdd) final;

	void Apply2AllInPipes (PipeFunc const& f) const final {};
	bool Apply2AllInPipesB(PipeCrit const& c) const final { return false; }

	void SetAllIncoming(PosNob & src) final { assert(false); }
	void SetAllOutgoing(PosNob & src) final 
	{ 
		assert(src.IsInputLine()); 
		SetPipe(static_cast<InputLine *>(&src)->GetPipe()); 
	}
	
	void AddOutgoing(Pipe & pipe) final { SetPipe(&pipe); }

	void DrawExterior(DrawContext const&, tHighlight const) const final;
	void DrawInterior(DrawContext const&, tHighlight const) const final;

	NobIoMode GetIoMode() const final { return NobIoMode::input; }

	void Reconnect() final;

	void AppendMenuItems(AddMenuFunc const&) const final;

	size_t GetNrOfInConns () const final { return 0; }
	size_t GetNrOfOutConns() const final { return 1; }

private:

	SignalGenerator * m_pSigGen;

	MicroMeterPnt getOffset() const;
	MicroMeterPnt getCenter() const;

	void drawSocket
	(
		DrawContext  const&,
		float        const,
		float        const,
		D2D1::ColorF const
	) const;
};
