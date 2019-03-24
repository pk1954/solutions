// win32_viewCollection.h
//

#pragma once

#include <vector>
#include "ObserverInterface.h"
#include "win32_rootWindow.h"

class ViewCollection : public ObserverInterface
{
public:

	virtual void Notify( bool const bImmediately )
	{
		for ( auto &v : m_aView )
		{
			v->Notify( bImmediately );
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
