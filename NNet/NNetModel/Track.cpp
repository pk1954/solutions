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

SignalNr const Track::AddSignal(unique_ptr<Signal> pSignal)
{
	m_signals.push_back(move(pSignal));
	return SignalNr(Cast2Int(m_signals.size() - 1));
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

void Track::Apply2AllSignals(SignalFunc const & func) const
{
	for (int i = 0; i < m_signals.size(); ++i)
		if (Signal const * const pSignal { GetSignalPtr(SignalNr(i)) })
			func(* pSignal); 
}             

Signal * const Track::FindSignal(SignalCrit const & crit)
{
	for (int i = 0; i < m_signals.size(); ++i)
	{ 
		Signal * const pSignal { GetSignalPtr(SignalNr(i)) };
		if (pSignal && crit(*pSignal))
			return pSignal;  
	}
	return nullptr;
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
