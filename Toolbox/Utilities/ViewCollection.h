// ViewCollection.h
//
// Utilities

#pragma once

#include <assert.h>
#include <vector>
#include "ObserverInterface.h"

using std::vector;

class ViewCollection
{
public:

	virtual void NotifyAll( bool const bImmediately )
	{
		int i = 0;
		for ( auto &v : m_aView )
		{
			++i;
			v->Notify( bImmediately );
		}
	}

	void Register( ObserverInterface * const pObserver )
	{
		assert( pObserver != nullptr );
		m_aView.push_back( pObserver );
	}

	void Unregister( ObserverInterface * const pObserver )
	{
		assert( pObserver != nullptr );
		for (auto it = m_aView.begin(); it != m_aView.end();)
		{
			if ( * it == pObserver ) 
				it = m_aView.erase(it);
			else 
				++it;
		}
	}

	void Clear( )
	{
		m_aView.clear();
	}

private:

	vector< ObserverInterface * > m_aView;
};
