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

	void AttachObserver( ObserverInterface * pObserver )
	{
		assert( pObserver != nullptr );
		m_aView.push_back( pObserver );
	}

private:

    std::vector< ObserverInterface * > m_aView;
};
