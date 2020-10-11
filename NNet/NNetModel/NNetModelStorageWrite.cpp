// NNetModelStorageWrite.cpp
//
// NNetModel

#include "stdafx.h"
#include <assert.h>
#include "Signal.h"
#include "Track.h"
#include "MonitorData.h"
#include "ModelDescription.h"
#include "NNetModelReaderInterface.h"
#include "NNetModelWriterInterface.h"
#include "win32_HiResTimer.h"
#include "NNetModelStorage.h"

using std::wcout;
using std::endl;
using std::put_time;
using std::wofstream;

void NNetModelStorage::Write( wostream & out )
{
    HiResTimer timer;
    timer.Start();

    static int const BUF_SIZE { 128 };

    struct tm tmDest;
    time_t t = std::time( nullptr );
    localtime_s( &tmDest, &t );

    out << L"# NNetModel" << endl;
    out << L"# Created " << put_time( &tmDest, L"%d-%m-%Y %H-%M-%S" ) << endl;

    WCHAR  infoBuf[BUF_SIZE];
    DWORD  bufCharCount = BUF_SIZE;

    // Get and display the name of the computer.
    GetComputerName( infoBuf, &bufCharCount );
    out << L"# Computer name: " << infoBuf << endl;

    // Get and display the user name.
    GetUserName( infoBuf, &bufCharCount );
    out << L"# User name: " << infoBuf << endl;
    out << endl;

    out << L"Protocol version " << PROTOCOL_VERSION << endl;
    out << endl;

    writeDescription( out );

    Apply2GlobalParameters
    ( 
        [&]( tParameter const & par ) 
        {
            out << L"GlobalParameter " << GetParameterName( par ) << L" = "
                << m_pParam->GetParameterValue( par ) 
                << endl; 
        }
    );

    m_CompactIds.resize( m_pModelReaderInterface->GetSizeOfShapeList() );
    ShapeId idCompact( 0 );
    for ( int i = 0; i < m_CompactIds.size( ); ++i )
    {
        m_CompactIds[ i ] = m_pModelReaderInterface->GetConstShape( ShapeId( i ) )
            ? idCompact++
            : NO_SHAPE;
    }

    out << endl;
    out << L"NrOfShapes = " << idCompact << endl;
    out << endl;

    m_pModelWriterInterface->Apply2All<BaseKnot>( [&]( BaseKnot & shape ) { WriteShape( out, shape ); } );
    m_pModelWriterInterface->Apply2All<Pipe    >( [&]( Pipe     & shape ) { WriteShape( out, shape ); } );

    out << endl;

    m_pModelWriterInterface->Apply2All<InputNeuron>
    (
        [&]( InputNeuron & inpNeuron )
        { 
            out << L"ShapeParameter InputNeuron " << getCompactIdVal( inpNeuron.GetId() ) << L" "
                << GetParameterName( tParameter::pulseRate ) 
                << L" = " << inpNeuron.GetPulseFrequency( )
                << endl; 
        }
    );

    out << endl;

    m_pModelWriterInterface->Apply2All<Neuron>
        ( 
            [&]( Neuron & neuron ) 
            { 
                if ( neuron.HasTriggerSound( ) )
                {
                    SoundDescr sound { neuron.GetTriggerSound() };
                    out << L"TriggerSound " << getCompactIdVal( neuron.GetId() ) << L" "
                        << sound.m_frequency << L" Hertz "
                        << sound.m_duration  << L" msec "
                        << endl; 
                }
            } 
    );

    MonitorData const * const pMonitorData { m_pModelWriterInterface->GetMonitorData() };

    pMonitorData->Apply2AllSignals
    ( 
        [&]( SignalId const idSignal )
        {
            Signal  const * pSignal = pMonitorData->GetSignal( idSignal );
            Signal  const & signal { * pSignal };
            ShapeId const idShape { signal.GetSignalSource() };
            // TODO
        }
    );

    timer.Stop();
    fMicroSecs const usTilStart { timer.GetMicroSecsTilStart( ) }; //for tests only

    setUnsavedChanges( false );  // no unsaved changes
}

void NNetModelStorage::writeDescription( wostream & out )
{
    wstring buffer;
    int iLineNr = 0;
    while ( m_pDescription->GetDescriptionLine( iLineNr++, buffer ) )
    {
        out << L"Description \"" << buffer << "\"" << endl;
    }
}

void NNetModelStorage::WritePipe( wostream & out, Shape const & shape )
{
    Pipe const & pipe { static_cast<Pipe const &>( shape ) };
    out << getCompactIdVal( pipe.GetStartKnotId() ) << L"->" << getCompactIdVal( pipe.GetEndKnotId() ) ;
}

void NNetModelStorage::WriteMicroMeterPoint( wostream & out, MicroMeterPoint const & pnt )
{
    out << pnt.GetXvalue() << L"|" << pnt.GetYvalue();
}

void NNetModelStorage::WriteShape( wostream & out, Shape & shape )
{
    if ( shape.IsDefined() )
    {
        out << L"CreateShape " << getCompactIdVal( shape.GetId() ) << L" " << shape.GetName();
        out << L" (";
        switch ( shape.GetShapeType( ).GetValue() )
        {
        case ShapeType::Value::inputNeuron:
        case ShapeType::Value::neuron:
        case ShapeType::Value::knot:
            WriteMicroMeterPoint( out, static_cast<BaseKnot &>( shape ).GetPosition() );
            break;

        case ShapeType::Value::pipe:
            WritePipe( out, shape );
            break;

        default:
            assert( false );
            break;
        }
        out << L")";
        out << endl;
    }
    if ( shape.IsMarked( ) )
    {
        out << L"MarkShape " << getCompactIdVal( shape.GetId() ) << endl;
    }
}

void NNetModelStorage::writeModel( )
{
    wofstream modelFile( m_wstrPathOfOpenModel );
    Write( modelFile );
    modelFile.close( );
    setUnsavedChanges( false );
}
