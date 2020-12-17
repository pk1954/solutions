// AnimationInterface.h
//
// NNetModel

#pragma once

#include "MoreTypes.h"

class Observable;

class AnimationInterface
{
public:

	~AnimationInterface( ) {};

	virtual void Initialize( Observable         * const ) = 0;
	virtual void Start( MicroMeterCircle  const * const ) = 0;
	virtual void Stop( )                                  = 0;
};
