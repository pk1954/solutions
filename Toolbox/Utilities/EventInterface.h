// EventInterface.h : 
//

#pragma once

class EventInterface
{
public:
    virtual ~EventInterface( ) {};

    virtual void Wait( ) = 0;
    virtual void Continue( ) = 0;
};
