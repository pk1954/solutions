// PosNob.ixx
//
// NNetModel

module;

#include <cassert>
#include <functional>
#include <type_traits>

export module NNetModel:PosNob;

import Types;
import :NobType;
import :Pipe;
import :Nob;

using std::is_base_of;
using std::remove_pointer_t;

export class PosNob : public Nob
{
public:
	PosNob(NobType const type)
	  : Nob(type)
	{ }

	bool operator==(Nob const&) const override;

	void Dump() const override;

	mV   GetVoltage()  const { return m_mVinputBuffer; }
	void SetVoltage(mV const v) { m_mVinputBuffer = v; }

	bool Includes    (MicroMeterPnt  const&) const override;
	bool IsIncludedIn(MicroMeterRect const&) const override;
	void Expand      (MicroMeterRect      &) const override;

	bool IsPrecursorOf        (Pipe   const&) const;
	bool IsSuccessorOf        (Pipe   const&) const;
	bool IsDirectlyConnectedTo(Pipe   const&) const;
	bool IsPrecursorOf        (PosNob const&) const;
	bool IsSuccessorOf        (PosNob const&) const;
	bool IsDirectlyConnectedTo(PosNob const&) const;

	void EvaluateSelectionStatus();

	virtual size_t GetNrOfInConns () const = 0;
	virtual size_t GetNrOfOutConns() const = 0;

	virtual MicroMeter GetExtension() const = 0;

	virtual void ReplaceIncoming(Pipe* const, Pipe* const) = 0;
	virtual void ReplaceOutgoing(Pipe* const, Pipe* const) = 0;

	virtual void Apply2AllInPipes (PipeFunc const&) = 0;
	virtual void Apply2AllOutPipes(PipeFunc const&) = 0;

	virtual void Apply2AllInPipesC (PipeFuncC const&) const = 0;
	virtual void Apply2AllOutPipesC(PipeFuncC const&) const = 0;

	virtual bool Apply2AllInPipesB (PipeCrit const&) const = 0;
	virtual bool Apply2AllOutPipesB(PipeCrit const&) const = 0;

	bool Apply2AllConnectedPipesB(PipeCrit const&) const;

	virtual void AddIncoming(Pipe*) { assert(false); }
	virtual void AddOutgoing(Pipe*) { assert(false); }

	virtual void SetAllIncoming(PosNob &) = 0;
	virtual void SetAllOutgoing(PosNob &) = 0;

	static bool TypeFits(NobType const type) { return type.IsPosNobType(); }
};

export PosNob const* Cast2PosNob(Nob const*);
export PosNob      * Cast2PosNob(Nob      *);

export template <typename T>
concept PosNob_t = is_base_of<PosNob, remove_pointer_t<T>>::value;
