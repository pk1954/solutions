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
#include "NobType.h"
#include "NobId.h"

class Nob;
class Param;
class DrawContext;
class SignalGenerator;

using D2D1::ColorF;
using std::is_base_of;
using std::remove_pointer_t;
using std::remove_pointer;
using std::unique_ptr;
using std::wostream;
using std::wstring;

using UPNob       = unique_ptr<Nob>;
using UPNobC      = unique_ptr<const Nob>;
using Nob2NobFunc = function<Nob * (Nob const *)>;
using AddMenuFunc = function<void  (int const  )>;

enum class NobIoMode { input, output, internal };

class Nob
{
public:
	static void SetParams(Param const * const pParams) { m_pParameters = pParams; }

	static bool TypeFits(NobType const) { return true; }  // every nob type is a Nob

	explicit Nob(NobType const);
	Nob(Nob const &);
	virtual ~Nob() = default;

	virtual void Check() const;
	virtual void Dump() const;

	bool operator==(Nob const &) const;
	bool operator!=(Nob const & nob) const { return ! (this->Nob::operator==(nob)); };

	virtual void SetDir   (Radian const);
	virtual void SetPos   (MicroMeterPnt  const &);
	virtual void SetPosDir(MicroMeterPosDir const &);
	virtual void AppendMenuItems(AddMenuFunc const &) const;

	virtual MicroMeterPosDir GetPosDir() const;

	virtual Radian        GetDir      ()                                      const = 0;
	virtual MicroMeterPnt GetPos      ()                                      const = 0;
	virtual NobIoMode     GetIoMode   ()                                      const = 0;
	virtual void          DrawExterior(DrawContext const &, tHighlight const) const = 0;
	virtual void          DrawInterior(DrawContext const &, tHighlight const) const = 0;
	virtual void          Expand      (MicroMeterRect       &)                const = 0;
	virtual bool          IsIncludedIn(MicroMeterRect const &)                const = 0;
	virtual bool          Includes    (MicroMeterPnt  const &)                const = 0;
	virtual void          MoveNob     (MicroMeterPnt  const &)                      = 0;
	virtual void          RotateNob   (MicroMeterPnt  const &, Radian const)        = 0;
	virtual void          Prepare     ()                                            = 0;
	virtual bool          CompStep    ()                                            = 0;
	virtual void          Link        (Nob const &, Nob2NobFunc const &)            = 0;

	virtual void Recalc() { };

	virtual void Select   (bool const bOn) { m_bSelected   = bOn; }
	virtual void Emphasize(bool const bOn) { m_bEmphasized = bOn; }

	virtual mV   GetNextOutput()  const { return m_mVinputBuffer; }
	virtual bool IsCompositeNob() const { return false; }

	virtual void ClearDynamicData()     { m_mVinputBuffer.Set2Zero(); }
	virtual void Reconnect()            {};

	virtual void                    SetSigGen (SignalGenerator * const) { }
	virtual SignalGenerator       * GetSigGen ()                        { return nullptr; }
	virtual SignalGenerator const * GetSigGenC() const                  { return nullptr; }

	bool    IsInputNob   () const { return GetIoMode() == NobIoMode::input;    }
	bool    IsOutputNob  () const { return GetIoMode() == NobIoMode::output;   }
	bool    IsInternalNob() const { return GetIoMode() == NobIoMode::internal; }
	bool    IsSelected   () const { return m_bSelected; }
	bool    IsEmphasized () const { return m_bEmphasized; }
	bool    IsDefined    () const { return ::IsDefined(m_identifier); }
	wstring GetName      () const { return NobType::GetName(m_type.GetValue()); }
	NobType GetNobType   () const { return m_type; }
	NobId   GetId        () const { return m_identifier; }

	MicroMeter GetPosX() const { return GetPos().GetX(); }
	MicroMeter GetPosY() const { return GetPos().GetY(); }

	bool IsInputConnector () const { return m_type.IsInputConnectorType (); }
	bool IsOutputConnector() const { return m_type.IsOutputConnectorType(); }
	bool IsIoConnector    () const { return m_type.IsIoConnectorType    (); }
	bool IsPipe           () const { return m_type.IsPipeType           (); }
	bool IsKnot           () const { return m_type.IsKnotType           (); }
	bool IsNeuron         () const { return m_type.IsNeuronType         (); }
	bool IsIoLine         () const { return m_type.IsIoLineType         (); }
	bool IsInputLine      () const { return m_type.IsInputLineType      (); }
	bool IsOutputLine     () const { return m_type.IsOutputLineType     (); }
	bool IsAnyNeuron      () const { return m_type.IsAnyNeuronType      (); }
	bool IsBaseKnot       () const { return m_type.IsBaseKnotType       (); }
	bool IsUndefined      () const { return m_type.IsUndefinedType      (); }

	friend wostream & operator<< (wostream &, Nob const &);

	bool  HasParentNob() const        { return m_pNobParent != nullptr; }
	Nob * GetParentNob() const        { return m_pNobParent; }
	void  SetParentNob(Nob * const p) { m_pNobParent = p; }

	void  SetId(NobId const id) { m_identifier = id; }

	ColorF GetExteriorColor(tHighlight const) const;
	ColorF GetInteriorColor(tHighlight const) const;
	ColorF GetInteriorColor(mV const) const;
	ColorF GetInteriorColor() const { return GetInteriorColor(m_mVinputBuffer); }

protected:

	mV m_mVinputBuffer { 0._mV };

	void SetType(NobType const type) { m_type = type; }

	Param const * GetParam() const { return m_pParameters; }

private:

	NobType m_type        { NobType::Value::undefined };
	Nob   * m_pNobParent  { nullptr };
	NobId   m_identifier  { NO_NOB };
	bool    m_bSelected   { false };
	bool    m_bEmphasized { false };

	inline static Param const * m_pParameters{ nullptr };
};

MicroMeterPosDir CalcOffsetPosDir(Nob const &, MicroMeter const);

template <typename T> 
concept Nob_t = is_base_of<Nob, remove_pointer_t<T>>::value;

template <Nob_t T> bool HasType(Nob const & nob)
{ 
	return remove_pointer<T>::type::TypeFits(nob.GetNobType()); 
}

template <Nob_t T>
inline MicroMeterLine CalcMaxDistLine(vector<T *> const & list) // find two nobs with maximum distance
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

static bool IsSelected(UPNob const & upNob) { return upNob && upNob->IsSelected(); }
