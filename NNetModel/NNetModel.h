// NNetModel.h 
//
// NNetModel

#pragma once

#include "util.h"
#include "Segment.h"
#include "Pipeline.h"
#include "ModelInterface.h"

class ObserverInterface;
class EventInterface;

class NNetModel : public ModelInterface
{
public:

	NNetModel();

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

	Segment GetSegment( ) const { return m_segment; }

	Pipeline const * GetPipeline( ) const { return & m_pipeline; }

	// static functions  

	static void        InitClass( int const, ObserverInterface * const, EventInterface * const );
	static NNetModel * CreateModel();
	static void        DestroyCore( NNetModel * );

private:

	int      m_iCounter;
	Pipeline m_pipeline;
	Segment  m_segment;
};
