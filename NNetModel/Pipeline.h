// Pipeline.h
//
// NNetModel

#pragma once

#include <vector>
#include "MoreTypes.h"
#include "Segment.h"

using std::vector;

class Knot;

class Pipeline
{
public:
	Pipeline( meterPerSec const impulseSpeed )
    :	m_pKnotStart  ( nullptr ),
		m_pKnotEnd    ( nullptr ),
		m_width       ( 10.0_MicroMeter ),
		m_potential   ( ),
		m_impulseSpeed( impulseSpeed )
	{
	}

	void SetStartKnot( Knot * );
	void SetEndKnot  ( Knot * );

	MicroMeterPoint  GetStartPoint( ) const; 
	MicroMeterPoint  GetEndPoint  ( ) const; 
	MicroMeter GetWidth     ( ) const; 

	void Step();

	bool GetSegment( int const, Segment &, mV & ) const;

private:
	MicroMeter distance( MicroMeterPoint const &, MicroMeterPoint const & );

	void initialize( );

	Knot    *   m_pKnotStart;
	Knot    *   m_pKnotEnd;
	MicroMeter  m_width;
	vector<mV>  m_potential;
	meterPerSec m_impulseSpeed;
};
