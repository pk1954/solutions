// ModelInterface.h
//
// Utilities

#pragma once

class ModelInterface
{
public:

	virtual void CopyModelData( ModelInterface const * const ) = 0;
	virtual void Compute( )                                    = 0;
};
