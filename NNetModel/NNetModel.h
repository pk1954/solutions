// NNetModel.h 
//
// NNetModel

#pragma once

#include "util.h"
#include "ModelInterface.h"

class ObserverInterface;
class EventInterface;

class NNetModel : public ModelInterface
{
public:

	virtual ~NNetModel( ) { };

	// readOnly functions

	BYTES const GetCoreSize()
	{ 
		return BYTES(sizeof(NNetModel)); 
	};

	// manipulating functions

	virtual void CopyModelData( ModelInterface const * const src )
	{
		* this = * static_cast<NNetModel const *>( src );
	}

	virtual void Compute( );

	virtual void ResetAll( );

	// static functions  

	static void        InitClass( int const, ObserverInterface * const, EventInterface * const );
	static NNetModel * CreateCore();
	static void        DestroyCore( NNetModel * );

private:

	int m_iDummy;
};
