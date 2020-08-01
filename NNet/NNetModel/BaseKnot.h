// BaseKnot.h
//
// NNetModel

#pragma once

#include <vector>
#include "PixelTypes.h"
#include "MoreTypes.h"
#include "Connections.h"
#include "Shape.h"

using std::wstring;

class DrawContext;
class NNetModel;

struct IDWriteTextFormat;

class BaseKnot : public Shape
{
public:

	BaseKnot
	( 
		MicroMeterPoint const center,
		ShapeType       const type,
		MicroMeter      const extension
	)
	  : Shape( type ),
		m_center( center ),
		m_extension( extension )
	{ }

	virtual ~BaseKnot() {}

	virtual bool IsEqual( BaseKnot const & other ) const
	{
		if ( ! Shape::IsEqual( other ) )
			return false;
		if ( ! IsCloseToZero( m_center - other.m_center ) )
		{
			MicroMeterPoint diff = m_center - other.m_center;
			return false;
		}
		if ( ! IsCloseToZero( m_extension - other.m_extension ) )
			return false;
		return true;
	}

	virtual void Prepare      ( );
 	virtual mV   GetNextOutput( ) const = 0;
	virtual void MoveShape    ( MicroMeterPoint const & );
	virtual void SetPosition  ( MicroMeterPoint const & );
	virtual bool IsInRect     ( MicroMeterRect  const & umRect ) const { return umRect.Includes( m_center ); }

	static bool TypeFits( ShapeType const type ) { return type.IsBaseKnotType( ); }

	MicroMeterPoint GetPosition ( ) const { return m_center; }
	MicroMeter      GetExtension( ) const { return m_extension;	}
	mV              GetVoltage  ( ) const { return m_mVinputBuffer; }

	bool IsOrphanedKnot( ) const { return IsKnot() && m_connections.IsOrphan(); }

	bool IsPointInShape( MicroMeterPoint const & ) const;
	bool IsPrecursorOf( ShapeId const );
	bool IsSuccessorOf( ShapeId const );

	void RestoreConnections() { m_connections.Restore( this );	}
	
	void AddConnections( BaseKnot * const pSrc ) 
	{ 
		m_connections.Add( pSrc->m_connections );
		RestoreConnections( );
	}

	void SetConnections( Connections * const pSrc ) 
	{ 
		m_connections = * pSrc;
		RestoreConnections( );
	}

	void ClearConnections( )
	{
		m_connections.ClearIncoming();
		m_connections.ClearOutgoing();
	}

	Connections m_connections;

protected:

	void drawCircle( DrawContext const &, D2D1::ColorF const, MicroMeterPoint const, MicroMeter const ) const;
	void drawCircle( DrawContext const &, D2D1::ColorF const, MicroMeter const ) const;

	MicroMeterRect const GetRect4Text( ) const;

private:

	MicroMeterPoint m_center;
	MicroMeter      m_extension;
};

BaseKnot const * Cast2BaseKnot( Shape const * );
BaseKnot       * Cast2BaseKnot( Shape       * );
