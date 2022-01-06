// ViewCollection.h
//
// Utilities

#pragma once

#include <assert.h>
#include <vector>
#include "observerInterface.h"

using std::vector;

class ViewCollection
{
public:
	virtual ~ViewCollection() = default;

	virtual void NotifyAll(bool const bImmediately)
	{
		int i = 0;
		for (auto &v : m_aView)
		{
			++i;
			v->Notify(bImmediately);
		}
	}

	void Register(ObserverInterface & observer)
	{
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
