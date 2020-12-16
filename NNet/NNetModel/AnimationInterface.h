// AnimationInterface.h
//
// NNetModel

#pragma once

#include "ShapeId.h"

class AnimationInterface
{
public:

	~AnimationInterface( ) {};

	virtual void Initialize( Observable * const   ) = 0;
	virtual void Start( ShapeId           const   ) = 0;
	virtual void Start( MicroMeterCircle  const & ) = 0;
	virtual void Stop( )                            = 0;
};
