// Pipeline.h
//
// NNetModel

#pragma once

#include <vector>
#include "MoreTypes.h"
#include "NNetParameters.h"
#include "Shape.h"
#include "Segment.h"

using std::vector;

class GraphicsInterface;
class PixelCoordsFp;
class BaseKnot;

class Pipeline : public Shape
{
public:
	Pipeline( );

	static bool TypeFits( tShapeType const type )
	{
		return type == tShapeType::pipeline;
	}

	void SetStartKnot( NNetModel const &, ShapeId const );
	void SetEndKnot  ( NNetModel const &, ShapeId const );

	virtual void FixShapeIds( ShapeId const idLimit )
	{
		Shape::FixShapeIds( idLimit );
		::FixShapeId( m_idKnotStart, idLimit );
		::FixShapeId( m_idKnotEnd,   idLimit );
	}

	void Recalc( NNetModel const & );

	ShapeId    GetStartKnot   ( ) const { return m_idKnotStart;  }
	ShapeId    GetEndKnot     ( ) const { return m_idKnotEnd;    }
	size_t     GetNrOfSegments( ) const { return m_potential.size(); }
	MicroMeter GetWidth       ( ) const { return m_width; }

	MicroMeterPoint GetStartPoint( NNetModel const & ) const; 
	MicroMeterPoint GetEndPoint  ( NNetModel const & ) const; 
	MicroMeter      GetLength    ( NNetModel const & ) const;
	MicroMeterPoint GetVector    ( NNetModel const & ) const; 

	virtual void Step          ( NNetModel const & );
	virtual void Prepare       ( NNetModel const & );
	virtual mV   GetNextOutput ( NNetModel const & ) const;
	virtual void DrawExterior  ( NNetModel const &, PixelCoordsFp  & ) const;
	virtual void DrawInterior  ( NNetModel const &, PixelCoordsFp  & ) const;
	virtual void MoveTo        ( NNetModel       &, MicroMeterPoint const & ) {}
	virtual bool IsPointInShape( NNetModel const &, MicroMeterPoint const & ) const;

private:
	ShapeId    m_idKnotStart;
	ShapeId    m_idKnotEnd;
	MicroMeter m_width;
	float      m_fDampingPerSegment;
	vector<mV> m_potential;
};

Pipeline const * Cast2Pipeline( Shape const * );
Pipeline       * Cast2Pipeline( Shape       * );
