// Pipe.h
//
// NNetModel

#pragma once

#include <vector>
#include "NNetParameters.h"
#include "tHighlightType.h"
#include "Nob.h"

import PointType;
import MoreTypes;
import PixelTypes;

using std::vector;

class DrawContext;
class BaseKnot;

class Pipe : public Nob
{
public:
	using SegNr = NamedType<size_t, struct segNr_Parameter>;

	Pipe();
	Pipe(BaseKnot * const, BaseKnot * const);
	Pipe(Pipe const &);   // copy constructor

	~Pipe() final = default;

	bool operator==(Nob const &) const;

	void AppendMenuItems(AddMenuFunc const &) const final;

	void Dump() const override;

	static bool TypeFits(NobType const type) { return type.IsPipeType(); }

	void SetStartKnot(BaseKnot * const);
	void SetEndKnot  (BaseKnot * const);

	void Emphasize(bool const) final;
	void Emphasize(bool const, bool const);

	BaseKnot * GetStartKnotPtr() { return m_pKnotStart; }
	BaseKnot * GetEndKnotPtr  () { return m_pKnotEnd;   }

	bool IsConnectedTo(NobId const idBaseKnot) const
	{
		return (GetStartKnotId() == idBaseKnot) || (GetEndKnotId() == idBaseKnot);
	}

	BaseKnot const * GetStartKnotPtr() const { return m_pKnotStart; }
	BaseKnot const * GetEndKnotPtr  () const { return m_pKnotEnd;   }
	size_t           GetNrOfSegments() const { return m_potential.size(); }

	void SetNrOfSegments(size_t const n) { m_potential.resize(n); }

	NobId         GetStartKnotId() const;
	NobId         GetEndKnotId  () const;
	MicroMeterPnt GetStartPoint () const; 
	MicroMeterPnt GetEndPoint   () const; 
	MicroMeter    GetLength     () const;
	MicroMeterPnt GetVector     () const; 
	MicroMeterPnt GetVector     (float const) const; 
	MicroMeter    GetSegLength  () const { return GetLength() / Cast2Float(GetNrOfSegments()); };

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

	SegNr GetSegNr  (float const f)     const { return SegNr(Cast2Int(round(f * Cast2Float(GetNrOfSegments()-1)))); }
	mV    GetVoltage(SegNr const segNr) const { return m_potential[segNr2index(segNr)]; }
	mV    GetVoltage(float const f)     const { return GetVoltage(GetSegNr(f)); }

	mV    GetVoltageAt(MicroMeterPnt const &) const;

	void  SetVoltage(SegNr const segNr, mV const v) { m_potential[segNr2index(segNr)] = v; }

	void DrawArrows(DrawContext const &, MicroMeter const) const;

	void DislocateEndPoint();
	void DislocateStartPoint();

	friend wostream & operator<< (wostream &, Pipe const &);

	MicroMeterPnt GetSegmentVector() const
	{
		MicroMeterPnt const umVector  { GetEndPoint() - GetStartPoint() };
		MicroMeterPnt const umpSegVec { umVector / Cast2Float(GetNrOfSegments()) };
		return umpSegVec;
	}

	MicroMeterPnt GetSegmentCenter(SegNr const segNr) const
	{
		return getSegmentPos(segNr, 0.5f);
	}

	MicroMeterPnt GetSegmentStart(SegNr const segNr) const
	{
		return getSegmentPos(segNr, 0.0f);
	}

	MicroMeterPnt GetSegmentEnd(SegNr const segNr) const
	{
		return getSegmentPos(segNr, 1.0f);
	}

	void Apply2AllSegments(auto const & func) const
	{
		for (auto segNr = SegNr(0); segNr.GetValue() < GetNrOfSegments(); ++segNr )
			func(segNr);
	}

private:
	BaseKnot * m_pKnotStart { nullptr };
	BaseKnot * m_pKnotEnd   { nullptr };
	size_t     m_potIndex   { 0 };   // index in m_potential if SegNr 0
	vector<mV> m_potential  { };

	MicroMeterPnt dislocation() const;
	size_t        segNr2index(SegNr const) const;
	void          recalc();

	MicroMeterPnt getSegmentPos(SegNr const segNr, float const fPos) const
	{
		MicroMeterPnt const umVector  { GetEndPoint() - GetStartPoint() };
		MicroMeterPnt const umpSegVec { umVector / Cast2Float(GetNrOfSegments()) };
		float         const fPosition { (static_cast<float>(segNr.GetValue()) + fPos) };
		return GetStartPoint() + umpSegVec * fPosition;
	}
};

Pipe const * Cast2Pipe(Nob const *);
Pipe       * Cast2Pipe(Nob       *);
