// InputLine.cpp 
//
// NNetModel

module;

#include <cassert>
#include <chrono>
#include <iomanip>
#include <sstream> 
#include "Resource.h"

module NNetModel:InputLine;

import Direct2D;
import DrawContext;
import SimulationTime;
import :tHighlight;
import :SignalGenerator;
import :StdSigGen;
import :ParamType;
import :NNetColors;
import :NNetParameters;
import :NobType;
import :IoLine;
import :BaseKnot;

using std::chrono::microseconds;
using std::wostringstream;
using std::setprecision;
using std::wstring;
using std::fixed;

InputLine::InputLine(MicroMeterPnt const& upCenter)
  : IoLine(upCenter, NobType::Value::inputLine),
	m_pSigGen(StdSigGen::Get())
{ }

void InputLine::Check() const
{
	IoLine::Check();
	assert(!HasIncoming());
}

void InputLine::DrawExterior(DrawContext const & context, tHighlight const type) const
{
	drawSocket(context, 2.0f, 0.1f, GetExteriorColor(type));
}

void InputLine::DrawInterior(DrawContext const & context, tHighlight const type) const
{
	drawSocket(context, 1.6f, 0.0f, GetInteriorColor(type));
}

MicroMeterPnt InputLine::getOffset() const
{
	return GetScaledDirVector() * 0.7f;
}

MicroMeterPnt InputLine::getCenter() const
{
	return GetPos() - getOffset();
}

bool InputLine::Includes(MicroMeterPnt const & point) const
{
	return Distance(point, getCenter()) <= GetExtension();
}

void InputLine::drawSocket
(
	DrawContext  const & context, 
	float        const   M,       // overall width/height                        
	float        const   VEM,     // vertical offset of end point middle section 
	D2D1::ColorF const   colF
) const
{
	MicroMeterPnt  const umDirVector   { GetDirVector() * M };
	MicroMeter     const umSize        { GetExtension() * M };
	MicroMeterPnt  const umOrthoVector { getOffset().OrthoVector() };
	MicroMeterPnt  const umDirVectorVS { umDirVector * 0.5f };
	MicroMeter     const umWidthLR     { umSize - GetExtension() * 1.4f };  // width of left/right section                 
	MicroMeterPnt  const umCenter      { getCenter() };
	MicroMeterPnt  const umStart       { umCenter + umDirVectorVS };
	MicroMeterPnt  const umEndLR       { umCenter - umDirVectorVS };
	MicroMeterPnt  const umEndCenter   { umCenter - umDirVector * VEM };
	MicroMeterLine const umLine        { umStart, umEndLR };

	context.DrawLine(umStart, umEndCenter,   umSize,    colF);
	context.DrawLine(umLine + umOrthoVector, umWidthLR, colF);
	context.DrawLine(umLine - umOrthoVector, umWidthLR, colF);
}

void InputLine::CollectInput()
{
	m_mVinputBuffer = m_pSigGen->GetSignalValue();
}

void InputLine::AppendMenuItems(AddMenuFunc const & add) const
{
	add(IDD_EXTEND_INPUTLINE);   // case 10
	IoLine::AppendMenuItems(add);
}
