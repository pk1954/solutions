// NNetModelExport.cpp
//
// NNetModel

#include "stdafx.h"
#include "scanner.h"
#include <assert.h>
#include <filesystem>
#include "Signal.h"
#include "Track.h"
#include "MonitorData.h"
#include "ModelDescription.h"
#include "NNetModelReaderInterface.h"
#include "NNetModelWriterInterface.h"
#include "win32_util.h"
#include "win32_script.h"
#include "win32_HiResTimer.h"
#include "NNetModelStorage.h"
#include "NNetModelExport.h"

using std::wcout;
using std::endl;
using std::put_time;
using std::wofstream;
using std::filesystem::path;

void NNetModelExport::Initialize( NNetModelReaderInterface * const pNMRI )
{
    m_pMRI = pNMRI;
}

void NNetModelExport::writeHeader( wostream & out )
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

    out << L"Protocol version " << NNetModelStorage::PROTOCOL_VERSION << endl;
    out << endl;
}

void NNetModelExport::writeGlobalParameters( wostream & out )
{
    ParameterType::Apply2GlobalParameters
    ( 
        [&]( ParameterType::Value const & par ) 
        {
            out << L"GlobalParameter " << ParameterType::GetName( par ) << L" = "
                << m_pMRI->GetParameter( par ) 
                << endl; 
        }
    );
}

void NNetModelExport::writeShapes( wostream & out )
{
    m_CompactIds.resize( m_pMRI->GetSizeOfShapeList() );
    wcout << L"*** Before writeShapes ";
    m_pMRI->DumpModel();
    ShapeId idCompact( 0 );
    for ( int i = 0; i < m_CompactIds.size( ); ++i )
    {
        m_CompactIds[ i ] = m_pMRI->GetConstShape( ShapeId( i ) )
            ? idCompact++
            : ShapeId();
    }
    out << L"NrOfShapes = " << idCompact << endl;
    out << endl;
    m_pMRI->Apply2All<BaseKnot>( [&]( BaseKnot const & shape ) { writeShape( out, shape ); } );
    m_pMRI->Apply2All<Pipe    >( [&]( Pipe     const & shape ) { writeShape( out, shape ); } );
}

void NNetModelExport::writeShapeParameters( wostream & out )
{
    m_pMRI->Apply2All<InputNeuron>
    (
        [&]( InputNeuron const & inpNeuron )
        { 
            out << L"ShapeParameter InputNeuron " << getCompactIdVal( inpNeuron.GetId() ) << L" "
                << ParameterType::GetName( ParameterType::Value::pulseRate ) 
                << L" = " << inpNeuron.GetPulseFrequency( )
                << endl; 
        }
    );
}

void NNetModelExport::writeTriggerSounds( wostream & out )
{
    m_pMRI->Apply2All<Neuron>
    ( 
        [&]( Neuron const & neuron ) 
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

void NNetModelExport::writeMonitorData( wostream & out )
{
    MonitorData const & monitorData { m_pMRI->GetMonitorData() };

    out << L"NrOfTracks " << monitorData.GetNrOfTracks() << endl;

    monitorData.Apply2AllSignals
    ( 
        [&]( SignalId const idSignal )
        {
            out << L"Signal "; 
            WriteTrackNr( out, idSignal.GetTrackNr() );
            out << L"source " << NNetModelStorage::SIGSRC_CIRCLE; 
            out << monitorData.GetSignal( idSignal ).GetCircle( );
            out << endl; 
        }
    );
}

void NNetModelExport::writeDescription( wostream & out )
{
    wstring wstrLine;
    int iLineNr = 0;
    while ( m_pDescription->GetDescriptionLine( iLineNr++, wstrLine ) )
    {
        out << L"Description \"" << wstrLine << "\"" << endl;
    }
}

void NNetModelExport::writePipe( wostream & out, Pipe const & pipe )
{
    out << Pipe::OPEN_BRACKET 
        << getCompactIdVal( pipe.GetStartKnotId() ) 
        << Pipe::SEPARATOR
        << getCompactIdVal( pipe.GetEndKnotId() ) 
        << Pipe::CLOSE_BRACKET;
}

void NNetModelExport::writeShape( wostream & out, Shape const & shape )
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

void NNetModelExport::write( wostream & out )
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
}

void NNetModelExport::WriteModel( )
{
    wofstream modelFile( m_pMRI->GetModelFilePath() );
    write( modelFile );
    modelFile.close( );
}
