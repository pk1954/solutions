// Track.h : 
//
// NNetModel

#pragma once

#include <memory>
#include <vector>
#include "observable.h"
#include "NamedType.h"
#include "SignalFactory.h"
#include "Signal.h"

using std::vector;
using std::unique_ptr;

using SignalNr = NamedType<int, struct SignalNrParam>;

class SignalId;

class Track
{
public:

	Track()                           = default;  // constructor   
	~Track()                          = default;  // destructor
	Track             (      Track&&) = delete;   // move constructor
	Track & operator= (const Track&)  = delete;   // copy assignment operator
	Track & operator= (      Track&&) = delete;   // move assignment operator
	Track             (const Track&);             // copy constructor

	bool operator==(Track const &) const;

	void Dump()  const;
	void CheckSignals() const;

	void               AddSignal   (unique_ptr<Signal>, SignalNr const);
	SignalNr           AddSignal   (unique_ptr<Signal>);
	unique_ptr<Signal> RemoveSignal(SignalNr const);

	Signal const * GetConstSignalPtr(SignalNr const) const;
	Signal       * GetSignalPtr     (SignalNr const);
	bool           IsValid          (SignalNr const) const;
	bool           IsEmpty          () const { return m_signals.empty(); }

	template<class FUNC>
	void Apply2AllSignalNrsC(FUNC const & func) const
	{
		for (int i = 0; i < m_signals.size(); ++i)
			func(SignalNr(i)); 
	}             

	template<class FUNC>
	void Apply2AllSignalsC(FUNC const & func) const
	{
		for (int i = 0; i < m_signals.size(); ++i)
		{
			if (Signal const * pSignal { GetConstSignalPtr(SignalNr(i)) })
				func(* pSignal); 
		}
	}             

	template<class FUNC>
	void Apply2AllSignals(FUNC const & func)
	{
		for (int i = 0; i < m_signals.size(); ++i)
			if (Signal * pSignal { GetSignalPtr(SignalNr(i)) })
				func(* pSignal); 
	}             

	template<class CRIT>
	SignalNr FindSignalNr(CRIT const & crit) const
	{
		for (int i = 0; i < m_signals.size(); ++i)
		{ 
			SignalNr       const signalNr { SignalNr(i) };
			Signal const * const pSignal  { GetConstSignalPtr(signalNr) };
			if (pSignal && crit(*pSignal))
				return signalNr;  
		}
		return SignalNr::NULL_VAL();
	}

	template<class CRIT>
	Signal const * FindSignal(CRIT const & crit) const
	{
		return GetConstSignalPtr(FindSignalNr(crit));
	}

private:
	vector<unique_ptr<Signal>> m_signals;
};
