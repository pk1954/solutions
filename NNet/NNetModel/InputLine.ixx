// InputLine.ixx
//
// NNetModel

export module NNetModel:InputLine;

import Direct2D;
import Types;
import DrawContext;
import :tHighlight;
import :SigGenDynamicData;
import :Nob;
import :NobType;
import :IoLine;
import :BaseKnot;
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

	void DrawExterior(DrawContext const&, tHighlight const) const final;
	void DrawInterior(DrawContext const&, tHighlight const) const final;

	NobIoMode GetIoMode() const final { return NobIoMode::input; }

	bool Includes(MicroMeterPnt const&) const final;

	void AppendMenuItems(AddMenuFunc const&) const final;

	Pipe & GetPipe()     final { return GetFirstOutgoing(); }
	void   ConnectPipe() final { GetPipe().SetStartKnot(this); };

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
