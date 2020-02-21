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
#include "BaseKnot.h"
#include "Segment.h"

using std::vector;

class GraphicsInterface;
class PixelCoordsFp;

class Pipeline : public Shape
{
public:
	Pipeline( MicroMeterPoint const = NP_NULL );
	virtual ~Pipeline() {}

	static bool TypeFits( ShapeType const type )
	{
		return type.IsPipelineType( );
	}

	void SetStartKnot( BaseKnot * const );
	void SetEndKnot  ( BaseKnot * const );

	BaseKnot * const GetStartKnotPtr( ) const { return m_pKnotStart; }
	BaseKnot * const GetEndKnotPtr  ( ) const { return m_pKnotEnd;   }

	ShapeId    GetStartKnotId ( ) const { return m_pKnotStart->GetId(); }
	ShapeId    GetEndKnotId   ( ) const { return m_pKnotEnd  ->GetId(); }

	size_t     GetNrOfSegments( ) const { return m_potential.size(); }
	MicroMeter GetWidth       ( ) const { return m_width; }

	MicroMeterPoint GetStartPoint( ) const; 
	MicroMeterPoint GetEndPoint  ( ) const; 
	MicroMeter      GetLength    ( ) const;
	MicroMeterPoint GetVector    ( ) const; 

	virtual bool IsInRect( MicroMeterRect const & umRect ) const 
	{ 
		return m_pKnotStart->IsInRect( umRect ) || m_pKnotEnd->IsInRect( umRect );
	}

	virtual void Prepare( )
	{
		m_mVinputBuffer = m_pKnotStart->GetNextOutput( );
	}

	virtual void Step( )
	{
		EnterCritSect();
		* m_potIter = m_mVinputBuffer;
		if ( m_potIter == m_potential.begin() )
			m_potIter = m_potential.end( );
		-- m_potIter;
		LeaveCritSect();
	}

	mV GetNextOutput( ) const {	return * m_potIter; }

	virtual void DrawExterior  ( PixelCoordsFp  &, tHighlightType const ) const;
	virtual void DrawInterior  ( PixelCoordsFp  & );
	virtual bool IsPointInShape( MicroMeterPoint const & ) const;
	virtual void Recalc( );
	virtual void Clear( );

	void DislocateEndPoint  ( ) { dislocate( GetEndKnotPtr(),    PIPELINE_WIDTH ); }
	void DislocateStartPoint( )	{ dislocate( GetStartKnotPtr(), -PIPELINE_WIDTH );	}

	static void       SetArrowSize( MicroMeter const size ) { m_arrowSize = size; }
	static MicroMeter GetArrowSize( ) { return m_arrowSize; }

	static MicroMeter const STD_ARROW_SIZE;

private:
	
	static MicroMeter m_arrowSize;

	typedef vector<mV> tPotentialVector;

	BaseKnot                 * m_pKnotStart;
	BaseKnot                 * m_pKnotEnd;
	MicroMeter                 m_width;
	tPotentialVector           m_potential;
	tPotentialVector::iterator m_potIter;

	void dislocate( BaseKnot * const, MicroMeter const );
	void drawSegment( fPixelPoint &, fPixelPoint const, fPIXEL const, mV const ) const;
};

Pipeline const * Cast2Pipeline( Shape const * );
Pipeline       * Cast2Pipeline( Shape       * );
