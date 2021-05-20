// BaseKnot.h
//
// NNetModel

#pragma once

#include "PixelTypes.h"
#include "CircleType.h"
#include "MoreTypes.h"
#include "Connections.h"
#include "Nob.h"

using std::wstring;

class DrawContext;
class NNetModel;
class Pipe;

template <typename T> 
concept BaseKnot_t = is_base_of<BaseKnot, remove_pointer_t<T>>::value;

struct IDWriteTextFormat;

class BaseKnot : public Nob
{
public:

	BaseKnot
	( 
		MicroMeterPnt const & center,
		NobType       const   type,
		MicroMeter      const   extension
	)
	  : Nob( type ),
		m_circle( center, extension )
	{ }

	virtual ~BaseKnot() {}

	virtual bool operator==( Nob const & ) const override;

	virtual BaseKnot & operator*=(float const f)
	{
		m_circle *= f;
		return * this;
	}

	virtual BaseKnot & operator+=(BaseKnot const &rhs)
	{
		m_circle += rhs.GetPos();
		return * this;
	}

	virtual BaseKnot & operator-=(BaseKnot const &rhs)
	{
		m_circle += rhs.GetPos();
		return * this;
	}

	virtual MicroMeterPnt const GetPos() const 
	{ 
		return m_circle.GetPos(); 
	}

	virtual void       Dump         () const;
	virtual void       Check        () const;
	virtual void       Prepare      ();
 	virtual mV   const GetNextOutput() const = 0;
	virtual void       SetPos       (MicroMeterPnt const &);
	virtual bool const IsIncludedIn (MicroMeterRect  const &) const; 
	virtual void       Expand       (MicroMeterRect        &) const;
	virtual void       MoveNob      (MicroMeterPnt const &);
	virtual void       RotateNob    (MicroMeterPnt const &, Radian const);
	virtual void       Link         (Nob const &, Nob2NobFunc const &);

	static bool const TypeFits( NobType const type ) { return type.IsBaseKnotType(); }

	MicroMeterCircle const GetCircle   () const { return m_circle; }
	MicroMeter       const GetExtension() const { return m_circle.GetRadius(); }
	mV               const GetVoltage  () const { return m_mVinputBuffer; }

	bool IsOrphanedKnot() const { return IsKnot() && m_connections.IsOrphan(); }

	bool const Includes( MicroMeterPnt const & ) const;
	bool const IsPrecursorOf( Pipe const & ) const;
	bool const IsSuccessorOf( Pipe const & ) const ;

	void Reconnect() { m_connections.Reconnect( this );	}
	
	void AddConnections( BaseKnot * const pSrc ) 
	{ 
		m_connections.Add( pSrc->m_connections );
		Reconnect();
	}

	void SetConnections( Connections * const pSrc ) 
	{ 
		m_connections = * pSrc;
		Reconnect();
	}

	void ClearConnections()
	{
		m_connections.ClearIncoming();
		m_connections.ClearOutgoing();
	}

	Connections m_connections;

protected:

	void drawCircle(DrawContext const &, D2D1::ColorF const, MicroMeterCircle const) const;
	void drawCircle(DrawContext const &, D2D1::ColorF const, MicroMeter       const) const;

	MicroMeterRect const GetRect4Text() const;

private:

	MicroMeterCircle m_circle;
};

BaseKnot const * Cast2BaseKnot(Nob const *);
BaseKnot       * Cast2BaseKnot(Nob       *);
