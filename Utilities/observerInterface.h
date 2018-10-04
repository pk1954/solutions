// observerInterface.h : 
//

#pragma once

class ObserverInterface
{
public:
    virtual ~ObserverInterface( ) {};

    virtual void Notify( ) = 0;
};
