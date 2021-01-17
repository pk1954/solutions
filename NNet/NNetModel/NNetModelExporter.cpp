// NNetModelExporter.cpp
//
// NNetModel

#include "stdafx.h"
#include "scanner.h"
#include <assert.h>
#include <filesystem>
#include "Signal.h"
#include "Track.h"
#include "InputNeuron.h"
#include "BaseKnot.h"
#include "MonitorData.h"
#include "ModelDescription.h"
#include "NNetModelReaderInterface.h"
#include "NNetModelWriterInterface.h"
#include "win32_util.h"
#include "win32_script.h"
#include "win32_HiResTimer.h"
#include "NNetModelStorage.h"
#include "NNetModelExporter.h"

using std::wcout;
using std::endl;
using std::put_time;
using std::wofstream;
using std::filesystem::path;

void NNetModelExporter::Initialize( NNetModelReaderInterface * const pNMRI )
{
    m_pNMRI = pNMRI;
}

void NNetModelExporter::writeHeader( wostream & out )
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

void NNetModelExporter::writeGlobalParameters( wostream & out )
{
    ParameterType::Apply2GlobalParameters
    ( 
        [&]( ParameterType::Value const & par ) 
        {
            out << L"GlobalParameter " << ParameterType::GetName( par ) << L" = "
                << m_pNMRI->GetParameter( par ) 
                << endl; 
        }
    );
}

void NNetModelExporter::writeShapes( wostream & out )
{
    m_CompactIds.resize( m_pNMRI->GetSizeOfShapeList() );
    //wcout << L"*** Before writeShapes ";
    //m_pNMRI->DumpModel();
    ShapeId idCompact( 0 );
    for ( int i = 0; i < m_CompactIds.size( ); ++i )
    {
        m_CompactIds[ i ] = m_pNMRI->GetConstShape( ShapeId( i ) )
            ? idCompact++
            : ShapeId();
    }
    out << L"NrOfShapes = " << idCompact << endl;
    out << endl;
    m_pNMRI->GetShapes().Apply2All<BaseKnot>( [&]( BaseKnot const & shape ) { writeShape( out, shape ); } );
    m_pNMRI->GetShapes().Apply2All<Pipe    >( [&]( Pipe     const & shape ) { writeShape( out, shape ); } );
}

void NNetModelExporter::writeShapeParameters( wostream & out )
{
    m_pNMRI->GetShapes().Apply2All<InputNeuron>
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

void NNetModelExporter::writeTriggerSounds( wostream & out )
{
    m_pNMRI->GetShapes().Apply2All<Neuron>
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

void NNetModelExporter::writeMonitorData( wostream & out )
{
    MonitorData const & monitorData { m_pNMRI->GetMonitorData() };

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

void NNetModelExporter::writeDescription( wostream & out )
{
    wstring wstrLine;
    int iLineNr = 0;
    while ( m_pNMRI->GetDescription().GetDescriptionLine( iLineNr++, wstrLine ) )
    {
        out << L"Description \"" << wstrLine << "\"" << endl;
    }
}

void NNetModelExporter::writePipe( wostream & out, Pipe const & pipe )
{
    out << Pipe::OPEN_BRACKET 
        << getCompactIdVal( pipe.GetStartKnotId() ) 
        << Pipe::SEPARATOR
        << getCompactIdVal( pipe.GetEndKnotId() ) 
        << Pipe::CLOSE_BRACKET;
}

void NNetModelExporter::writeShape( wostream & out, Shape const & shape )
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

void NNetModelExporter::write( wostream & out )
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

void NNetModelExporter::WriteModel( )
{
    wofstream modelFile( m_pNMRI->GetModelFilePath() );
    write( modelFile );
    modelFile.close( );
}
