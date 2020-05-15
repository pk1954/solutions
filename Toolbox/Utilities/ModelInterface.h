// ModelInterface.h
//
// Utilities

#pragma once

class ModelInterface
{
public:

	virtual void CopyModelData( ModelInterface const * const ) = 0;
	virtual bool Compute( )                                    = 0;
};
