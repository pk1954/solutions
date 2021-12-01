// InputNeuron.cpp 
//
// NNetModel

#include "stdafx.h"
#include <iomanip>
#include <sstream> 
#include "Resource.h"
#include "DrawContext.h"
#include "tHighlightType.h"
#include "ParameterType.h"
#include "NobType.h"
#include "NNetColors.h"
#include "NNetParameters.h"
#include "Knot.h"
#include "InputConnector.h"
#include "InputNeuron.h"

using std::chrono::microseconds;
using std::wostringstream;
using std::setprecision;
using std::wstring;
using std::fixed;

InputNeuron::InputNeuron(MicroMeterPnt const & upCenter)
	: IoNeuron(upCenter, NobType::Value::inputNeuron)
{ }

InputNeuron::InputNeuron(BaseKnot const & baseKnot)
	: IoNeuron(baseKnot, NobType::Value::inputNeuron)
{ 
	SetOutgoing(baseKnot);
}

void InputNeuron::Check() const
{
	Neuron::Check();
	assert(!HasIncoming());
}

fHertz InputNeuron::GetActFrequency() const
{
	return HasParentNob()
		? static_cast<InputConnector *>(GetParentNob())->GetActFrequency()
		: m_pParameters->BaseFrequency();
}

void InputNeuron::Prepare()
{
	if (HasParentNob())
		static_cast<InputConnector *>(GetParentNob())->Tick();
	fHertz     const freq            { GetActFrequency() };
	fMicroSecs const time2Trigger    { PulseDuration(freq) };
	float      const ticks2Trigger   { time2Trigger / m_pParameters->TimeResolution() };
	mV         const increasePerTick { m_pParameters->Threshold() / ticks2Trigger };
	m_mVinputBuffer += increasePerTick;
}

void InputNeuron::DrawExterior(DrawContext const & context, tHighlight const type) const
{
	drawSocket(context, 2.0f, 0.1f, GetExteriorColor(type));
}

void InputNeuron::DrawInterior(DrawContext const & context, tHighlight const type) const
{
	drawSocket(context, 1.6f, 0.0f, GetInteriorColor(type));
}

MicroMeterPnt InputNeuron::getOffset() const
{
	return GetScaledDirVector() * 0.7f;
}

MicroMeterPnt InputNeuron::getCenter() const
{
	return GetPos() - getOffset();
}

bool InputNeuron::Includes(MicroMeterPnt const & point) const
{
	return Distance(point, getCenter()) <= GetExtension();
}

void InputNeuron::drawSocket
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

void InputNeuron::AppendMenuItems(AddMenuFunc const & add) const
{
	if (! HasOutgoing())
		add(IDD_ADD_OUTGOING2KNOT);
	IoNeuron::AppendMenuItems(add);
}
