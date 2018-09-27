// observerInterface.h : 
//

#pragma once

class ObserverInterface
{
public:
    virtual void SetDisplayRate( int ) = 0;
    virtual void SetDirtyFlag  ( )     = 0;
};
