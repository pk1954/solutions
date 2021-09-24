// Nob.h
//
// NNetModel

#pragma once

#include <type_traits>   
#include "d2d1helper.h"
#include "BoolOp.h"
#include "MoreTypes.h"
#include "tHighlightType.h"
#include "MicroMeterPosDir.h"
#include "NNetParameters.h"
#include "NobType.h"
#include "NobId.h"

class Nob;
class DrawContext;

using std::is_base_of;
using std::remove_pointer_t;
using std::remove_pointer;
using std::unique_ptr;
using std::wostream;
using std::wstring;

using UPNob       = unique_ptr<Nob>;
using NobFuncC    = function<void  (Nob const &)>;
using NobFunc     = function<void  (Nob       &)>;
using NobCrit     = function<bool  (Nob const &)>;
using Nob2NobFunc = function<Nob * (Nob const *)>;

static NobCrit const NobCritAlwaysTrue { [](auto & s) { return true; } };

enum class NobIoMode { input, output, internal };

class Nob
{
public:
	static void Initialize(Param const & param) { m_pParameters = & param; }

	static bool const TypeFits(NobType const type) { return true; }  // every nob type is a Nob

	Nob(NobType const);
	Nob(Nob const &);
	virtual ~Nob() {}

	virtual void Check() const;
	virtual void Dump() const;

	virtual bool operator==(Nob const &) const;
	bool operator!=(Nob const & nob) const { return ! (this->Nob::operator==(nob)); };

	virtual void SetDir   (Radian const);
	virtual void SetPos   (MicroMeterPnt  const &);
	virtual void SetPosDir(MicroMeterPosDir const &);

	virtual MicroMeterPosDir const GetPosDir() const;

	virtual Radian        const GetDir      ()                                      const = 0;
	virtual MicroMeterPnt const GetPos      ()                                      const = 0;
	virtual NobIoMode     const GetIoMode   ()                                      const = 0;
	virtual void                DrawExterior(DrawContext const &, tHighlight const) const = 0;
	virtual void                DrawInterior(DrawContext const &, tHighlight const) const = 0;
	virtual void                Prepare     ()                                            = 0;
	virtual bool          const CompStep    ()                                            = 0;
	virtual void                Recalc      ()                                            = 0;
	virtual bool          const IsIncludedIn(MicroMeterRect  const &)               const = 0;
	virtual bool          const Includes    (MicroMeterPnt const &)                 const = 0;
	virtual void                Expand      (MicroMeterRect &)                      const = 0;
	virtual void                MoveNob     (MicroMeterPnt const &)                       = 0;
	virtual void                RotateNob   (MicroMeterPnt const &, Radian const)         = 0;
	virtual void                Link        (Nob const &, Nob2NobFunc const &)            = 0;

	virtual bool const IsCompositeNob() const { return false; }

	virtual void       Select(bool const bOn) { m_bSelected = bOn; }
	virtual void       Clear()                { m_mVinputBuffer = 0.0_mV; };
	virtual void       SetId(NobId const id)  { m_identifier = id; }
	virtual void       Reconnect          ()  {};

	bool    const IsInputNob   () const { return GetIoMode() == NobIoMode::input;    }
	bool    const IsOutputNob  () const { return GetIoMode() == NobIoMode::output;   }
	bool    const IsInternalNob() const { return GetIoMode() == NobIoMode::internal; }
	bool    const IsSelected   () const { return m_bSelected; }
	bool    const IsDefined    () const { return ::IsDefined(m_identifier); }
	wstring const GetName      () const { return NobType::GetName(m_type.GetValue()); }
	NobType const GetNobType   () const { return m_type; }
	NobId   const GetId        () const { return m_identifier; }

	MicroMeter const GetPosX() const { return GetPos().GetX(); }
	MicroMeter const GetPosY() const { return GetPos().GetY(); }

	bool const IsInputConnector () const { return m_type.IsInputConnectorType (); }
	bool const IsOutputConnector() const { return m_type.IsOutputConnectorType(); }
	bool const IsIoConnector    () const { return m_type.IsIoConnectorType    (); }
	bool const IsPipe           () const { return m_type.IsPipeType           (); }
	bool const IsKnot           () const { return m_type.IsKnotType           (); }
	bool const IsNeuron         () const { return m_type.IsNeuronType         (); }
	bool const IsIoNeuron       () const { return m_type.IsIoNeuronType       (); }
	bool const IsInputNeuron    () const { return m_type.IsInputNeuronType    (); }
	bool const IsOutputNeuron   () const { return m_type.IsOutputNeuronType   (); }
	bool const IsAnyNeuron      () const { return m_type.IsAnyNeuronType      (); }
	bool const IsBaseKnot       () const { return m_type.IsBaseKnotType       (); }
	bool const IsUndefined      () const { return m_type.IsUndefinedType      (); }

	friend wostream & operator<< (wostream &, Nob const &);

	bool  const HasParentNob() const        { return m_pNobParent != nullptr; }
	Nob * const GetParentNob() const        { return m_pNobParent; }
	void        SetParentNob(Nob * const p) { m_pNobParent = p; }

protected:

	mV m_mVinputBuffer { 0._mV };

	inline static Param const * m_pParameters{ nullptr };

	D2D1::ColorF GetExteriorColor(tHighlight const) const;
	D2D1::ColorF GetInteriorColor(tHighlight const) const;
	D2D1::ColorF GetInteriorColor(mV const) const;
	D2D1::ColorF GetInteriorColor() const { return GetInteriorColor(m_mVinputBuffer); }

	float GetFillLevel(mV const) const;
	float GetFillLevel() const { return GetFillLevel(m_mVinputBuffer); };

	void SetType(NobType const type) { m_type = type; }

private:

	NobType m_type       { NobType::Value::undefined };
	bool    m_bSelected  { false };
	NobId   m_identifier { NO_NOB };
	Nob   * m_pNobParent { nullptr };
};

MicroMeterPosDir const CalcOffsetPosDir(Nob const &, MicroMeter const);

template <typename T> 
concept Nob_t = is_base_of<Nob, remove_pointer_t<T>>::value;

template <Nob_t T> bool HasType(Nob const & nob) 
{ 
	return remove_pointer<T>::type::TypeFits(nob.GetNobType()); 
}

template <Nob_t T>
inline MicroMeterLine const CalcMaxDistLine(vector<T *> const & list) // find two nobs with maximum distance
{
	MicroMeter     maxDist { 0.0_MicroMeter };   	
	MicroMeterLine lineMax { MicroMeterLine::ZERO_VAL() };
	for (auto it1 : list)
		for (auto it2 : list)    //TODO: optimize
		{
			auto const line { MicroMeterLine(it1->GetPos(), it2->GetPos()) };
			auto const dist { line.Length() };
			if (dist > maxDist)
			{
				maxDist = dist;
				lineMax = line;
			}
		}
	if (lineMax.GetStartPoint().GetX() >  lineMax.GetEndPoint().GetX())
		lineMax.Normalize();
	return lineMax;
}
