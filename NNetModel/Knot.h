// Knot.h
//
// NNetModel

#pragma once

#include <vector>
#include "Geometry.h"
#include "MoreTypes.h"
#include "Shape.h"
#include "Pipeline.h"

using std::vector;

class Knot : public Shape
{
public:
	Knot( MicroMeterPoint const center )
	  : m_center( center ),
		m_extension( 50.0_MicroMeter ),
		m_potential( 0 )
	{
	}

	MicroMeterPoint GetPosition( ) const
	{
		return m_center;
	}

	MicroMeter GetExtension( ) const
	{
		return m_extension;
	}

	void AddIncomming( Pipeline * pPipe )
	{
		m_incomming.push_back( pPipe );
		pPipe->SetEndKnot( this );
	}

	void AddOutgoing( Pipeline * pPipe )
	{
		m_outgoing.push_back( pPipe );
		pPipe->SetStartKnot( this );
	}

	virtual mV GetNextPotential( )
	{
		return m_potential;
	}

	bool IsPointInShape( MicroMeterPoint const & point ) const
	{
		MicroMeterPoint const corner1 = m_center + MicroMeterPoint( + m_extension, + m_extension );
		MicroMeterPoint const corner2 = m_center + MicroMeterPoint( + m_extension, - m_extension );
		MicroMeterPoint const corner3 = m_center + MicroMeterPoint( - m_extension, + m_extension );

		return IsPointInRect< MicroMeterPoint >( point, corner1, corner2, corner3 );
	}

protected:
	mV m_potential;

private:

	vector<Pipeline *> m_incomming;
	vector<Pipeline *> m_outgoing;

	MicroMeterPoint m_center;
	MicroMeter      m_extension;
};