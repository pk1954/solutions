// Track.cpp 
//
// Signals

module;

#include <cassert>
#include <iostream>
#include <vector>
#include <memory>

module Signals:Track;

import SaveCast;
import :Signal;
import :SignalNr;

using std::move;
using std::wcout;
using std::endl;
using std::unique_ptr;
using std::vector;

Track::~Track() = default;

//bool Track::operator==(Track const & rhs) const
//{
//	return m_signals == rhs.m_signals;
//}
// TODO
void Track::AddSignal
(
	unique_ptr<Signal> pSignal,
	SignalNr     const signalNr
)
{
	m_signals.insert(m_signals.begin() + signalNr.GetValue(), move(pSignal));
}

SignalNr Track::AddSignal(unique_ptr<Signal> upSignal)
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
		return pSignal;
	}
	return nullptr;
}

Signal const * Track::GetConstSignalPtr(SignalNr const signalNr) const
{
	return IsValid(signalNr) ? m_signals[signalNr.GetValue()].get() : nullptr;
}

Signal * Track::GetSignalPtr(SignalNr const signalNr) // calling const version 
{
	return IsValid(signalNr) ? m_signals[signalNr.GetValue()].get() : nullptr;
}

bool Track::IsValid(SignalNr const signalNr) const
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

void Track::Dump() const
{
	wcout << L"NrSignals: " << m_signals.size() << endl;
	wcout << '(' << endl;
	int iNr = 0;
	for (auto & it: m_signals)
	{
		wcout << L"Signal " << iNr++ << endl;
		it->Dump();
	}
	wcout << ')' << endl;
}
