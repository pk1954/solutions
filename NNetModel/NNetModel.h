// NNetModel.h 
//
// NNetModel

#pragma once

#include "util.h"
#include "NNetTypes.h"
#include "Segment.h"
#include "Neuron.h"
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

	BYTES const GetCoreSize() const 
	{ 
		return BYTES(sizeof(NNetModel)); 
	};

	microseconds GetSimulationTime( ) const 
	{
		return m_timeStamp;
	}

	// manipulating functions

	virtual void CopyModelData( ModelInterface const * const src )
	{
		* this = * static_cast<NNetModel const *>( src );
	}

	virtual void Compute( );

	virtual void ResetAll( );

	Pipeline const * GetPipeline( ) const { return & m_pipeline; }
	Neuron   const * GetNeuron( )   const { return & m_neuron; }
	Knot     const * GetKnot( )     const { return & m_knot; }

	// static functions  

	static void        InitClass( int const, ObserverInterface * const, EventInterface * const );
	static NNetModel * CreateModel();
	static void        DestroyCore( NNetModel * );

private:

	int          m_iCounter;
	Neuron       m_neuron;
	Knot         m_knot;
	Pipeline     m_pipeline;
	microseconds m_timeStamp;
};
