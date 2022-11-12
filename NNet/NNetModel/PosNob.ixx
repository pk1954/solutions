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

	bool operator==(Nob const & rhs) const override
	{
		PosNob const& posNobRhs { static_cast<PosNob const&>(rhs) };
		return (this->Nob::operator==(rhs)) &&
			GetPos      ().IsCloseTo(posNobRhs.GetPos()) &&
			GetExtension().IsCloseTo(posNobRhs.GetExtension());
	}

	mV GetVoltage()  const { return m_mVinputBuffer; }
	void SetVoltage(mV const v) { m_mVinputBuffer = v; }

	bool Includes(MicroMeterPnt const& point) const override
	{
		return Distance(point, GetPos()) <= GetExtension();
	}

	bool IsIncludedIn(MicroMeterRect const& umRect) const final
	{
		return umRect.Includes(GetPos());
	}

	void Expand(MicroMeterRect& umRect) const final
	{
		umRect.Expand(GetPos());
	}

	bool IsPrecursorOf(Pipe const& pipeSucc) const
	{
		return Apply2AllOutPipesB([&pipeSucc](Pipe const& pipe) { return &pipe == &pipeSucc; });
	}

	bool IsSuccessorOf(Pipe const& pipePred) const
	{
		return Apply2AllInPipesB([&pipePred](Pipe const& pipe) { return &pipe == &pipePred; });
	}

	bool IsPrecursorOf(PosNob const& b) const
	{
		return Apply2AllOutPipesB([&b](Pipe const& p) { return p.GetEndNobPtr() == &b; });
	}

	bool IsSuccessorOf(PosNob const& b) const
	{
		return Apply2AllInPipesB([&b](Pipe const& p) { return p.GetStartNobPtr() == &b; });
	}
	
	bool IsDirectlyConnectedTo(PosNob const & posNob) const
	{
		return IsSuccessorOf(posNob) || IsPrecursorOf(posNob);
	}

	bool IsDirectlyConnectedTo(Pipe const& pipe) const
	{
		return IsDirectlyConnectedTo(*static_cast<PosNob const*>(pipe.GetStartNobPtr())) ||
			   IsDirectlyConnectedTo(*static_cast<PosNob const*>(pipe.GetEndNobPtr()));
	}
	size_t GetNrOfConnections() const { return GetNrOfInConns() + GetNrOfOutConns(); }

	MicroMeterCircle GetCircle() const { return MicroMeterCircle(GetPos(), GetExtension());	}

	void EvaluateSelectionStatus()
	{
		bool bSelected { Apply2AllConnectedPipesB([](Pipe const& p) { return p.IsSelected(); }) };
		Nob::Select(bSelected);      // if any connected pipe is selected
	}                                // PosNob must also be selected

	virtual size_t GetNrOfInConns () const = 0;
	virtual size_t GetNrOfOutConns() const = 0;

	virtual MicroMeter GetExtension() const = 0;

	virtual void ReplaceIncoming(Pipe* const, Pipe* const) = 0;
	virtual void ReplaceOutgoing(Pipe* const, Pipe* const) = 0;

	virtual void Apply2AllInPipes (PipeFunc const& f) const = 0;
	virtual void Apply2AllOutPipes(PipeFunc const& f) const = 0;

	virtual bool Apply2AllInPipesB (PipeCrit const& c) const = 0;
	virtual bool Apply2AllOutPipesB(PipeCrit const& c) const = 0;

	bool Apply2AllConnectedPipesB(PipeCrit const & c) const
	{
		return Apply2AllInPipesB(c) || Apply2AllOutPipesB(c);
	}

	virtual void AddIncoming(Pipe&) { assert(false); }
	virtual void AddOutgoing(Pipe&) { assert(false); }

	virtual void RemoveIncoming(Pipe&) { assert(false); }
	virtual void RemoveOutgoing(Pipe&) { assert(false); }

	virtual void SetIncoming(PosNob &) = 0;
	virtual void SetOutgoing(PosNob &) = 0;

	static bool TypeFits(NobType const type) { return type.IsPosNobType(); }
};

export PosNob const* Cast2PosNob(Nob const * pNob)
{
	assert(pNob);
	assert(pNob->IsPosNob());
	return static_cast<PosNob const*>(pNob);
}

export PosNob * Cast2PosNob(Nob * pNob)
{
	assert(pNob);
	assert(pNob->IsPosNob());
	return static_cast<PosNob *>(pNob);
}

export template <typename T>
concept PosNob_t = is_base_of<PosNob, remove_pointer_t<T>>::value;
