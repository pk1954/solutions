// Observable.ixx : 
//
// Toolbox\Utilities

module;

#include <cassert>
#include <algorithm>
#include <vector>

export module Observable;

import ObserverInterface;

using std::vector;
using std::ranges::count;

export class Observable
{
public:
	virtual ~Observable() = default;

	void RegisterObserver(ObserverInterface & observer)
	{
		if (count(m_observers, &observer) == 0)
			m_observers.push_back(&observer);
	}

	void UnregisterObserver(ObserverInterface const & observer)
	{
		if (m_observers.empty())
			return;
		for (auto it = m_observers.begin(); it != m_observers.end();)
		{
			if (*it == &observer)
				it = m_observers.erase(it);
			else
				++it;
		}
	}

	void UnregisterAllObservers()
	{
		m_observers.clear();
	}

	void NotifyAll(bool const bImmediately = false)
	{
		for (auto& v : m_observers)
		{
			v->Notify(bImmediately);
		}
	}

private:
	vector<ObserverInterface*> m_observers;
};
