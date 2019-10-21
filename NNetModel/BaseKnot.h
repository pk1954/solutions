// BaseKnot.h
//
// NNetModel
//
// geometry aspects of all kinds of knots

#pragma once

#include <vector>
#include "MoreTypes.h"
#include "Shape.h"

using std::vector;

class GraphicsInterface;
class PixelCoordsFp;
class NNetModel;

class BaseKnot : public Shape
{
public:
	BaseKnot
	( 
		MicroMeterPoint const center,
		tShapeType      const type,
		MicroMeter      const extension
 	)
	  : Shape( type ),
		m_center( center ),
		m_extension( extension )
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

	void AddIncomming  ( ShapeId const );
	void AddOutgoing   ( ShapeId const );
	bool IsPointInShape( MicroMeterPoint const & ) const;

	void Apply2AllIncomingPipelines( std::function<void(ShapeId const &)> const & func ) const
	{
		for ( auto id : m_incoming )
		{
			func( id );
		}
	}

	virtual void MoveTo( MicroMeterPoint const & );

protected:

	vector<ShapeId> m_incoming;
	vector<ShapeId> m_outgoing;

private:

	MicroMeterPoint m_center;
	MicroMeter      m_extension;
};