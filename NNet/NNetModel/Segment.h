// Segment.h
//
// NNetModel

#pragma once

#include "MoreTypes.h"

class Segment
{
public:
	Segment()
	:	m_startPnt( NP_NULL ),
		m_endPnt  ( NP_NULL ),
		m_width   ( 0.0f )
	{}

	Segment
	(  
		MicroMeterPnt const startPnt,
		MicroMeterPnt const endPnt,
		MicroMeter      const width
	) :
		m_startPnt( startPnt ),
		m_endPnt  ( endPnt   ),
		m_width   ( width )
	{}

	MicroMeterPnt GetStartPoint() const { return m_startPnt; }
	MicroMeterPnt GetEndPoint  () const { return m_endPnt; }
	MicroMeter      GetWidth     () const { return m_width; }

private:
	MicroMeterPnt m_startPnt;
	MicroMeterPnt m_endPnt;
	MicroMeter      m_width;
};
