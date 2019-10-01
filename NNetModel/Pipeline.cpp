// Pipeline.cpp 
//
// NNetModel

#include "stdafx.h"
#include "MoreTypes.h"
#include "Knot.h"
#include "Pipeline.h"

MicroMeter Pipeline::distance( MicroMeterPoint const & npA, MicroMeterPoint const & npB )
{
	MicroMeterPoint  npDiff   = npA - npB;
	MicroMeter distance = MicroMeter
	(
		sqrt
		( 
			npDiff.GetXvalue() * npDiff.GetXvalue() + 
			npDiff.GetYvalue() * npDiff.GetYvalue() 
		)
	);
	return distance;
}

void Pipeline::initialize( )
{
	if ( m_pKnotStart && m_pKnotEnd )
	{
		MicroMeter   const segmentLength  = CoveredDistance( m_impulseSpeed, TIME_RESOLUTION );
		MicroMeter   const pipelineLength = distance( m_pKnotStart->GetPosition(), m_pKnotEnd->GetPosition() );
		unsigned int const iNrOfSegments  = CastToUnsignedInt(round(pipelineLength / segmentLength));

		m_potential.resize( iNrOfSegments, BASE_POTENTIAL );
	}
}

void Pipeline::SetStartKnot( Knot * pKnot )
{
	m_pKnotStart = pKnot;
	initialize();
}

void Pipeline::SetEndKnot( Knot * pKnot )
{
	m_pKnotEnd = pKnot;
	initialize();
}

MicroMeterPoint Pipeline::GetStartPoint( ) const 
{ 
	return m_pKnotStart ? m_pKnotStart->GetPosition() : MicroMeterPoint::NULL_VAL(); 
}

MicroMeterPoint Pipeline::GetEndPoint( ) const 
{ 
	return m_pKnotEnd ? m_pKnotEnd->GetPosition() : MicroMeterPoint::NULL_VAL();
}

MicroMeter Pipeline::GetWidth( ) const 
{ 
	return m_width; 
}

void Pipeline::Step()
{
	for (std::size_t i = m_potential.size() - 1; i > 0; --i )
	{
		m_potential[i] = m_potential[i-1];
	}

	m_potential[0] = m_pKnotStart->GetNextPotential();
}

bool Pipeline::GetSegment( int const iNr, Segment & seg, mV & potential ) const
{
	if ( iNr < m_potential.size() )
	{
		MicroMeterPoint npDiff = GetEndPoint() - GetStartPoint();
		seg = Segment
		{
			GetStartPoint() + (npDiff *  iNr     ) / static_cast<int>(m_potential.size()),
			GetStartPoint() + (npDiff * (iNr + 1)) / static_cast<int>(m_potential.size()),
			m_width
		};
		potential = m_potential[ iNr ];
		assert ( potential < 200.0_mV );
		return true;
	}
	else 
		return false;
}
