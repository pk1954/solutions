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

class GraphicsInterface;
class PixelCoordsFp;

class Knot : public Shape
{
public:
	Knot
	( 
		MicroMeterPoint const center,
		tShapeType      const type = tShapeType::knot
	)
	  : Shape( type ),
		m_center( center ),
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

	virtual void Prepare( )
	{
	}

	virtual void Step( )
	{
	}

	virtual mV GetNextOutput( ) const
	{
		return 0._mV;
	}

	bool IsPointInShape( MicroMeterPoint const & point ) const
	{
		MicroMeterPoint const corner1 = m_center + MicroMeterPoint( + m_extension, + m_extension );
		MicroMeterPoint const corner2 = m_center + MicroMeterPoint( + m_extension, - m_extension );
		MicroMeterPoint const corner3 = m_center + MicroMeterPoint( - m_extension, + m_extension );

		return IsPointInRect< MicroMeterPoint >( point, corner1, corner2, corner3 );
	}

	virtual void Draw( GraphicsInterface &,	PixelCoordsFp const & ) const {};

	virtual void Move( MicroMeterPoint const & delta )
	{
		m_center += delta;
	}

protected:
	mV m_potential;

private:

	vector<Pipeline *> m_incomming;
	vector<Pipeline *> m_outgoing;

	MicroMeterPoint m_center;
	MicroMeter      m_extension;
};