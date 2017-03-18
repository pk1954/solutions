// NextGenFunctor.h

#pragma once

#include "generationCmd.h"

class NextGenFunctor
{
public:
    virtual ~NextGenFunctor( ) {};
    virtual void operator() ( GenerationCmd ) const = 0;
};
