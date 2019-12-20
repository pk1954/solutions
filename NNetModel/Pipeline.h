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
	Pipeline( NNetModel * );
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
	virtual void DrawExterior  ( PixelCoordsFp  & ) const;
	virtual void DrawInterior  ( PixelCoordsFp  & ) const;
	virtual bool IsPointInShape( MicroMeterPoint const & ) const;

private:
	ShapeId    m_idKnotStart;
	ShapeId    m_idKnotEnd;
	MicroMeter m_width;
	float      m_fDampingFactor;
	vector<mV> m_potential;
};

Pipeline const * Cast2Pipeline( Shape const * );
Pipeline       * Cast2Pipeline( Shape       * );
