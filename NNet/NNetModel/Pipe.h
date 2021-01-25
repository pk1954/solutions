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

	Pipe( Pipe const & );   // copy constructor

	Pipe & operator=( Pipe const & ); // copy assignment operator

	virtual ~Pipe();

	virtual bool operator==( Shape const & ) const override;

	virtual void Dump() const;

	static unsigned long GetCounter  ( ) { return m_counter; }
	static   void        ResetCounter( ) { m_counter = 0L; }

	virtual  void        IncCounter  ( ) { ++ m_counter; }
	virtual  void        DecCounter  ( ) { -- m_counter; }

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

	size_t GetNrOfSegments( ) const { return m_potential.size(); }

	virtual bool IsInRect( MicroMeterRect const & ) const;

	virtual void CheckShape( ) const;
	virtual void Prepare( );
	virtual void MoveShape( MicroMeterPoint const & );
	virtual bool CompStep( )
	{
		m_potential[ m_potIndex ] = m_mVinputBuffer;
		if ( m_potIndex == 0 )
			m_potIndex = m_potential.size() - 1;  // caution!
		else                                      // modification if m_potIndex
		    --m_potIndex;                         // must be atomic
		return false;
	}

	mV GetNextOutput( ) const { return m_potential[ m_potIndex ]; }
	mV GetVoltage( MicroMeterPoint const & ) const;

	virtual void DrawArrows    ( DrawContext const &, tHighlightType const, MicroMeter const  ) const;
	virtual void DrawExterior  ( DrawContext const &, tHighlightType const ) const;
	virtual void DrawInterior  ( DrawContext const &, tHighlightType const ) const;
	virtual bool IsPointInShape( MicroMeterPoint const & ) const;
	virtual void Recalc( );
	virtual void Clear( );

	void DislocateEndPoint  ( ) { dislocate( GetEndKnotPtr(),    PIPE_WIDTH ); }
	void DislocateStartPoint( )	{ dislocate( GetStartKnotPtr(), -PIPE_WIDTH ); }

	inline static wstring const SEPARATOR     { L"->" };
	inline static wchar_t const OPEN_BRACKET  { L'(' };
	inline static wchar_t const CLOSE_BRACKET { L')' };

	friend wostream & operator<< ( wostream &, Pipe const & );

private:
	
	inline static unsigned long m_counter { 0L };

	typedef vector<mV> tPotentialVector;

	BaseKnot       * m_pKnotStart { nullptr };
	BaseKnot       * m_pKnotEnd   { nullptr };
	size_t           m_potIndex   { 0 };
	tPotentialVector m_potential  { };

	void init( const Pipe & );

	void dislocate( BaseKnot * const, MicroMeter const );
};

Pipe const * Cast2Pipe( Shape const * );
Pipe       * Cast2Pipe( Shape       * );
