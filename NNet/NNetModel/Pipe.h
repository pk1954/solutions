// Pipe.h
//
// NNetModel

#pragma once

#include <vector>
#include "MoreTypes.h"
#include "PixelTypes.h"
#include "NNetParameters.h"
#include "tHighlightType.h"
#include "Nob.h"
#include "Segment.h"

using std::vector;

class DrawContext;
class BaseKnot;

class Pipe : public Nob
{
public:
	Pipe(BaseKnot * const, BaseKnot * const);

	Pipe(Pipe const &);   // copy constructor

	Pipe & operator=(Pipe const &); // copy assignment operator

	virtual ~Pipe();

	virtual bool operator==(Nob const &) const override;

	virtual void Dump() const;

	static bool    const TypeFits(NobType const type) { return type.IsPipeType(); }
	static NobType const GetNobType() { return NobType::Value::pipe; }

	void SetStartKnot(BaseKnot * const);
	void SetEndKnot  (BaseKnot * const);

	BaseKnot * const GetStartKnotPtr() const { return m_pKnotStart; }
	BaseKnot * const GetEndKnotPtr  () const { return m_pKnotEnd;   }

	NobId         GetStartKnotId() const;
	NobId         GetEndKnotId  () const;
	MicroMeterPnt GetStartPoint () const; 
	MicroMeterPnt GetEndPoint   () const; 
	MicroMeter    GetLength     () const;
	MicroMeterPnt GetVector     () const; 

	size_t GetNrOfSegments() const { return m_potential.size(); }

	virtual void SetDir(Radian const) {};

	virtual Radian        const GetDir      () const { return Vector2Radian(GetVector()); };
	virtual NobIoMode     const GetIoMode   () const { return NobIoMode::internal; }
	virtual MicroMeterPnt const GetPos      ()                                      const;
	virtual bool          const IsIncludedIn(MicroMeterRect  const &)               const;
	virtual bool          const Includes    (MicroMeterPnt const &)                 const;
	virtual void                Check       ()                                      const;
	virtual void                DrawArrows  (DrawContext const &, MicroMeter const) const;
	virtual void                DrawExterior(DrawContext const &, tHighlight const) const;
	virtual void                DrawInterior(DrawContext const &, tHighlight const) const;
	virtual void                Expand      (MicroMeterRect &)                      const;
	virtual void                MoveNob     (MicroMeterPnt const &);
	virtual void                Link        (Nob const &, Nob2NobFunc const &);
	virtual void                RotateNob   (MicroMeterPnt const &, Radian const) {}
	virtual void                Prepare     ();
	virtual bool          const CompStep    ();
	virtual void                Recalc      ();
	virtual void                Clear       ();
	virtual void                Select      (bool const);

	mV const GetNextOutput() const { return m_potential[ m_potIndex ]; }
	mV const GetVoltage( MicroMeterPnt const & ) const;

	void DislocateEndPoint  () { dislocate( GetEndKnotPtr(),    PIPE_WIDTH ); }
	void DislocateStartPoint() { dislocate( GetStartKnotPtr(), -PIPE_WIDTH ); }

	inline static wstring const SEPARATOR     { L"->" };
	inline static wchar_t const OPEN_BRACKET  { L'(' };
	inline static wchar_t const CLOSE_BRACKET { L')' };

	friend wostream & operator<< ( wostream &, Pipe const & );

private:
	
	typedef vector<mV> tPotentialVector;

	BaseKnot       * m_pKnotStart { nullptr };
	BaseKnot       * m_pKnotEnd   { nullptr };
	size_t           m_potIndex   { 0 };
	tPotentialVector m_potential  { };

	void init( const Pipe & );

	void dislocate( BaseKnot * const, MicroMeter const );
};

Pipe const * Cast2Pipe( Nob const * );
Pipe       * Cast2Pipe( Nob       * );
