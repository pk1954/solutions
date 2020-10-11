// Attach2MonitorCommand.h
//
// Commands

#pragma once

#include "MoreTypes.h"
#include "Observable.h"
#include "MonitorData.h"
#include "NNetModelReaderInterface.h"
#include "Signal.h"
#include "Track.h"
#include "ShapeId.h"
#include "Command.h"
#include "Neuron.h"

class Param;

class Attach2MonitorCommand : public Command
{
public:
	Attach2MonitorCommand
	( 
		ShapeId                    const id, 
		MonitorData              * const pMonitorData,
		NNetModelReaderInterface * const pModelReaderInterface,
		Param                    * const pParam,
		Observable               * const pDynamicModelObservable
	)
	{ 
		m_pSignal = new Signal( * pModelReaderInterface, * pParam, * pDynamicModelObservable );
		m_pSignal->SetSignalSource( id );
		m_pMonitorData = pMonitorData;
	}

	~Attach2MonitorCommand( )
	{
		delete m_pSignal;
	}

	virtual void Do( NNetModelWriterInterface * const pModel ) 
	{ 
		TrackIter const itTrack = m_pMonitorData->InsertTrack( );
		itTrack->AddSignal( m_pSignal );
	}

	virtual void Undo( NNetModelWriterInterface * const pModel ) 
	{
		m_pMonitorData->DeleteTrack( );
	}

private:
	Signal      * m_pSignal      { nullptr };
	MonitorData * m_pMonitorData { nullptr }; 
};

