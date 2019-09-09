// ViewCollection.h
//
// Utilities

#pragma once

#include "assert.h"
#include <vector>
#include "ObserverInterface.h"

class ViewCollection
{
public:

	virtual void NotifyAll( bool const bImmediately )
	{
		for ( auto &v : m_aView )
		{
			v->Notify( bImmediately );
		}
	}

	void Register( ObserverInterface * const pObserver )
	{
		assert( pObserver != nullptr );
		m_aView.push_back( pObserver );
	}

	void Clear( )
	{
		m_aView.clear();
	}

private:

	std::vector< ObserverInterface * > m_aView;
};
