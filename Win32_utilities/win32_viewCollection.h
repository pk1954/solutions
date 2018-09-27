// win32_viewCollection.h
//

#pragma once

#include <vector>

class Observer;
class RootWindow;

class ViewCollection
{
public:
    ViewCollection( ) {};
    virtual ~ViewCollection( );

    void NotifyObservers( ) const;

    Observer * AttachObserver( RootWindow const *, UINT const );
    Observer * GetObserver   ( RootWindow const * );
    void       SetDisplayRate( RootWindow const *, const INT );

private:
    std::vector< Observer * > m_aView;
};
