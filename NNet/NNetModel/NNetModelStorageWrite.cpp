// NNetModelStorageWrite.cpp
//
// NNetModel

#include "stdafx.h"
#include "scanner.h"
#include <assert.h>
#include "SignalInterface.h"
#include "Track.h"
#include "MonitorData.h"
#include "ModelDescription.h"
#include "NNetModelReaderInterface.h"
#include "NNetModelWriterInterface.h"
#include "win32_util.h"
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

    writeHeader( out );
    out << endl;
    writeDescription( out );
    out << endl;
    writeGlobalParameters( out );
    out << endl;
    writeShapes( out );
    out << endl;
    writeShapeParameters( out );
    out << endl;
    writeTriggerSounds( out );
    out << endl;
    writeMonitorData( out );
    out << endl;

    timer.Stop();
    fMicroSecs const usTilStart { timer.GetMicroSecsTilStart( ) }; //for tests only

    setUnsavedChanges( false );  // no unsaved changes
}

void NNetModelStorage::writeHeader( wostream & out )
{
    static int const BUF_SIZE { 128 };

    out << Scanner::COMMENT_SYMBOL << L" NNetModel" << endl;
    out << Scanner::COMMENT_SYMBOL << L" Created " << Util::GetCurrentDateAndTime() << endl;

    WCHAR  infoBuf[BUF_SIZE];
    DWORD  bufCharCount = BUF_SIZE;

    // Get and display the name of the computer.
    GetComputerName( infoBuf, &bufCharCount );
    out << Scanner::COMMENT_SYMBOL << L" Computer name: " << infoBuf << endl;

    // Get and display the user name.
    GetUserName( infoBuf, &bufCharCount );
    out << Scanner::COMMENT_SYMBOL << L" User name: " << infoBuf << endl; 
    out << endl;

    out << L"Protocol version " << PROTOCOL_VERSION << endl;
    out << endl;
}

void NNetModelStorage::writeGlobalParameters( wostream & out )
{
    Apply2GlobalParameters
    ( 
        [&]( tParameter const & par ) 
        {
            out << L"GlobalParameter " << GetParameterName( par ) << L" = "
                << m_pParam->GetParameterValue( par ) 
                << endl; 
        }
    );
}

void NNetModelStorage::writeShapes( wostream & out )
{
    m_CompactIds.resize( m_pModelReaderInterface->GetSizeOfShapeList() );
    wcout << L"*** Before writeShapes ";
    m_pModelReaderInterface->DumpModel();
    ShapeId idCompact( 0 );
    for ( int i = 0; i < m_CompactIds.size( ); ++i )
    {
        m_CompactIds[ i ] = m_pModelReaderInterface->GetConstShape( ShapeId( i ) )
            ? idCompact++
            : NO_SHAPE;
    }
    out << L"NrOfShapes = " << idCompact << endl;
    out << endl;
    m_pModelWriterInterface->Apply2All<BaseKnot>( [&]( BaseKnot & shape ) { writeShape( out, shape ); } );
    m_pModelWriterInterface->Apply2All<Pipe    >( [&]( Pipe     & shape ) { writeShape( out, shape ); } );
}

void NNetModelStorage::writeShapeParameters( wostream & out )
{
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
}

void NNetModelStorage::writeTriggerSounds( wostream & out )
{
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
}

void NNetModelStorage::writeMonitorData( wostream & out )
{
    MonitorData const * const pMonitorData { m_pModelWriterInterface->GetMonitorData() };

    out << L"NrOfTracks " << pMonitorData->GetNrOfTracks() << endl;

    pMonitorData->Apply2AllSignals
    ( 
        [&]( SignalId const idSignal )
        {
            SignalInterface const & signal { pMonitorData->GetSignal( idSignal ) };
            out << L"SignalInterface track " << idSignal.GetTrackNr() << L" source ";
            signal.WriteSignalData( out );
            out << endl; 
        }
    );
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

void NNetModelStorage::writePipe( wostream & out, Pipe const & pipe )
{
    out << Pipe::OPEN_BRACKET 
        << getCompactIdVal( pipe.GetStartKnotId() ) 
        << Pipe::SEPARATOR
        << getCompactIdVal( pipe.GetEndKnotId() ) 
        << Pipe::CLOSE_BRACKET;
}

void NNetModelStorage::writeShape( wostream & out, Shape & shape )
{
    if ( shape.IsDefined() )
    {
        out << L"CreateShape " << getCompactIdVal( shape.GetId() ) << L" " << shape.GetName();
        switch ( shape.GetShapeType( ).GetValue() )
        {
        case ShapeType::Value::inputNeuron:
        case ShapeType::Value::neuron:
        case ShapeType::Value::knot:
            out << static_cast<BaseKnot const &>( shape ).GetPosition();
            break;

        case ShapeType::Value::pipe:
            writePipe( out, static_cast<Pipe const &>( shape ) );
            break;

        default:
            assert( false );
            break;
        }
        out << endl;
    }
}

void NNetModelStorage::writeModel( )
{
    wofstream modelFile( m_wstrPathOfOpenModel );
    Write( modelFile );
    modelFile.close( );
    setUnsavedChanges( false );
}
