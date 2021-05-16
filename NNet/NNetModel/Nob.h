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

template <typename T> 
concept Nob_t = is_base_of<Nob, remove_pointer_t<T>>::value;

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

	Nob( NobType const );
	virtual ~Nob() {}

	virtual void Check() const;
	virtual void Dump() const;

	virtual bool operator==(Nob const &) const;

	virtual void SetDir   (Radian const);
	virtual void SetPos   (MicroMeterPoint  const &);
	virtual void SetPosDir(MicroMeterPosDir const &);

	virtual MicroMeterPosDir const GetPosDir() const;

	virtual Radian           const GetDir      ()                                                   const = 0;
	virtual MicroMeterPoint  const GetPos      ()                                                   const = 0;
	virtual NobIoMode        const GetIoMode   ()                                                   const = 0;
	virtual void                   DrawExterior(DrawContext const &, tHighlight const)              const = 0;
	virtual void                   DrawInterior(DrawContext const &, tHighlight const)              const = 0;
	virtual void                   Prepare     ()                                                         = 0;
	virtual bool             const CompStep    ()                                                         = 0;
	virtual void                   Recalc      ()                                                         = 0;
	virtual bool             const IsIncludedIn(MicroMeterRect  const &)                            const = 0;
	virtual bool             const Includes    (MicroMeterPoint const &)                            const = 0;
	virtual void                   Expand      (MicroMeterRect &)                                   const = 0;
	virtual void                   MoveNob     (MicroMeterPoint const &)                                  = 0;
	virtual void                   RotateNob   (MicroMeterPoint const &, Radian const)                    = 0;
	virtual void                   Select      (bool const, bool const)                                   = 0;
	virtual void                   Link        (Nob const &, Nob2NobFunc const &) = 0;

	virtual void Clear()               { m_mVinputBuffer = 0.0_mV; };
	virtual void SetId(NobId const id) { m_identifier = id; }
	virtual void SetParentPointers  () {};
	virtual void ClearParentPointers() {};

	virtual bool const IsCompositeNob() { return false; }

	void Select(bool const bOn) { m_bSelected = bOn; }

	bool    const IsSelected() const { return m_bSelected; }
	bool    const IsDefined () const { return ::IsDefined( m_identifier ); }
	wstring const GetName   () const { return NobType::GetName( m_type.GetValue() ); }
	NobType const GetNobType() const { return m_type; }
	NobId   const GetId     () const { return m_identifier; }

	MicroMeter const GetPosX() const { return GetPos().GetX(); }
	MicroMeter const GetPosY() const { return GetPos().GetY(); }

	bool const HasType(NobType const type) const { return m_type == type; }

	bool const IsClosedConnector() const { return m_type.IsClosedConnectorType(); }
	bool const IsAnyConnector   () const { return m_type.IsAnyConnectorType   (); }
	bool const IsConnector      () const { return m_type.IsConnectorType      (); }
	bool const IsPipe           () const { return m_type.IsPipeType           (); }
	bool const IsKnot           () const { return m_type.IsKnotType           (); }
	bool const IsNeuron         () const { return m_type.IsNeuronType         (); }
	bool const IsInputNeuron    () const { return m_type.IsInputNeuronType    (); }
	bool const IsOutputNeuron   () const { return m_type.IsOutputNeuronType   (); }
	bool const IsAnyNeuron      () const { return m_type.IsAnyNeuronType      (); }
	bool const IsBaseKnot       () const { return m_type.IsBaseKnotType       (); }
	bool const IsUndefined      () const { return m_type.IsUndefinedType      (); }

	friend wostream & operator<< ( wostream &, Nob const & );

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

	float GetFillLevel( mV const ) const;
	float GetFillLevel() const { return GetFillLevel(m_mVinputBuffer); };

	void SetType(NobType const type) { m_type = type; }

private:

	NobType m_type       { NobType::Value::undefined };
	bool    m_bSelected  { false };
	NobId   m_identifier { };
	Nob   * m_pNobParent { nullptr };
};

template <Nob_t T> bool HasType( Nob const & nob ) 
{ 
	return remove_pointer<T>::type::TypeFits( nob.GetNobType() ); 
}
