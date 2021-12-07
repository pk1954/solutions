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

	~Pipe() final = default;

	virtual bool operator==(Nob const &) const;

	void AppendMenuItems(AddMenuFunc const &) const final;

	void Dump() const override;

	static bool TypeFits(NobType const type) { return type.IsPipeType(); }

	void SetStartKnot(BaseKnot * const);
	void SetEndKnot  (BaseKnot * const);

	void Emphasize(bool const) final;
	void Emphasize(bool const, bool const);

	BaseKnot * GetStartKnotPtr() { return m_pKnotStart; }
	BaseKnot * GetEndKnotPtr  () { return m_pKnotEnd;   }

	BaseKnot const * GetStartKnotPtr() const { return m_pKnotStart; }
	BaseKnot const * GetEndKnotPtr  () const { return m_pKnotEnd;   }
	size_t           GetNrOfSegments() const { return m_potential.size(); }

	NobId         GetStartKnotId() const;
	NobId         GetEndKnotId  () const;
	MicroMeterPnt GetStartPoint () const; 
	MicroMeterPnt GetEndPoint   () const; 
	MicroMeter    GetLength     () const;
	MicroMeterPnt GetVector     () const; 

	void          RotateNob(MicroMeterPnt const &, Radian const) final { /* Pipe dir defined by endpoints */ }
	void          SetDir   (Radian const)                        final { /* Pipe dir defined by endpoints */ };

	Radian        GetDir       () const final { return Vector2Radian(GetVector()); };
	NobIoMode     GetIoMode    () const final { return NobIoMode::internal; }
	mV            GetNextOutput() const final { return m_potential[m_potIndex]; }

	MicroMeterPnt GetPos          ()                                      const final;
	bool          IsIncludedIn    (MicroMeterRect const &)                const final;
	bool          Includes        (MicroMeterPnt  const &)                const final;
	void          Check           ()                                      const final;
	void          DrawExterior    (DrawContext const &, tHighlight const) const final;
	void          DrawInterior    (DrawContext const &, tHighlight const) const final;
	void          Expand          (MicroMeterRect &)                      const final;
	void          MoveNob         (MicroMeterPnt const &)                       final;
	void          Link            (Nob const &, Nob2NobFunc const &)            final;
	void          Prepare         ()                                            final;
	bool          CompStep        ()                                            final;
	void          Recalc          ()                                            final;
	void          ClearDynamicData()                                            final;
	void          Select          (bool const)                                  final;

	mV GetVoltageAt(MicroMeterPnt const &) const;

	void DrawArrows(DrawContext const &, MicroMeter const) const;

	void DislocateEndPoint  (MicroMeter d =  PIPE_WIDTH) { dislocate(m_pKnotEnd,   d); }
	void DislocateStartPoint(MicroMeter d = -PIPE_WIDTH) { dislocate(m_pKnotStart, d); }

	inline static wstring const SEPARATOR     { L"->" };
	inline static wchar_t const OPEN_BRACKET  { L'(' };
	inline static wchar_t const CLOSE_BRACKET { L')' };

	friend wostream & operator<< (wostream &, Pipe const &);

private:
	
	BaseKnot * m_pKnotStart { nullptr };
	BaseKnot * m_pKnotEnd   { nullptr };
	size_t     m_potIndex   { 0 };
	vector<mV> m_potential  { };

	void init(const Pipe &);

	void dislocate(BaseKnot * const, MicroMeter const);
};

Pipe const * Cast2Pipe(Nob const *);
Pipe       * Cast2Pipe(Nob       *);
