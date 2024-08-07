// UPNobList.ixx
//
// NNetModel

module;

#include <cassert>

export module NNetModel:UPNobList;

import std;
import SaveCast;
import Types;
import :NobType;
import :NobId;
import :Nob;

using std::move;
using std::array;
using std::vector;
using std::function;
using std::unique_ptr;
using std::make_unique;
using std::ranges::any_of;
using std::ranges::for_each;

export class UPNobList
{
public:

	UPNobList() = default;
	UPNobList(const UPNobList &);
	~UPNobList() = default;

	UPNobList & operator=  (UPNobList const&);
	bool        operator== (UPNobList const&) const;

	bool  IsEmpty     ()               const { return   m_list.empty(); }
	bool  IsNotEmpty  ()               const { return ! m_list.empty(); }
	int   Size        ()               const { return Cast2Int(m_list.size()); }
	bool  IsEmptySlot (NobId const id) const { return GetAt(id) == nullptr; }
	bool  IsNobDefined(NobId const id) const { return GetAt(id) != nullptr; }
	bool  IsValidNobId(NobId const id) const { return (0 <= id.GetValue()) && (id.GetValue() < Size()); }
	Nob * Front       ()               const { return   m_list[0].get(); }
	Nob * GetAt       (NobId const id) const { return   m_list[id.GetValue()].get(); }
	Nob & GetRef      (NobId const id)       { return * m_list[id.GetValue()].get(); }
	void  IncreaseSize(long  const nr)       { m_list.resize(m_list.size() + nr); }
	void  ReduceSize  (long  const nr)       { m_list.resize(m_list.size() - nr); }
	void  Reserve     (long  const nr)       { m_list.reserve(nr); }

	void         Clear();
	UPNob        ExtractNob(NobId const);
	Nob *        ReplaceNob(UPNob);
	void         SetNob2Slot(NobId const, UPNob); // only for special situations
	void         SetNob2Slot(UPNob);              // only for special situations
	void         SelectAllNobs(bool const)              const;
	void         DeselectAllNobs ()                     const;
	void         CheckNobList    ()                     const;
	void         Dump            ()                     const;
	bool         Contains        (Nob const*)           const;
	unsigned int CountInSelection(NobType const)        const;
	unsigned int GetCounter      (NobType const)        const;
	unsigned int GetCounter      ()                     const;
	void         Move            (MicroMeterPnt const&) const;
	void         Reconnect       (NobId const)          const;

	unique_ptr<vector<Nob*>> GetAllSelected();

	enum class SelMode { allNobs, selectedNobs };
	MicroMeterRect CalcEnclosingRect(SelMode const = SelMode::allNobs) const;

	NobId Push(UPNob);
	UPNob Pop() { return Pop<Nob>(); }

	bool AnyNobsSelected() const { return any_of(m_list, IsSelected); }  //TODO: optimize , cache in bool

	NobId FindNobAt(MicroMeterPnt const pnt, auto const& crit) const
	{
		for (size_t i = m_list.size(); i-- > 0;)
		{
			Nob const* pNob = m_list[i].get();
			if (pNob && pNob->Includes(pnt))
			{
				while (pNob->HasParentNob())
					pNob = pNob->GetParentNob();
				if (crit(*pNob))
					return pNob->GetId();
			}
		};
		return NobId(NO_NOB);
	}

	template <Nob_t T>
	unique_ptr<T> Pop()
	{
		unique_ptr<T> upT{ unique_ptr<T>(static_cast<T*>(m_list.back().release())) };
		decCounter(upT.get());
		m_list.pop_back();
		return upT;
	}

	MicroMeterPnt CenterOfGravity(auto const& crit) const
	{
		MicroMeterPnt umPntRes{ MicroMeterPnt::ZERO_VAL() };
		size_t        counter{ 0 };

		Apply2AllC
		(
			[&umPntRes, &counter, &crit](Nob const& nob) 
			{
				if (nob.IsPosNob() && crit(nob))
				{
					umPntRes += nob.GetPos();
					++counter;
				}
			}
		);

		umPntRes /= static_cast<float>(counter);
		return umPntRes;
	}

