// OutputLine.ixx
//
// NNetModel

module;

#include <cassert>

export module NNetModel:OutputLine;

import Types;
import DrawContext;
import :tHighlight;
import :IoLine;
import :PosNob;
import :Nob;

export class OutputLine : public IoLine
{
public:

	explicit OutputLine(MicroMeterPnt const&);
	explicit OutputLine(PosNob const&);
	explicit OutputLine(Pipe &);

	~OutputLine() final = default;

	void Check() const final;
	
	bool operator==(Nob const&) const override;

	static bool TypeFits(NobType const type) { return type.IsOutputLineType(); }

	void CollectInput()	override { m_mVinputBuffer = GetPipeC()->GetNextOutput(); }

	void ReplaceIncoming(Pipe* const pDel, Pipe* const pAdd) final;
	void ReplaceOutgoing(Pipe* const pDel, Pipe* const pAdd) final { assert(false); }

	void Apply2AllInPipes (PipeFunc const& f) const final {};
	bool Apply2AllInPipesB(PipeCrit const& c) const final { return false; }

	void SetAllOutgoing(PosNob&) final;
	void SetAllIncoming(PosNob&) final;

	void AddIncoming(Pipe& pipe) final { SetPipe(&pipe); }

	void DrawExterior(DrawContext const&, tHighlight const) const override;
	void DrawInterior(DrawContext const&, tHighlight const) const override;

	bool Includes(MicroMeterPnt const&) const final;

	NobIoMode GetIoMode() const final { return NobIoMode::output; }

	void Reconnect() final;

	void AppendMenuItems(AddMenuFunc const&) const final;

	size_t GetNrOfInConns () const final { return 1; }
	size_t GetNrOfOutConns() const final { return 0; }

private:

	void drawPlug
	(
		DrawContext  const&,
		float        const,
		float        const,
		D2D1::ColorF const
	) const;
};

export OutputLine const* Cast2OutputLine(Nob const*);
export OutputLine      * Cast2OutputLine(Nob      *);
