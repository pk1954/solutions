// BaseKnot.h
//
// NNetModel

#pragma once

#include "PixelTypes.h"
#include "CircleType.h"
#include "MoreTypes.h"
#include "PipeList.h"
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

	static bool const TypeFits(NobType const type) { return type.IsBaseKnotType(); }

	MicroMeterCircle const GetCircle   () const { return m_circle; }
	MicroMeter       const GetExtension() const { return m_circle.GetRadius(); }
	mV               const GetVoltage  () const { return m_mVinputBuffer; }

	bool IsOrphanedKnot() const { return IsKnot() && IsOrphan(); }

	bool const Includes( MicroMeterPnt const & ) const;
	bool const IsPrecursorOf( Pipe const & ) const;
	bool const IsSuccessorOf( Pipe const & ) const ;

	bool const HasIncoming() const { return m_inPipes.IsNotEmpty (); }
	bool const HasOutgoing() const { return m_outPipes.IsNotEmpty(); }
	bool const IsOrphan()    const { return !(HasIncoming() || HasOutgoing()); }

	virtual void Reconnect() 
	{ 
		m_inPipes .Apply2All([&](Pipe & pipe) { pipe.SetEndKnot  (this); } );
		m_outPipes.Apply2All([&](Pipe & pipe) { pipe.SetStartKnot(this); } );
	}
	
	void AddIncoming(BaseKnot const & src) 
	{ 
		src.Apply2AllInPipes ([&](Pipe & pipe) { AddIncoming(& pipe); } );
	}

	void AddOutgoing(BaseKnot const & src) 
	{ 
		src.Apply2AllOutPipes([&](Pipe & pipe) { AddOutgoing(& pipe); } );
	}

	void SetConnections(BaseKnot const &); 
	void ClearConnections();

	size_t GetNrOfIncomingConnections() const { return m_inPipes .Size(); }
	size_t GetNrOfOutgoingConnections() const { return m_outPipes.Size(); }
	size_t GetNrOfConnections()         const { return m_inPipes .Size() + m_outPipes.Size(); }

	Pipe & GetFirstOutgoing() { return m_inPipes .GetFirst(); }
	Pipe & GetFirstIncoming() { return m_outPipes.GetFirst(); }

	Pipe const & GetFirstOutgoing() const { return m_inPipes .GetFirst(); }
	Pipe const & GetFirstIncoming() const { return m_outPipes.GetFirst(); }

	void AddIncoming(Pipe * const p) { m_inPipes .Add(p); }
	void AddOutgoing(Pipe * const p) { m_outPipes.Add(p); }

	void RemoveIncoming(Pipe * const p) { m_inPipes .Remove(p); } 
	void RemoveOutgoing(Pipe * const p) { m_outPipes.Remove(p); }

	void ReplaceIncoming(Pipe * const pDel, Pipe * const pAdd) { m_inPipes .Replace(pDel, pAdd); }
	void ReplaceOutgoing(Pipe * const pDel, Pipe * const pAdd) { m_outPipes.Replace(pDel, pAdd); }

	void SetIncoming(PipeList const & l) { m_inPipes  = l; }
	void SetOutgoing(PipeList const & l) { m_outPipes = l; }

	void SetIncoming(BaseKnot const & b) { SetIncoming(b.m_inPipes ); }
	void SetOutgoing(BaseKnot const & b) { SetOutgoing(b.m_outPipes); }

	void Apply2AllInPipes        (PipeFunc const &f) const { m_inPipes .Apply2All(f); }
	void Apply2AllOutPipes       (PipeFunc const &f) const { m_outPipes.Apply2All(f); }
	void Apply2AllConnectedPipes (PipeFunc const &f) const 
	{ 
		Apply2AllInPipes(f); 
		Apply2AllOutPipes(f); 
	}

	bool Apply2AllInPipesB       (PipeCrit const &c) const { return m_inPipes .Apply2AllB(c); }
	bool Apply2AllOutPipesB      (PipeCrit const &c) const { return m_outPipes.Apply2AllB(c); }
	bool Apply2AllConnectedPipesB(PipeCrit const &c) const 
	{ 
		return Apply2AllInPipesB(c) || Apply2AllOutPipesB(c); 
	}

	MicroMeterRect const GetRect4Text() const;

	PipeList const & GetIncoming() const { return m_inPipes; }
	PipeList const & GetOutgoing() const { return m_outPipes; }

private:

	void drawCircle(DrawContext const &, D2D1::ColorF const, MicroMeterCircle const) const;
	void drawCircle(DrawContext const &, D2D1::ColorF const, MicroMeter       const) const;

	PipeList         m_inPipes;
	PipeList         m_outPipes;
	MicroMeterCircle m_circle;
};

BaseKnot const * Cast2BaseKnot(Nob const *);
BaseKnot       * Cast2BaseKnot(Nob       *);
