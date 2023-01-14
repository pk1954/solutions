// Nob.ixx
//
// NNetModel

module;

#include <cassert>
#include <string>
#include <vector>
#include <iostream>
#include <type_traits>   
#include <functional>   
#include <compare>
#include "d2d1helper.h"

export module NNetModel:Nob;

import BoolOp;
import Types;
import DrawContext;
import :NNetParameters;
import :tHighlight;
import :MicroMeterPosDir;
import :NobType;
import :NobId;

using D2D1::ColorF;
using std::is_base_of;
using std::remove_pointer_t;
using std::remove_pointer;
using std::unique_ptr;
using std::wostream;
using std::wstring;
using std::function;
using std::vector;

export enum class NobIoMode { input, output, internal };

class Nob;

export using UPNob       = unique_ptr<Nob>;
export using Nob2NobFunc = function<Nob * (Nob const *)>;
export using AddMenuFunc = function<void(int const)>;
export using PushFunc    = function<void(UPNob)>;

export class Nob
{
public:
	static void SetParams(NNetParameters const * const pParams) { m_pParameters = pParams; }

	static bool TypeFits(NobType const) { return true; }  // every nob type is a Nob

	explicit Nob(NobType const);
	Nob(Nob const&);
	virtual ~Nob() = default;

	virtual void Check() const;
	virtual void Dump() const;

	virtual bool operator==(Nob const & ) const;

	bool operator!=(Nob const & nob) const { return !(this->Nob::operator==(nob)); };

	virtual void SetDir(Radian const );

	virtual void Recalc     ()                        = 0;
	virtual void SetPosNoFix(MicroMeterPnt    const&) = 0;
	virtual void SetPosDir  (MicroMeterPosDir const&);
	virtual void AppendMenuItems(AddMenuFunc const&) const;

	void SetPos(MicroMeterPnt const&);

	MicroMeterPosDir GetPosDir() { return MicroMeterPosDir(GetPos(), GetDir());	};

	virtual bool          IsIncludedIn(MicroMeterRect const &) const { assert(false); return false; }
	virtual bool          Includes    (MicroMeterPnt  const &) const { assert(false); return false; }
	virtual bool          CompStep    ()                             { assert(false); return false; }

	virtual Radian        GetDir()    const { assert(false); return 0.0_Radian; }
	virtual MicroMeterPnt GetPos()    const { assert(false); return NP_NULL; }
	virtual NobIoMode     GetIoMode() const { assert(false); return NobIoMode::internal; }

	virtual void DrawExterior  (DrawContext    const &, tHighlight const) const {}
	virtual void DrawInterior  (DrawContext    const &, tHighlight const) const {}
	virtual void Expand        (MicroMeterRect       &)                   const {}
	virtual void MoveNob       (MicroMeterPnt  const &)                         {}
	virtual void RotateNob     (MicroMeterPnt  const &, Radian const)           {}
	virtual void CollectInput  ()                                               {}
	virtual void Link          (Nob const &, Nob2NobFunc const &)               {}
	virtual void Reconnect     ()                                               {}
	virtual void PosChanged    ()                                               {}
	virtual void DirectionDirty()                                               {}

	virtual void Select   (bool const bOn) { m_bSelected = bOn; }
	virtual void Emphasize(bool const bOn) { m_bEmphasized = bOn; }

	virtual mV   GetNextOutput()  const { return m_mVinputBuffer; }
	virtual bool IsCompositeNob() const { return false; }
	virtual bool HasParentNob()   const { return false; }
	virtual Nob* GetParentNob()   const { return nullptr; }

	virtual void ClearDynamicData() { m_mVinputBuffer.Set2Zero(); }

	virtual void SelectAllConnected(bool const) = 0;

	bool    IsInputNob   () const { return GetIoMode() == NobIoMode::input; }
	bool    IsOutputNob  () const { return GetIoMode() == NobIoMode::output; }
	bool    IsInternalNob() const { return GetIoMode() == NobIoMode::internal; }
	bool    IsSelected   () const { return m_bSelected; }
	bool    IsEmphasized () const { return m_bEmphasized; }
	bool    IsDefined    () const { return ::IsDefined(m_identifier); }
	wstring GetTypeName  () const { return NobType::GetName(m_type.GetValue()); }
	NobType GetNobType   () const { return m_type; }
	NobId   GetId        () const { return m_identifier; }

	MicroMeter GetPosX() const { return GetPos().GetX(); }
	MicroMeter GetPosY() const { return GetPos().GetY(); }

	bool IsInputConnector () const { return m_type.IsInputConnectorType(); }
	bool IsOutputConnector() const { return m_type.IsOutputConnectorType(); }
	bool IsIoConnector    () const { return m_type.IsIoConnectorType(); }
	bool IsPipe           () const { return m_type.IsPipeType(); }
	bool IsKnot           () const { return m_type.IsKnotType(); }
	bool IsFork           () const { return m_type.IsForkType(); }
	bool IsSynapse        () const { return m_type.IsSynapseType(); }
	bool IsNeuron         () const { return m_type.IsNeuronType(); }
	bool IsIoLine         () const { return m_type.IsIoLineType(); }
	bool IsInputLine      () const { return m_type.IsInputLineType(); }
	bool IsOutputLine     () const { return m_type.IsOutputLineType(); }
	bool IsPosNob         () const { return m_type.IsPosNobType(); }
	bool IsUndefined      () const { return m_type.IsUndefinedType(); }

	friend wostream & operator<< (wostream &, Nob const &);

	void  SetId(NobId const id) { m_identifier = id; }

	ColorF GetExteriorColor(tHighlight const) const;
	ColorF GetInteriorColor(tHighlight const) const;
	ColorF GetInteriorColor(mV const) const;
	ColorF GetInteriorColor() const { return GetInteriorColor(m_mVinputBuffer); }

protected:

	mV m_mVinputBuffer{ 0._mV };

	void SetType(NobType const type) { m_type = type; }

	NNetParameters const * GetParam() const { return m_pParameters; }

	void FillExternalCircle(DrawContext const&, tHighlight const) const;
	void FillInternalCircle(DrawContext const&, tHighlight const) const;

private:

	NobType m_type        { NobType::Value::undefined };
	NobId   m_identifier  { NO_NOB };
	bool    m_bSelected   { false };
	bool    m_bEmphasized { false };

	inline static NNetParameters const * m_pParameters { nullptr };
};

export MicroMeterPosDir CalcOffsetPosDir(Nob const&, MicroMeter const);

export template <typename T>
concept Nob_t = is_base_of<Nob, remove_pointer_t<T>>::value;

export template <Nob_t T> bool HasType(Nob const& nob)
{
	return remove_pointer<T>::type::TypeFits(nob.GetNobType());
}

export template <Nob_t T>
inline MicroMeterLine CalcMaxDistLine(vector<T*> const& list) // find two nobs with maximum distance
{
	MicroMeter     maxDist{ 0.0_MicroMeter };
	MicroMeterLine lineMax{ MicroMeterLine::ZERO_VAL() };
	for (auto it1 : list)
		for (auto it2 : list)    //TODO: optimize
		{
			auto const line{ MicroMeterLine(it1->GetPos(), it2->GetPos()) };
			auto const dist{ line.Length() };
			if (dist > maxDist)
			{
				maxDist = dist;
				lineMax = line;
			}
		}
	if (lineMax.GetStartPoint().GetX() > lineMax.GetEndPoint().GetX())
		lineMax.Normalize();
	return lineMax;
}

export bool IsSelected(UPNob const& upNob) { return upNob && upNob->IsSelected(); }
