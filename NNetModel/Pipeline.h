// Pipeline.h
//
// NNetModel

#pragma once

#include <vector>
#include "NNetPoint.h"
#include "Segment.h"

using std::vector;

class Pipeline
{
public:
	Pipeline
	(  
		NNetPoint const npStart,
		NNetPoint const npEnd,
		NanoMeter const nmWidth
	) :
		m_npStart( npStart ),
		m_npEnd  ( npEnd   ),
		m_nmWidth( nmWidth ),
		m_dWave( 0.0 ),
		m_potential( NR_OF_ELEMENTS, BASE_POTENTIAL )
	{
	}

	NNetPoint GetStartPoint( ) const { return m_npStart; }
	NNetPoint GetEndPoint  ( ) const { return m_npEnd; }
	NanoMeter GetWidth     ( ) const { return m_nmWidth; }

	void Start( )
	{
		m_dWave = 0.0;
		std::fill( m_potential.begin(), m_potential.end(), BASE_POTENTIAL );
	}

	void Step()
	{
		for (std::size_t i = NR_OF_ELEMENTS - 1; i > 0; --i )
		{
			m_potential[i] = m_potential[i-1];
		}
		double dY = m_dWave / ( m_dWave * m_dWave + 1.0 );
		m_potential[0] = BASE_POTENTIAL + CastToInt( 256.0 * dY );
		m_dWave += 0.1;
	}

	bool GetSegment( unsigned int const uiNr, Segment & seg, int & iPotential ) const
	{
		if ( uiNr < NR_OF_ELEMENTS )
		{
			NNetPoint npDiff = m_npEnd - m_npStart;
			seg = Segment
			{
				m_npStart + (npDiff *  uiNr     ) / NR_OF_ELEMENTS,
				m_npStart + (npDiff * (uiNr + 1)) / NR_OF_ELEMENTS,
				m_nmWidth
			};
			iPotential = m_potential[ uiNr ];
			return true;
		}
		else 
			return false;
	}

private:
	static int const NR_OF_ELEMENTS = 2000;
	static int const BASE_POTENTIAL = 32;

	NNetPoint m_npStart;
	NNetPoint m_npEnd;
	NanoMeter m_nmWidth;
	double    m_dWave;
	vector<unsigned int> m_potential;
};
