// AnimationInterface.h
//
// NNetModel

#pragma once

#include "ShapeId.h"

class AnimationInterface
{
public:

	~AnimationInterface( ) {};

	virtual void Start( ShapeId const id ) = 0;
	virtual void Stop( )                   = 0;
};
