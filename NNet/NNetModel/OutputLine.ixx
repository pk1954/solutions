// OutputLine.ixx
//
// NNetModel

export module NNetModel:OutputLine;

import Types;
import DrawContext;
import :tHighlight;
import :IoLine;
import :BaseKnot;
import :Nob;

export class OutputLine : public IoLine
{
public:

	explicit OutputLine(MicroMeterPnt const&);
	//explicit OutputLine(BaseKnot      const&);

	~OutputLine() final = default;

	void Check() const final;
	
	bool operator==(Nob const&) const override;

	static bool TypeFits(NobType const type) { return type.IsOutputLineType(); }

	void CollectInput()	override { m_mVinputBuffer = GetFirstIncoming().GetNextOutput(); }

	void DrawExterior(DrawContext const&, tHighlight const) const override;
	void DrawInterior(DrawContext const&, tHighlight const) const override;

	bool Includes(MicroMeterPnt const&) const final;

	NobIoMode GetIoMode() const final { return NobIoMode::output; }

	void AppendMenuItems(AddMenuFunc const&) const final;

	Pipe & GetPipe()     final { return GetFirstIncoming(); }
	void   ConnectPipe() final { GetPipe().SetEndKnot(this); };

private:

	void drawPlug
	(
		DrawContext  const&,
		float        const,
		float        const,
		D2D1::ColorF const
	) const;
};
