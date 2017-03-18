// NextGenFunctor.h

#pragma once

class NextGenFunctor
{
public:
    virtual ~NextGenFunctor( ) {};
	
	virtual void OnReset( )                            const = 0;
	virtual void OnNextGeneration( )                   const = 0;
    virtual void OnAppCommand( unsigned short, short ) const = 0;
};
