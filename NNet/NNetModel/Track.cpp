// Track.cpp 
//
// NNetModel

#include "stdafx.h"
#include "Track.h"

using std::move;

Track::Track(const Track & rhs) // copy constructor
{
	for (auto const & upSignal : rhs.m_signals)
		AddSignal(move(SignalFactory::MakeSignal(*upSignal.get())));
}

bool Track::operator==(Track const & rhs) const
{
	return m_signals == rhs.m_signals;
}

void Track::AddSignal
(
	unique_ptr<Signal> pSignal,
	SignalNr     const signalNr
)
{
	m_signals.insert(m_signals.begin() + signalNr.GetValue(), move(pSignal));
}

SignalNr const Track::AddSignal(unique_ptr<Signal> upSignal)
{
	m_signals.push_back(move(upSignal));
	return SignalNr(Cast2Int(m_signals.size()) - 1);
}

unique_ptr<Signal> Track::RemoveSignal(SignalNr const signalNr)
{
	if (IsValid(signalNr))
	{
		vector<unique_ptr<Signal>>::iterator itSignal { m_signals.begin() + signalNr.GetValue() };
		unique_ptr<Signal> pSignal { move(*itSignal) };
		m_signals.erase(itSignal);
		return move(pSignal);
	}
	return nullptr;
}

void Track::Apply2AllSignals(SignalNrFunc const & func) const
{
	for (int i = 0; i < m_signals.size(); ++i)
		func(SignalNr(i)); 
}             

void Track::Apply2AllSignals(Signal::Func const & func) const
{
	for (int i = 0; i < m_signals.size(); ++i)
		if (Signal const * const pSignal { GetSignalPtr(SignalNr(i)) })
			func(* pSignal); 
}             

Signal * const Track::FindSignal(Signal::Crit const & crit)
{
	return GetSignalPtr(FindSignalNr(crit));
}

SignalNr const Track::FindSignalNr(Signal::Crit const & crit) const
{
	for (int i = 0; i < m_signals.size(); ++i)
	{ 
		SignalNr       const signalNr { SignalNr(i) };
		Signal const * const pSignal  { GetSignalPtr(signalNr) };
		if (pSignal && crit(*pSignal))
			return signalNr;  
	}
	return SignalNr::NULL_VAL();
}

Signal const * const Track::GetSignalPtr(SignalNr const signalNr) const
{
	return IsValid(signalNr) ? m_signals[signalNr.GetValue()].get() : nullptr;
}

Signal * const Track::GetSignalPtr(SignalNr const signalNr) // calling const version 
{
	return const_cast<Signal*>(static_cast<const Track&>(*this).GetSignalPtr(signalNr));
}

bool const Track::IsValid(SignalNr const signalNr) const
{
	return (0 <= signalNr.GetValue()) && (signalNr.GetValue() < m_signals.size());
}

void Track::CheckSignals() const
{
#ifdef _DEBUG
	for (const auto & pSignal : m_signals)
	{
		assert(pSignal != nullptr);
		pSignal->CheckSignal();
	}
#endif
}
