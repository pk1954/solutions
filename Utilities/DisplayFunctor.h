// DisplayFunctor.h
//

#pragma once

class DisplayFunctor
{
public:
    virtual ~DisplayFunctor( ) {};
    virtual bool operator() ( bool const ) const = 0;
};
