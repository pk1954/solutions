// BaseKnot.h
//
// NNetModel

#pragma once

#include "PixelTypes.h"
#include "CircleType.h"
#include "MoreTypes.h"
#include "Connections.h"
#include "Shape.h"

using std::wstring;

class DrawContext;
class NNetModel;
class Pipe;

template <typename T> 
concept BaseKnot_t = is_base_of<BaseKnot, remove_pointer_t<T>>::value;

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
		m_circle( center, extension )
	{ }

	virtual ~BaseKnot() {}

	virtual bool operator==( Shape const & ) const override;

	virtual BaseKnot & operator*=(float const f)
	{
		m_circle *= f;
		return * this;
	}

	virtual BaseKnot & operator+=(BaseKnot const &rhs)
	{
		m_circle += rhs.GetPosition();
		return * this;
	}

	virtual BaseKnot & operator-=(BaseKnot const &rhs)
	{
		m_circle += rhs.GetPosition();
		return * this;
	}

	virtual void Dump         ( ) const;
	virtual void CheckShape   ( ) const;
	virtual void Prepare      ( );
 	virtual mV   GetNextOutput( ) const = 0;
	virtual void MoveShape    ( MicroMeterPoint const & );
	virtual void SetPosition  ( MicroMeterPoint const & );
	virtual bool IsInRect     ( MicroMeterRect  const & umRect ) const { return umRect.Includes( GetPosition() ); }

	static bool TypeFits( ShapeType const type ) { return type.IsBaseKnotType( ); }

	MicroMeterCircle const GetCircle   ( ) const { return m_circle; }
	MicroMeterPoint  const GetPosition ( ) const { return m_circle.GetPosition(); }
	MicroMeter       const GetExtension( ) const { return m_circle.GetRadius(); }
	mV               const GetVoltage  ( ) const { return m_mVinputBuffer; }

	bool IsOrphanedKnot( ) const { return IsKnot() && m_connections.IsOrphan(); }

	bool IsPointInShape( MicroMeterPoint const & ) const;
	bool IsPrecursorOf( Pipe const & ) const;
	bool IsSuccessorOf( Pipe const & ) const ;

	void Reconnect() { m_connections.Reconnect( this );	}
	
	void AddConnections( BaseKnot * const pSrc ) 
	{ 
		m_connections.Add( pSrc->m_connections );
		Reconnect( );
	}

	void SetConnections( Connections * const pSrc ) 
	{ 
		m_connections = * pSrc;
		Reconnect( );
	}

	void ClearConnections( )
	{
		m_connections.ClearIncoming();
		m_connections.ClearOutgoing();
	}

	Connections m_connections;

protected:

	void drawCircle( DrawContext const &, D2D1::ColorF const, MicroMeterCircle const ) const;
	void drawCircle( DrawContext const &, D2D1::ColorF const, MicroMeter       const ) const;

	MicroMeterRect const GetRect4Text( ) const;

private:

	MicroMeterCircle m_circle;
};

BaseKnot const * Cast2BaseKnot( Shape const * );
BaseKnot       * Cast2BaseKnot( Shape       * );
