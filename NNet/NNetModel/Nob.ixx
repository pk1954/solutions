// Nob.ixx
//
// NNetModel

export module NNetModel:Nob;

import std;
import BoolOp;
import Color;
import Types;
import ColorLUT;
import DrawContext;
import :NNetParameters;
import :tHighlight;
import :NobType;
import :NobId;

using std::is_base_of;
using std::remove_pointer_t;
using std::remove_pointer;
using std::unique_ptr;
using std::wostream;
using std::wstring;
using std::function;
using std::vector;
using std::uint16_t;

export enum class NobIoMode { input, output, internal };

class Nob;

export using UPNob       = unique_ptr<Nob>;
export using Nob2NobFunc = function<Nob * (Nob const *)>;
export using AddMenuFunc = function<void(int const)>;
export using PushFunc    = function<void(UPNob)>;

export class Nob
{
public:
	static void SetColorLut(ColorLUT const& lut) { m_pLutVoltage = &lut; }
	static void SetParams(NNetParameters const * const pParams) { m_pParameters = pParams; }

	static bool TypeFits(NobType const) { return true; }  // every nob type is a Nob

	Nob() = default;
	Nob(Nob const&);

	virtual ~Nob() = default;

	virtual void Check() const;
	virtual void Dump () const;

	bool operator==(Nob const &) const;
	bool operator!=(Nob const & nob) const { return !(this->Nob::operator==(nob)); };

	virtual void Recalc     ()                        = 0;
	virtual void SetPosNoFix(MicroMeterPnt    const&) = 0;
	virtual void AppendMenuItems(AddMenuFunc const&) const;

	void SetPos(MicroMeterPnt const&);

	virtual bool          IsIncludedIn(MicroMeterRect const &) const = 0; 
	virtual bool          Includes    (MicroMeterPnt  const &) const = 0; 
	virtual bool          CompStep    ()                             = 0; 

	virtual Radian        GetDir()    const = 0;
	virtual MicroMeterPnt GetPos()    const = 0;
	virtual NobIoMode     GetIoMode() const = 0;
	virtual MicroMeterPnt GetCenter() const { return GetPos(); }

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

	virtual void Select   (bool const bOn) { m_states = bOn ? (m_states | 0x1) : (m_states & 0xfe); }
	virtual void Emphasize(bool const bOn) { m_states = bOn ? (m_states | 0x2) : (m_states & 0xfd); }

	virtual mV   GetPotential()   const { return m_mVpotential; }
	virtual bool IsCompositeNob() const { return false; }
	virtual bool HasParentNob()   const { return false; }
	virtual Nob* GetParentNob()   const { return nullptr; }

	virtual void ClearDynamicData() { m_mVpotential.Set2Zero(); }

	virtual void    SelectAllConnected(bool const) = 0;
	virtual NobType GetNobType()             const = 0;

	bool    IsInputNob   () const { return GetIoMode() == NobIoMode::input; }
	bool    IsOutputNob  () const { return GetIoMode() == NobIoMode::output; }
	bool    IsInternalNob() const { return GetIoMode() == NobIoMode::internal; }
	bool    IsSelected   () const { return m_states & 0x1; }
	bool    IsEmphasized () const { return m_states & 0x2; }
	bool    IsDefined    () const { return ::IsDefined(m_identifier); }
	wstring GetTypeName  () const { return NobType::GetName(GetNobType().GetValue()); }
	NobId   GetId        () const { return m_identifier; }

	MicroMeter GetPosX() const { return GetPos().GetX(); }
	MicroMeter GetPosY() const { return GetPos().GetY(); }

	bool IsInputConnector () const { return GetNobType().IsInputConnectorType(); }
	bool IsOutputConnector() const { return GetNobType().IsOutputConnectorType(); }
	bool IsIoConnector    () const { return GetNobType().IsIoConnectorType(); }
	bool IsPipe           () const { return GetNobType().IsPipeType(); }
	bool IsKnot           () const { return GetNobType().IsKnotType(); }
	bool IsFork           () const { return GetNobType().IsForkType(); }
	bool IsSynapse        () const { return GetNobType().IsSynapseType(); }
	bool IsNeuron         () const { return GetNobType().IsNeuronType(); }
	bool IsIoLine         () const { return GetNobType().IsIoLineType(); }
	bool IsInputLine      () const { return GetNobType().IsInputLineType(); }
	bool IsOutputLine     () const { return GetNobType().IsOutputLineType(); }
	bool IsPosNob         () const { return GetNobType().IsPosNobType(); }
	bool IsUndefined      () const { return GetNobType().IsUndefinedType(); }

	friend wostream & operator<< (wostream &, Nob const &);

	void  SetId(NobId const id) { m_identifier = id; }

	Color GetExteriorColor(tHighlight const) const;
	Color GetInteriorColor(tHighlight const, mV const) const;
	Color GetInteriorColor(mV const) const;

	NNetParameters const * GetParam() const { return m_pParameters; }

protected:

#pragma pack(push, 1)
	mV       m_mVpotential { 0._mV };   // 4 byte
	NobId    m_identifier  { NO_NOB };  // 2 byte
	uint16_t m_states      { 0 };       // 2 byte 
#pragma pack(pop)	

private:

	inline static ColorLUT       const * m_pLutVoltage { nullptr };
	inline static NNetParameters const * m_pParameters { nullptr };
};

export template <typename T>
concept Nob_t = is_base_of<Nob, remove_pointer_t<T>>::value;

export template <Nob_t T> bool HasType(Nob const& nob)
{
	return remove_pointer<T>::type::TypeFits(nob.GetNobType());
}

export bool IsSelected(UPNob const& upNob) { return upNob && upNob->IsSelected(); }
