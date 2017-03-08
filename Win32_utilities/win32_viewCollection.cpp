// win32_viewCollection.cpp
//

#include "stdafx.h"
#include "observerInterface.h"
#include "win32_viewCollection.h"
#include "win32_rootWindow.h"

ViewCollection::~ViewCollection( )
{
    for ( auto &v : m_aView )
    {
        delete v;
    }
};

ObserverInterface * ViewCollection::GetObserver( RootWindow const * pRootWin )
{
    for ( auto &v : m_aView )
    {
        if ( v->GetWindowHandle( ) == pRootWin->GetWindowHandle( ) )
            return v;
    }

    return nullptr;
}

void ViewCollection::NotifyObservers( ) const
{
    for ( auto &v : m_aView )
    {
        v->SetDirtyFlag( );
    }
}

ObserverInterface * ViewCollection::AttachObserver( RootWindow const * pRootWin, UINT const iMilliSecs )
{
    assert( pRootWin != nullptr );
    ObserverInterface * po = new ObserverInterface( pRootWin->GetWindowHandle( ), iMilliSecs );
    m_aView.push_back( po );
    return po;
}

void ViewCollection::SetDisplayRate( RootWindow const * pRootWin, INT const iMilliSecs )
{
    ObserverInterface * po = GetObserver( pRootWin );
    po->SetDisplayRate( iMilliSecs );
}

