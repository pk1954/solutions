// Pipe.h
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

class DrawContext;
class BaseKnot;

class Pipe : public Shape
{
public:
	Pipe( BaseKnot * const, BaseKnot * const );
	virtual ~Pipe();

	virtual bool IsEqual( Pipe const & ) const;

	static unsigned long GetCounter( ) { return m_counter; }

	static bool TypeFits( ShapeType const type ) { return type.IsPipeType( ); }

	void SetStartKnot( BaseKnot * const );
	void SetEndKnot  ( BaseKnot * const );

	BaseKnot * const GetStartKnotPtr( ) const { return m_pKnotStart; }
	BaseKnot * const GetEndKnotPtr  ( ) const { return m_pKnotEnd;   }

	ShapeId         GetStartKnotId( ) const;
	ShapeId         GetEndKnotId  ( ) const;
	MicroMeterPoint GetStartPoint ( ) const; 
	MicroMeterPoint GetEndPoint   ( ) const; 
	MicroMeter      GetLength     ( ) const;
	MicroMeterPoint GetVector     ( ) const; 

	size_t     GetNrOfSegments( ) const { return m_potential.size(); }
	MicroMeter GetWidth       ( ) const { return m_width; }

	virtual bool IsInRect( MicroMeterRect const & ) const;

	virtual void Prepare();
	virtual void Select( tBoolOp const );
	virtual void MoveShape( MicroMeterPoint const & );
	virtual bool CompStep( )
	{
		* m_potIter = m_mVinputBuffer;
		if ( m_potIter == m_potential.begin() )
			m_potIter = m_potential.end() - 1;  // caution!
		else                                    // modification if m_potIter
		    --m_potIter;                        // must be atomic
		return false;
	}

	mV GetNextOutput( ) const 
	{ 
		return * m_potIter; 
	}

	mV GetVoltage( MicroMeterPoint const & ) const;

	virtual void DrawExterior  ( DrawContext const &, tHighlightType const  = tHighlightType::normal ) const;
	virtual void DrawInterior  ( DrawContext const & ) const;
	virtual bool IsPointInShape( MicroMeterPoint const & ) const;
	virtual void Recalc( );
	virtual void Clear( );

	void DislocateEndPoint  ( ) { dislocate( GetEndKnotPtr(),    PIPE_WIDTH ); }
	void DislocateStartPoint( )	{ dislocate( GetStartKnotPtr(), -PIPE_WIDTH ); }

	static void       SetArrowSize( MicroMeter const size ) { m_arrowSize = size; }
	static MicroMeter GetArrowSize( ) { return m_arrowSize; }

	inline static MicroMeter const STD_ARROW_SIZE { 30.0_MicroMeter };

private:
	
	inline static MicroMeter    m_arrowSize { STD_ARROW_SIZE };
	inline static unsigned long m_counter   { 0L };

	typedef vector<mV>                       tPotentialVector;
	typedef tPotentialVector::iterator       tPotIter;
	typedef tPotentialVector::const_iterator tPotConstIter;

	BaseKnot       * m_pKnotStart { nullptr };
	BaseKnot       * m_pKnotEnd   { nullptr };
	MicroMeter       m_width      { PIPE_WIDTH };
	tPotentialVector m_potential  { };
	tPotIter         m_potIter    { };

	void dislocate( BaseKnot * const, MicroMeter const );
};

Pipe const * Cast2Pipe( Shape const * );
Pipe       * Cast2Pipe( Shape       * );