	void Apply2AllC(auto const& f) const
	{
		for_each(m_list, [&f](auto const& it) { if (it) f(*it.get()); });
	}

	bool Apply2AllB(auto const& f) const
	{
		return any_of(m_list, [&f](auto const& it) { return it && f(*it.get()); });
	}

	void Apply2AllSelected(NobType const t, auto const& f) const
	{
		for_each(m_list, [&f, t](auto const& p) { if (p && p->IsSelected() && (p->GetNobType() == t)) f(*p.get()); });
	}

	template <Nob_t T>    // const version
	void Apply2AllC(auto const& func) const
	{
		for (auto const& it : m_list)
		{
			if (it && HasType<T>(*it))
				func(static_cast<T const&>(*it));
		}
	}

	template <Nob_t T>    // non const version
	void Apply2All(auto const& func)
	{
		for (size_t i = 0; i < m_list.size(); ++i)  // use normal loop instead of range-based loop
		{                                           // vector may be enlarged in loop
			if (m_list[i] && HasType<T>(*m_list[i]))
				func(static_cast<T&>(*m_list[i]));
		}
	}

	template <Nob_t T>    // const version
	void Apply2AllSelectedC(auto const& func) const
	{
		Apply2AllC<T>([func](T const& s) { if (s.IsSelected()) func(s); });
	}

	template <Nob_t T>    // non const version
	void Apply2AllSelected(auto const& func)
	{
		Apply2All<T>([func](T& s) { if (s.IsSelected()) func(s); });
	}

	template <Nob_t T>   // const version
	void Apply2AllInRectC(MicroMeterRect const& r, auto const& func) const
	{
		Apply2AllC<T>([&r, &func](T const& s) { if (s.IsIncludedIn(r)) func(s); });
	}

	template <Nob_t T>   // non const version
	void Apply2AllInRect(MicroMeterRect const& r, auto const& func)
	{
		Apply2All<T>([&r, &func](T& s) { if (s.IsIncludedIn(r)) func(s); });
	}

NobId ModelFindNobAt
(
	MicroMeterPnt const& umPoint, 
	auto          const& crit
) const // TODO: Template!
{
	NobId idRes{ NO_NOB };

	idRes = FindNobAt(umPoint, [&crit](Nob const& s) { return s.IsIoConnector() && crit(s); });
	if (IsDefined(idRes))
		return idRes;

	idRes = FindNobAt(umPoint, [&crit](Nob const& s) { return s.IsNeuron() && crit(s); });
	if (IsDefined(idRes))
		return idRes;

	idRes = FindNobAt(umPoint, [&crit](Nob const& s) { return s.IsIoLine() && (!s.HasParentNob()) && crit(s); });
	if (IsDefined(idRes))
		return idRes;

	idRes = FindNobAt(umPoint, [&crit](Nob const& s) { return s.IsKnot() && crit(s); });
	if (IsDefined(idRes))
		return idRes;

	idRes = FindNobAt(umPoint, [&crit](Nob const& s) { return s.IsSynapse() && crit(s); });
	if (IsDefined(idRes))
		return idRes;

	idRes = FindNobAt(umPoint, [&crit](Nob const& s) { return s.IsFork() && crit(s); });
	if (IsDefined(idRes))
		return idRes;

	idRes = FindNobAt(umPoint, [&crit](Nob const& s) { return s.IsPipe() && crit(s); });
	if (IsDefined(idRes))
		return idRes;

	return NO_NOB;
}

private:

	unsigned int& counter(Nob const& nob)
	{
		return m_nobsOfType[static_cast<unsigned int>(nob.GetNobType().GetValue())];
	}

	void incCounter(Nob const* pNob) { if (pNob) ++counter(*pNob); }
	void decCounter(Nob const* pNob) { if (pNob) --counter(*pNob); }

	void countNobs();
	void copy(UPNobList const&);

	vector<UPNob>                                 m_list{};
	array<unsigned int, NobType::NR_OF_NOB_TYPES> m_nobsOfType{};
};

export UPNob ShallowCopy(Nob const &);
