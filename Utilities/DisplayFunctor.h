// DisplayFunctor.h
//

#pragma once

class DisplayFunctor
{
public:
    virtual ~DisplayFunctor( ) {};
    virtual void Trigger( bool const = false ) const = 0;
};
