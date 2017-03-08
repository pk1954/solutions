// win32_viewCollection.h
//

#pragma once

#include <vector>

class ObserverInterface;
class RootWindow;

class ViewCollection
{
public:
    ViewCollection( ) {};
    virtual ~ViewCollection( );

    void NotifyObservers( ) const;

    ObserverInterface * AttachObserver( RootWindow const *, UINT const );
    ObserverInterface * GetObserver   ( RootWindow const * );
    void                SetDisplayRate( RootWindow const *, const INT );

private:
    std::vector< ObserverInterface * > m_aView;
};
