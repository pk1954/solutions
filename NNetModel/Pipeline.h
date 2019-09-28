// Pipeline.h
//
// NNetModel

#pragma once

#include <vector>
#include "NNetPoint.h"
#include "NNetTypes.h"
#include "Segment.h"

using std::vector;

class Knot;

class Pipeline
{
public:
	Pipeline( )
    :	m_pKnotStart( nullptr ),
		m_pKnotEnd  ( nullptr ),
		m_width     ( 10.0_MicroMeter ),
		m_potential ( )
	{
	}

	void SetStartKnot( Knot * );
	void SetEndKnot  ( Knot * );

	NNetPoint  GetStartPoint( ) const; 
	NNetPoint  GetEndPoint  ( ) const; 
	MicroMeter GetWidth     ( ) const; 

	//void Start( );
	void Step();

	bool GetSegment( int const, Segment &, mV & ) const;

private:
	MicroMeter distance( NNetPoint const &, NNetPoint const & );

	void initialize();

	Knot    *  m_pKnotStart;
	Knot    *  m_pKnotEnd;
	MicroMeter m_width;
	vector<mV> m_potential;
};
