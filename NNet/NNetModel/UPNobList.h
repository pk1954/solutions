// UPNobList.h
//
// NNetModel

#pragma once

#include <array>
#include <vector>
#include <numeric>
#include "Nob.h"
#include "NobId.h"
#include "IoNeuronList.h"
#include "NobErrorHandler.h"

using std::move;
using std::array;
using std::vector;
using std::accumulate;
using std::unique_ptr;
using std::make_unique;

class NobIdList;

class UPNobList
{
public:

	UPNobList() { }
	UPNobList( const UPNobList & );
	~UPNobList();

	UPNobList & operator=  ( UPNobList const & );
	bool        operator== ( UPNobList const & ) const;

	bool   const IsEmpty     ()               const { return m_list.size() == 0; }
	bool   const IsNotEmpty  ()               const { return m_list.size() > 0; }
	size_t const Size        ()               const { return m_list.size(); }
	NobId  const IdNewSlot   ()	              const { return NobId(Cast2Long(m_list.size())); }
	bool   const IsEmptySlot (NobId const id) const { return GetAt(id) == nullptr; }
	bool   const IsNobDefined(NobId const id) const { return GetAt(id) != nullptr; }
	bool   const IsValidNobId(NobId const id) const { return (0 <= id.GetValue()) && (id.GetValue() < Size()); }
	Nob  * const Front       ()               const { return   m_list[0].get(); }
	Nob  * const GetAt       (NobId const id) const { return   m_list[id.GetValue()].get(); }
	Nob  &       GetRef      (NobId const id)       { return * m_list[id.GetValue()].get(); }
	void         IncreaseSize(long  const nr)       { m_list.resize(m_list.size() + nr); }
	void         ReduceSize  (long  const nr)       { m_list.resize(m_list.size() - nr); }

	void               Clear             ();
	void               SetErrorHandler   (NobErrorHandler * const);
	void               SelectAllNobs     (bool const);
	void               DeselectAllNobs   () { SelectAllNobs(false); }
	UPNob              ExtractNob        (NobId const);	
	Nob        * const ReplaceNob        (UPNob);	
	void               SetNob2Slot       (NobId const, UPNob); // only for special situations
	void               SetNob2Slot       (UPNob);              // only for special situations
	void               CheckNobList      ()                                     const;
	void               Dump              ()                                     const;
	void               LinkNob           (Nob const &, Nob2NobFunc const &)     const;
	bool         const AnyNobsSelected   ()                                     const;
	void               CallErrorHandler  (NobId   const)                        const;
	unsigned int const CountInSelection  (NobType const)                        const;
	unsigned int const GetCounter        (NobType const)                        const;
	unsigned int const GetCounter        ()                                     const;
	NobId        const FindNobAt         (MicroMeterPnt const, NobCrit const &) const;
	bool         const Apply2AllB        (                     NobCrit const &) const;
	void               Apply2All         (NobFuncC const & )                    const;
	void               Apply2All         (NobFunc  const & );
	void               Apply2AllSelected (NobType const, NobFuncC const &)      const;
	void               Apply2AllSelected (NobType const, NobFunc  const &);

	unique_ptr<vector<Nob *>> GetAllSelected();

	enum class SelMode { allNobs,	selectedNobs };
	MicroMeterRect const CalcEnclosingRect(SelMode const = SelMode::allNobs) const;

	NobId const Push(UPNob);

	template <Nob_t T>
	unique_ptr<T> Pop()
	{
		unique_ptr<T> upT { unique_ptr<T>(static_cast<T*>(m_list.back().release())) };
		if (upT)
			decCounter(upT->GetNobType());
		m_list.pop_back();
		return move(upT);
	}

	template <Nob_t T>    // const version
	void Apply2All(function<void(T const &)> const & func) const
	{
		for ( auto const & it : m_list )
		{ 
			if (it && HasType<T>(*it))
				func(static_cast<T const &>(*it));
		};
	}                        

	template <Nob_t T>    // non const version
	void Apply2All(function<void(T &)> const & func)
	{
		for ( size_t i = 0; i < m_list.size(); ++i )
		{ 
			if (m_list[i] && HasType<T>(*m_list[i]))
				func(static_cast<T &>(*m_list[i]));
		};
	}                        

	template <Nob_t T>    // const version
	void Apply2AllSelected(function<void(T const &)> const & func) const
	{
		Apply2All<T>( {	[&](T const & s) { if (s.IsSelected()) func(s); } } );
	}

	template <Nob_t T>    // non const version
	void Apply2AllSelected(function<void(T &)> const & func) 
	{
		Apply2All<T>( {	[&](T & s) { if (s.IsSelected()) func(s); } } );
	}

	template <Nob_t T>   // const version
	void Apply2AllInRect( MicroMeterRect const & r, function<void(T const &)> const & func ) const
	{
		Apply2All<T>( [&](T const & s) { if ( s.IsIncludedIn(r) ) func(s); } );
	}

	template <Nob_t T>   // non const version
	void Apply2AllInRect( MicroMeterRect const & r, function<void(T &)> const & func )
	{
		Apply2All<T>( [&](T & s) { if ( s.IsIncludedIn(r) ) func(s); } );
	}

	template <Nob_t T>
	bool Apply2AllB( function<bool(T &)> const & crit ) const
	{
		bool bResult { false };
		for ( auto & it : m_list )
		{
			if (it)
			{
				if ( HasType<T>(*it) )	
					bResult = crit( static_cast<T &>(*it) );
				if ( bResult )
					break;
			}
		}
		return bResult;
	}

private:

	unsigned int const & counter(NobType const) const;
	unsigned int       & counter(NobType const);

	void incCounter(NobType const t) { ++counter(t); }
	void decCounter(NobType const t) { --counter(t); }

	void incCounter(Nob const * const ps) { if (ps) incCounter(ps->GetNobType()); }
	void decCounter(Nob const * const ps) { if (ps) decCounter(ps->GetNobType()); }

	void incCounter(UPNob const & ups) { incCounter(ups.get()); }
	void decCounter(UPNob const & ups) { decCounter(ups.get()); }

	void incCounter(NobId const & id) { incCounter(GetAt(id)); }
	void decCounter(NobId const & id) { decCounter(GetAt(id)); }

	void countNobs();
	void copy( UPNobList const & );

	vector<UPNob>                                 m_list             {};
	array<unsigned int, NobType::NR_OF_NOB_TYPES> m_nobsOfType       {};
	NobErrorHandler                             * m_pNobErrorHandler { nullptr };
};

UPNob ShallowCopy(Nob const &);

template <Nob_t T>
UPNob Copy(Nob const & nob) { return make_unique<T>(static_cast<T const &>(nob)); }
