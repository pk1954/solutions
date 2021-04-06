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
	Pipe(BaseKnot * const, BaseKnot * const);

	Pipe(Pipe const &);   // copy constructor

	Pipe & operator=(Pipe const &); // copy assignment operator

	virtual ~Pipe();

	virtual bool operator==(Shape const &) const override;

	virtual void Dump() const;

	static bool      const TypeFits(ShapeType const type) { return type.IsPipeType(); }
	static ShapeType const GetShapeType() { return ShapeType::Value::pipe; }

	void SetStartKnot(BaseKnot * const);
	void SetEndKnot  (BaseKnot * const);

	BaseKnot * const GetStartKnotPtr() const { return m_pKnotStart; }
	BaseKnot * const GetEndKnotPtr  () const { return m_pKnotEnd;   }

	ShapeId         GetStartKnotId() const;
	ShapeId         GetEndKnotId  () const;
	MicroMeterPoint GetStartPoint () const; 
	MicroMeterPoint GetEndPoint   () const; 
	MicroMeter      GetLength     () const;
	MicroMeterPoint GetVector     () const; 

	size_t GetNrOfSegments() const { return m_potential.size(); }

	virtual MicroMeterPoint const GetPosition ()                                      const;
	virtual bool            const Includes    (MicroMeterRect  const &)               const;
	virtual bool            const Includes    (MicroMeterPoint const &)               const;
	virtual void                  CheckShape  ()                                      const;
	virtual void                  DrawArrows  (DrawContext const &, MicroMeter const) const;
	virtual void                  DrawExterior(DrawContext const &, tHighlight const) const;
	virtual void                  DrawInterior(DrawContext const &, tHighlight const) const;
	virtual void                  Expand      (MicroMeterRect &)                      const;
	virtual void                  Select      (bool const, bool const);
	virtual void                  MoveShape   (MicroMeterPoint const &);
	virtual void                  Prepare();
	virtual bool            const CompStep();
	virtual void                  Recalc();
	virtual void                  Clear();

	mV const GetNextOutput() const { return m_potential[ m_potIndex ]; }
	mV const GetVoltage( MicroMeterPoint const & ) const;

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

Pipe const * Cast2Pipe( Shape const * );
Pipe       * Cast2Pipe( Shape       * );
