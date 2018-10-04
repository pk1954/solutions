// win32_viewCollection.h
//

#pragma once

#include <vector>
#include "ObserverInterface.h"
#include "win32_rootWindow.h"

class ViewCollection : public ObserverInterface
{
public:
    ViewCollection( ) 
	{ }

	~ViewCollection( )	
	{ }

	virtual void Notify( )
	{
		for ( auto &v : m_aView )
		{
			v->Notify();
		}
	}

	void AttachObserver( RootWindow * pRootWin )
	{
		assert( pRootWin != nullptr );
		m_aView.push_back( pRootWin );
	}

private:
    std::vector< ObserverInterface * > m_aView;
};
