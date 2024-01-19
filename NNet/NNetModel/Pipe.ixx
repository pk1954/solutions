// Pipe.ixx
//
// NNetModel

module;

#include <cassert>
#include <utility>
#include <functional>
#include <algorithm>
#include <iostream>
#include <vector>

export module NNetModel:Pipe;

import FixedPipeline;
import DrawContext;
import Types;
import SaveCast;
import :tHighlight;
import :NNetParameters;
import :NobType;
import :NobId;
import :Nob;

using std::min;
using std::pair;
using std::vector;
using std::function;
using std::wostream;
using std::unique_ptr;

//class PosNob;    // avoid circular reference

class Pipe;

export using PipeFunc  = function<void(Pipe      &)>;
export using PipeFuncC = function<void(Pipe const&)>;
export using PipeCrit  = function<bool(Pipe const&)>;

export class Pipe : public Nob
{
public:
	using SegNr = NamedType<int, struct segNr_Parameter>;

	Pipe() = default;
	Pipe(Nob * const, Nob * const);   //TODO: Nob --> PosNob
	//Pipe(Pipe const &);   // copy constructor

	~Pipe() final = default;

	bool operator==(Nob const&) const;

	void AppendMenuItems(AddMenuFunc const&) const final;

	void Dump() const override;

	static bool TypeFits(NobType const type) { return type.IsPipeType(); }

	void Recalc     ()                         final { assert(false); }
	void SetPosNoFix(MicroMeterPnt const& pos) final { assert(false); }

	void SetStartPnt(Nob * const);   //TODO: Nob --> PosNob
	void SetEndPnt  (Nob * const);   //TODO: Nob --> PosNob

	bool IsConnectedTo(NobId const) const;

	void Emphasize(bool const) final;
	void Emphasize(bool const, bool const);

	Nob * GetStartNobPtr() { return m_pNobStart; }   //TODO: Nob --> PosNob
	Nob * GetEndNobPtr  () { return m_pNobEnd; }     //TODO: Nob --> PosNob

	Nob const * GetStartNobPtr () const { return m_pNobStart; }   //TODO: Nob --> PosNob
	Nob const * GetEndNobPtr   () const { return m_pNobEnd;   }   //TODO: Nob --> PosNob
	size_t      GetNrOfSegments() const { return getSegments().Size(); }

	void          RotateNob(MicroMeterPnt const&, Radian const) final { /* Pipe dir defined by endpoints */ }
	void          Reconnect()                                   final { /* nothing to connect */ };

	Radian        GetDir()       const final { return Vector2Radian(GetVector()); };
	NobIoMode     GetIoMode()    const final { return NobIoMode::internal; }
	NobType       GetNobType()   const final { return NobType::Value::pipe; }
	mV            GetPotential() const final { return getSegments().Get(); }
	void          PushVoltage(mV const v) { getSegments().Push(v); }

	MicroMeterPnt GetPos            ()                                      const final;
	bool          IsIncludedIn      (MicroMeterRect const &)                const final;
	bool          Includes          (MicroMeterPnt  const &)                const final;
	void          Check             ()                                      const final;
	void          DrawExterior      (DrawContext const &, tHighlight const) const final;
	void          DrawInterior      (DrawContext const &, tHighlight const) const final;
	void          Expand            (MicroMeterRect &)                      const final;
	void          MoveNob           (MicroMeterPnt const &)                       final;
	void          Link              (Nob const &, Nob2NobFunc const &)            final;
	void          CollectInput      ()                                            final;
	bool          CompStep          ()                                            final;
	void          PosChanged        ()                                            final;
	void          ClearDynamicData  ()                                            final;
	void          Select            (bool const)                                  final;
	void          SelectAllConnected(bool const)                                  final;

	NobId         GetStartKnotId  ()                     const;
	NobId         GetEndKnotId    ()                     const;
	MicroMeterPnt GetStartPoint   ()                     const;
	MicroMeterPnt GetEndPoint     ()                     const;
	MicroMeter    GetLength       ()                     const;
	MicroMeterPnt GetVector       ()                     const;
	MicroMeterPnt GetVector       (float const)          const;
	MicroMeterPnt GetSegmentVector()                     const;
	MicroMeter    GetSegLength    ()                     const { return GetLength() / Cast2Float(GetNrOfSegments()); };
	MicroMeter    DistPntToPipe   (MicroMeterPnt const&) const;
	mV            GetVoltageAt    (MicroMeterPnt const&) const;
	SegNr         GetSegNr        (float const f)        const { return SegNr(Cast2Int(round(f * Cast2Float(GetNrOfSegments() - 1)))); }
	mV            GetVoltage      (SegNr const segNr)    const { return getSegments().Get(segNr.GetValue()); }
	void          SetNrOfSegments(size_t const)                    const;
	void          DrawArrows(DrawContext const&, MicroMeter const) const;
	void          DislocateEndPoint();
	void          DislocateStartPoint();
	void          RecalcSegments() { recalcSegments(); /* m_bSegmentsDirty = true; */ }

	void Apply2AllSegments(auto const& func) const { getSegments().Apply2All(func); }

	void Apply2AllSensorPoints
	(
		MicroMeter const  umResolution,
		auto       const& func
	) const
	{
		float const DATA_PNTS { 10.0f };
		float const fIncCalc  { umResolution / (GetLength() * DATA_PNTS) };
		float const fInc      { min(1.0f, fIncCalc) };
		for (float fRun = 0.0f; fRun <= 1.0f; fRun += fInc)
		{
			MicroMeterPnt const umpRun { GetVector(fRun) };
			SegNr         const segNr  { GetSegNr(fRun) };
			func(*this, umpRun, segNr);
		}
	}

	friend wostream& operator<< (wostream&, Pipe const&);

    static unsigned int Size() { return sizeof(Pipe); }

private:
	Nob * m_pNobStart { nullptr };  //TODO: Nob --> PosNob
	Nob * m_pNobEnd   { nullptr };  //TODO: Nob --> PosNob

	// mutable members - lazy evaluation

	mutable FixedPipeline<mV> m_segments;
//	mutable bool              m_bSegmentsDirty { true };

	FixedPipeline<mV> const& getSegments() const;
	FixedPipeline<mV>      & getSegments();

	MicroMeterPnt dislocation() const;
	void          recalcSegments() const;
};

export Pipe const* Cast2Pipe(Nob const* pNob)
{
	assert(pNob->IsPipe());
	return static_cast<Pipe const*>(pNob);
}

export Pipe* Cast2Pipe(Nob* pNob)
{
	assert(pNob->IsPipe());
	return static_cast<Pipe*>(pNob);
}

export Pipe const& Cast2Pipe(Nob const& nob)
{
	assert(nob.IsPipe());
	return *static_cast<Pipe const*>(&nob);
}

export Pipe& Cast2Pipe(Nob &nob)
{
	assert(nob.IsPipe());
	return *static_cast<Pipe *>(&nob);
}
