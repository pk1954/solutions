// observerInterface.h : 
//

#pragma once

class ObserverInterface
{
public:
    virtual ~ObserverInterface( ) {};

    virtual void Notify( bool const ) = 0;
};
