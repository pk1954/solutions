// OutputLine.ixx
//
// NNetModel

module;

#include "Nob.h"
#include "BaseKnot.h"

export module OutputLine;

import tHighlight;
import Types;
import DrawContext;
import IoLine;

export class OutputLine : public IoLine
{
public:

	explicit OutputLine(MicroMeterPnt const&);
	explicit OutputLine(BaseKnot      const&);

	~OutputLine() final = default;

	void Check() const final;

	bool operator==(Nob const&) const override;

	static bool TypeFits(NobType const type) { return type.IsOutputLineType(); }

	void DrawExterior(DrawContext const&, tHighlight const) const override;
	void DrawInterior(DrawContext const&, tHighlight const) const override;

	bool Includes(MicroMeterPnt const&) const final;

	NobIoMode GetIoMode() const final { return NobIoMode::output; }

	void AppendMenuItems(AddMenuFunc const&) const final;

private:

	void drawPlug
	(
		DrawContext  const&,
		float        const,
		float        const,
		D2D1::ColorF const
	) const;
};
