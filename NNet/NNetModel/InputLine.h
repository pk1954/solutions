// InputLine.h
//
// NNetModel

#pragma once

#include "Nob.h"
#include "BaseKnot.h"
#include "SignalGenerator.h"

import Direct2D;
import tHighlight;
import MoreTypes;
import DrawContext;
import SigGenDynamicData;
import IoLine;
import NobType;

class InputLine : public IoLine
{
public:

	explicit InputLine(SignalGenerator *, MicroMeterPnt const &);
	explicit InputLine(SignalGenerator *, BaseKnot      const &);

	~InputLine() final = default;

	void Check() const final;

	void SetSigGen(SignalGenerator * const p)  { m_pSigGen = p; }
	SignalGenerator       * GetSigGen ()       { return m_pSigGen; }
	SignalGenerator const * GetSigGenC() const { return m_pSigGen; }

	static bool TypeFits(NobType const type) { return type.IsInputLineType(); }

	void Prepare() final;

	void DrawExterior(DrawContext const &, tHighlight const) const final;
	void DrawInterior(DrawContext const &, tHighlight const) const final;

	NobIoMode GetIoMode() const final { return NobIoMode::input; }

	bool Includes(MicroMeterPnt const &) const final;

	void AppendMenuItems(AddMenuFunc const &) const final;

private:

	SignalGenerator * m_pSigGen { nullptr };

	MicroMeterPnt getOffset() const;
	MicroMeterPnt getCenter() const;

	void drawSocket
	(
		DrawContext  const &, 
		float        const, 
		float        const, 
		D2D1::ColorF const
	) const;
};	
