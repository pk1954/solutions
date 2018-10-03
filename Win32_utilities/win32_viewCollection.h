// win32_viewCollection.h
//

#pragma once

#include <vector>
#include "ObserverInterface.h"
#include "win32_rootWindow.h"

class ViewCollection
{
public:
    ViewCollection( ) {};

	~ViewCollection( ) {};

	void NotifyObservers( ) const
	{
		for ( auto &v : m_aView )
		{
			v->Trigger();
		}
	}

	ObserverInterface * AttachObserver( RootWindow * pRootWin )
	{
		assert( pRootWin != nullptr );
		ObserverInterface * po = pRootWin->GetObserver( );
		m_aView.push_back( po );
		return po;
	}

private:
    std::vector< ObserverInterface * > m_aView;
};
