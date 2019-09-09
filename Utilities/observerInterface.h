// observerInterface.h : 
//
// Utilities

#pragma once

class ObserverInterface
{
public:
    virtual ~ObserverInterface( ) {};

    virtual void Notify( bool const ) = 0;
};
