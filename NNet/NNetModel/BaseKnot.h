// BaseKnot.h
//
// NNetModel

#pragma once

#include "PixelTypes.h"
#include "CircleType.h"
#include "MoreTypes.h"
#include "Connections.h"
#include "Nob.h"

class DrawContext;
class NNetModel;

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
		MicroMeter    const   extension
	)
	  : Nob( type ),
		m_circle( center, extension )
	{ }

	virtual ~BaseKnot() {}

	virtual bool operator==( Nob const & ) const override;

	virtual BaseKnot & operator*=(float const);
	virtual BaseKnot & operator+=(BaseKnot const &);
	virtual BaseKnot & operator-=(BaseKnot const &);

	virtual MicroMeterPnt const GetPos() const { return m_circle.GetPos(); }

	virtual void       Dump         () const;
	virtual void       Check        () const;
	virtual void       Prepare      ();
 	virtual mV   const GetNextOutput() const = 0;
	virtual void       SetPos       (MicroMeterPnt const &);
	virtual bool const IsIncludedIn (MicroMeterRect  const &) const; 
	virtual void       Expand       (MicroMeterRect        &) const;
	virtual void       RotateNob    (MicroMeterPnt const &, Radian const);
	virtual void       Link         (Nob const &, Nob2NobFunc const &);
	virtual void       MoveNob      (MicroMeterPnt const &);

	unique_ptr<Connections>CloneConnection() const { return m_connections.Clone(); }

	static bool const TypeFits( NobType const type ) { return type.IsBaseKnotType(); }

	MicroMeterCircle const GetCircle   () const { return m_circle; }
	MicroMeter       const GetExtension() const { return m_circle.GetRadius(); }
	mV               const GetVoltage  () const { return m_mVinputBuffer; }

	bool IsOrphanedKnot() const { return IsKnot() && m_connections.IsOrphan(); }

	bool const Includes( MicroMeterPnt const & ) const;
	bool const IsPrecursorOf( Pipe const & ) const;
	bool const IsSuccessorOf( Pipe const & ) const ;

	bool const HasIncoming() const { return m_connections.HasIncoming(); }
	bool const HasOutgoing() const { return m_connections.HasOutgoing(); }
	bool const IsOrphan()    const { return  m_connections.IsOrphan(); }

	virtual void Reconnect() { m_connections.Reconnect( this );	}
	
	void AddConnections(BaseKnot    const &); 
	void SetConnections(BaseKnot    const &); 
	void SetConnections(Connections const &); 
	void ClearConnections();

	size_t GetNrOfIncomingConnections() const { return m_connections.GetNrOfIncomingConnections(); }
	size_t GetNrOfOutgoingConnections() const { return m_connections.GetNrOfOutgoingConnections(); }
	size_t GetNrOfConnections()         const { return m_connections.GetNrOfConnections(); }

	Pipe & GetFirstOutgoing() { return m_connections.GetFirstOutgoing(); }
	Pipe & GetFirstIncoming() { return m_connections.GetFirstIncoming(); }

	Pipe const & GetFirstOutgoing() const { return m_connections.GetFirstOutgoing(); }
	Pipe const & GetFirstIncoming() const { return m_connections.GetFirstIncoming(); }

	void AddIncoming(Pipe * const p) { m_connections.AddIncoming(p); }
	void AddOutgoing(Pipe * const p) { m_connections.AddOutgoing(p); }

	void RemoveIncoming(Pipe * const p) { m_connections.RemoveIncoming(p); } 
	void RemoveOutgoing(Pipe * const p) { m_connections.RemoveOutgoing(p); }

	void ReplaceIncoming(Pipe * const pDel, Pipe * const pAdd) { m_connections.ReplaceIncoming(pDel, pAdd); }
	void ReplaceOutgoing(Pipe * const pDel, Pipe * const pAdd) { m_connections.ReplaceOutgoing(pDel, pAdd); }

	void SetIncoming(BaseKnot const & b) { m_connections.SetIncoming(b.m_connections); }
	void SetOutgoing(BaseKnot const & b) { m_connections.SetOutgoing(b.m_connections); }

	void Apply2AllInPipes        (PipeFunc const &f) const { m_connections.Apply2AllInPipes       (f); }
	void Apply2AllOutPipes       (PipeFunc const &f) const { m_connections.Apply2AllOutPipes      (f); }
	void Apply2AllConnectedPipes (PipeFunc const &f) const { m_connections.Apply2AllConnectedPipes(f); }

	bool Apply2AllInPipesB       (PipeCrit const &c) const { return m_connections.Apply2AllInPipesB       (c); }
	bool Apply2AllOutPipesB      (PipeCrit const &c) const { return m_connections.Apply2AllOutPipesB      (c); }
	bool Apply2AllConnectedPipesB(PipeCrit const &c) const { return m_connections.Apply2AllConnectedPipesB(c); }

protected:

	void drawCircle(DrawContext const &, D2D1::ColorF const, MicroMeterCircle const) const;
	void drawCircle(DrawContext const &, D2D1::ColorF const, MicroMeter       const) const;

	MicroMeterRect const GetRect4Text() const;

private:

	Connections      m_connections;
	MicroMeterCircle m_circle;
};

BaseKnot const * Cast2BaseKnot(Nob const *);
BaseKnot       * Cast2BaseKnot(Nob       *);
