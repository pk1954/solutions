// ViewCollection.h
//
// Utilities

#pragma once

#include <assert.h>
#include <algorithm>
#include <vector>

import ObserverInterface;

using std::vector;
using std::ranges::count;

class ViewCollection
{
public:
	virtual ~ViewCollection() = default;

	virtual void NotifyAll(bool const bImmediately)
	{
		for (auto &v : m_aView)
		{
			v->Notify(bImmediately);
		}
	}

	void Register(ObserverInterface & observer)
	{
		if (count(m_aView, &observer) == 0)
			m_aView.push_back(&observer);
	}

	void Unregister(ObserverInterface const & observer)
	{
		for (auto it = m_aView.begin(); it != m_aView.end();)
		{
			if (* it == & observer) 
				it = m_aView.erase(it);
			else 
				++it;
		}
	}

	void Clear()
	{
		m_aView.clear();
	}

private:

	vector<ObserverInterface *> m_aView;
};
