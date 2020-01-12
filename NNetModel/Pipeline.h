// Pipeline.h
//
// NNetModel

#pragma once

#include <vector>
#include "MoreTypes.h"
#include "PixelTypes.h"
#include "NNetParameters.h"
#include "tHighlightType.h"
#include "Shape.h"
#include "Segment.h"

using std::vector;

class GraphicsInterface;
class PixelCoordsFp;
class BaseKnot;

class Pipeline : public Shape
{
public:
	Pipeline( NNetModel *, MicroMeterPoint const = NP_NULL );
	virtual ~Pipeline() {}

	static bool TypeFits( tShapeType const type )
	{
		return type == tShapeType::pipeline;
	}

	void SetStartKnot( ShapeId const );
	void SetEndKnot  ( ShapeId const );

	void Recalc( );

	ShapeId    GetStartKnot   ( ) const { return m_idKnotStart;  }
	ShapeId    GetEndKnot     ( ) const { return m_idKnotEnd;    }
	size_t     GetNrOfSegments( ) const { return m_potential.size(); }
	MicroMeter GetWidth       ( ) const { return m_width; }

	MicroMeterPoint GetStartPoint( ) const; 
	MicroMeterPoint GetEndPoint  ( ) const; 
	MicroMeter      GetLength    ( ) const;
	MicroMeterPoint GetVector    ( ) const; 

	virtual void Step          ( );
	virtual void Prepare       ( );
	virtual mV   GetNextOutput ( ) const;
	virtual void DrawExterior  ( PixelCoordsFp  &, tHighlightType const ) const;
	virtual void DrawInterior  ( PixelCoordsFp  & ) const;
	virtual bool IsPointInShape( MicroMeterPoint const & ) const;

	void DislocateEndPoint  ( ) { dislocate( GetEndKnot(),    PIPELINE_WIDTH ); }
	void DislocateStartPoint( )	{ dislocate( GetStartKnot(), -PIPELINE_WIDTH );	}

	static void       SetArrowSize( MicroMeter const size ) { m_arrowSize = size; }
	static MicroMeter GetArrowSize( ) { return m_arrowSize; }

	static MicroMeter const STD_ARROW_SIZE;

private:
	
	static MicroMeter m_arrowSize;

	typedef vector<mV> tPotentialVector;

	ShapeId                    m_idKnotStart;
	ShapeId                    m_idKnotEnd;
	MicroMeter                 m_width;
	tPotentialVector           m_potential;
	tPotentialVector::iterator m_potIter;

	void dislocate( ShapeId const, MicroMeter const );
	void drawSegment( fPixelPoint &, fPixelPoint const, fPIXEL const, mV const ) const;
};

Pipeline const * Cast2Pipeline( Shape const * );
Pipeline       * Cast2Pipeline( Shape       * );
