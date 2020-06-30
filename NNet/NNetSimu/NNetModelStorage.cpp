// NNetModelStorage.cpp
//
// NNetSimu

#include "stdafx.h"
#include <iostream>
#include <iomanip>
#include <ctime>
#include <sstream>
#include <filesystem>
#include "assert.h"
#include "SCRIPT.H"
#include "symtab.h"
#include "errhndl.h"
#include "Resource.h"
#include "MoreTypes.h"
#include "NNetModel.h"
#include "NNetParameters.h"
#include "Preferences.h"
#include "InputNeuron.h"
#include "win32_script.h"
#include "win32_thread.h"
#include "win32_HiResTimer.h"
#include "win32_NNetAppMenu.h"
#include "NNetModelStorage.h"

using std::wcout;
using std::endl;
using std::put_time;
using std::wofstream;
using std::filesystem::exists;

static float const PROTOCOL_VERSION { 1.5f };   // pipeline renamed to pipe

void NNetModelStorage::Initialize
( 
    HWND        const hwndApp,  
    NNetModel * const pModel, 
    Param     * const pParam,
    Script    * const pScript
)
{
    m_hwndApp = hwndApp;
    m_pModel  = pModel;
    m_pParam  = pParam;
    m_pScript = pScript;
}

void NNetModelStorage::setUnsavedChanges( bool const bState )
{
    m_bUnsavedChanges = bState;
    NNetAppMenu::SetAppTitle( m_wstrPathOfOpenModel, bState );
}

////////////////////////// Read /////////////////////////////////////////////

class WrapProtocol : public Script_Functor
{
public:
    WrapProtocol( NNetModel * const pNNetModel ) :
        m_pModel( pNNetModel )
    { };

    virtual void operator() ( Script & script ) const 
    {
        script.ScrReadString( L"version" );
        double dVersion = script.ScrReadFloat();
    }

private:
    NNetModel * m_pModel;
};

class WrapMarkShape : public Script_Functor
{
public:
    WrapMarkShape( NNetModel * const pNNetModel ) :
        m_pModel( pNNetModel )
    { };

    virtual void operator() ( Script & script ) const
    {   
        ShapeId const idFromScript{ script.ScrReadLong() };
        m_pModel->MarkShape( idFromScript, tBoolOp::opTrue );
    }

private:

    NNetModel * m_pModel;
};

class WrapCreateShape : public Script_Functor
{
public:
    WrapCreateShape( NNetModel * const pNNetModel ) :
        m_pModel( pNNetModel )
    { };

    virtual void operator() ( Script & script ) const
    {   
        ShapeId   const idFromScript{ script.ScrReadLong() };
        ShapeType const shapeType   { static_cast<ShapeType::Value>(script.ScrReadInt( )) };
        Shape         * pShape      { nullptr };

        script.ScrReadSpecial( L'(' );
        if ( shapeType.IsPipeType() )
        {
            ShapeId const idStart { script.ScrReadLong() };
            script.ScrReadSpecial( L'-' );
            script.ScrReadSpecial( L'>' );
            ShapeId const idEnd { script.ScrReadLong() };
            if ( idStart == idEnd )
            {
                wcout << "+++ Pipe has identical start and end point" << endl;
                wcout << "+++ " << idFromScript << L": " << idStart << L" -> " << idEnd << endl;
                wcout << "+++ Pipe ignored" << endl;
            }
            else
            { 
                Pipe * const pPipe { new Pipe( ) };
                pPipe->SetId( idFromScript );
                m_pModel->ConnectOutgoing( pPipe, m_pModel->GetShapePtr<BaseKnot *>( idStart ) );
                m_pModel->ConnectIncoming( pPipe, m_pModel->GetShapePtr<BaseKnot *>( idEnd   ) );
                pPipe->Recalc();
                pShape = pPipe;
            }
        }
        else 
        {
            MicroMeter      const xCoord { CastToFloat( script.ScrReadFloat() ) };
            script.ScrReadSpecial( L'|' );
            MicroMeter      const yCoord { CastToFloat( script.ScrReadFloat() ) };
            MicroMeterPoint const umPosition( xCoord, yCoord );
            switch ( shapeType.GetValue() )
            {
            case ShapeType::Value::inputNeuron:
                pShape = new InputNeuron( umPosition );
                break;

            case ShapeType::Value::neuron:
                pShape = new Neuron( umPosition );
                break;

            case ShapeType::Value::knot:
                pShape = new Knot( umPosition );
                break;

            default:
                assert( false );
                break;
            }
        }
        script.ScrReadSpecial( L')' );

        if ( pShape )
        {
            m_pModel->SetShape( pShape, idFromScript );
            pShape->SetId( idFromScript );
        }
    }

private:

    NNetModel * m_pModel;
};

class WrapGlobalParameter : public Script_Functor
{
public:
    WrapGlobalParameter( NNetModel * const pNNetModel ) :
        m_pModel( pNNetModel )
    { };

    virtual void operator() ( Script & script ) const 
    {
        tParameter const param( static_cast< tParameter >( script.ScrReadUint() ) );
        script.ScrReadSpecial( L'=' );
        float const fValue { CastToFloat( script.ScrReadFloat() ) };
        m_pModel->SetParameter( param, fValue );
    }

private:
    NNetModel * m_pModel;
};

class WrapNrOfShapes : public Script_Functor
{
public:
    WrapNrOfShapes( NNetModel * const pNNetModel ) :
        m_pModel( pNNetModel )
    { };

    virtual void operator() ( Script & script ) const 
    {
        script.ScrReadSpecial( L'=' );
        m_pModel->SetNrOfShapes( script.ScrReadLong() );
    }

private:
    NNetModel * m_pModel;
};

class WrapShapeParameter : public Script_Functor
{
public:
    WrapShapeParameter( NNetModel * const pNNetModel ) :
        m_pModel( pNNetModel )
    { };

    virtual void operator() ( Script & script ) const 
    {
        script.ScrReadString( L"InputNeuron" );
        ShapeId const id ( script.ScrReadUint() );
        tParameter const param( static_cast< tParameter >( script.ScrReadUint() ) );
        assert( param == tParameter::pulseRate );
        script.ScrReadSpecial( L'=' );
        float const fValue { CastToFloat( script.ScrReadFloat() ) };
        m_pModel->SetPulseRate( id, fValue );
    }

private:
    NNetModel * m_pModel;
};

class WrapTriggerSound : public Script_Functor
{
public:
    WrapTriggerSound( NNetModel * const pNNetModel ) :
        m_pModel( pNNetModel )
    { };

    virtual void operator() ( Script & script ) const 
    {
        ShapeId const id      { script.ScrReadLong () };
        Neuron      * pNeuron { m_pModel->GetShapePtr<Neuron *>( id ) };
        Hertz   const freq    { script.ScrReadUlong() };
        script.ScrReadString( L"Hertz" );
        MilliSecs const msec { script.ScrReadUlong() };
        script.ScrReadString( L"msec" );
        pNeuron->SetTriggerSoundOn( true );
        pNeuron->SetTriggerSoundFrequency( freq );
        pNeuron->SetTriggerSoundDuration ( msec );
    }

private:
    NNetModel * m_pModel;
};

void NNetModelStorage::prepareForReading( )
{
#define DEF_NNET_FUNC(name) SymbolTable::ScrDefConst( L#name, new Wrap##name##( m_pModel ) )
    DEF_NNET_FUNC( Protocol );
    DEF_NNET_FUNC( GlobalParameter );
    DEF_NNET_FUNC( ShapeParameter );
    DEF_NNET_FUNC( NrOfShapes );
    DEF_NNET_FUNC( CreateShape );
    DEF_NNET_FUNC( MarkShape );
    DEF_NNET_FUNC( TriggerSound );
#undef DEF_NET_FUNC

    ShapeType::Apply2All
    ( 
        [&]( ShapeType const & type ) 
        {
            SymbolTable::ScrDefConst
            ( 
                ShapeType::GetName( type.GetValue() ), 
                static_cast<unsigned long>(type.GetValue()) 
            );
        }
    );

    ///// Legacy /////
    SymbolTable::ScrDefConst( L"pipeline", static_cast<unsigned long>(ShapeType::Value::pipe) );  // support older protocol version
    ///// end Legacy /////

    Apply2AllParameters
    ( 
        [&]( tParameter const & param ) 
        {
            SymbolTable::ScrDefConst
            ( 
                GetParameterName( param ), 
                static_cast<unsigned long>( param ) 
            );
        }
    );

#undef DEF_NNET_FUNC

    m_bPreparedForReading = true;
}

void NNetModelStorage::readModel( ) 
{
    bool bSuccess { m_pScript->ScrProcess( m_wstrPathOfNewModel ) };
    if ( bSuccess )
    {
        m_wstrPathOfOpenModel = m_wstrPathOfNewModel;
        m_pModel->StaticModelChanged();
        setUnsavedChanges( false );
        SendMessage( m_hwndApp, WM_COMMAND, IDM_READ_MODEL_FINISHED, static_cast<LPARAM>( bSuccess ) );
    }
    else
    {
        MessageBox( nullptr, m_wstrPathOfNewModel.c_str(), L"Error in model file. Using default model.", MB_OK );
        SendMessage( m_hwndApp, WM_COMMAND, IDM_NEW_MODEL, 0 );
    }
}

static unsigned int __stdcall readModelThreadProc( void * data ) 
{
    SetThreadDescription( GetCurrentThread(), L"ReadModel" );
    reinterpret_cast<NNetModelStorage *>( data )->readModel( );
    return 0;
}

void NNetModelStorage::Read( bool bConcurrently, wstring const wstrPath )
{
    if ( ! m_bPreparedForReading )
        prepareForReading( );

    m_wstrPathOfNewModel = ( wstrPath == L"" ) ? m_wstrPathOfOpenModel : wstrPath;
    if ( exists( m_wstrPathOfNewModel ) )
    {
        m_pModel->ResetModel();
        wcout << L"** NNet model file " << m_wstrPathOfNewModel << endl;
        if ( bConcurrently )
        {
            UINT threadId { 0 };
            Util::RunAsAsyncThread( readModelThreadProc, static_cast<void *>(this), & threadId );
        }
        else
        {
            readModel( );
        }
    }
    else
    {
        MessageBox( nullptr, m_wstrPathOfNewModel.c_str(), L"Could not find model file", MB_OK );
        PostMessage( m_hwndApp, WM_COMMAND, IDM_NEW_MODEL, 0 );
    }
}

void NNetModelStorage::ReadAsync( wstring const wstrPath )
{
    Read( true, wstrPath );
}

////////////////////////// Write /////////////////////////////////////////////

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

    Apply2GlobalParameters
    ( 
        [&]( tParameter const & par ) 
        {
            out << L"GlobalParameter " << GetParameterName( par ) << L" = "
            << m_pParam->GetParameterValue( par ) 
            << endl; 
        }
    );

    m_CompactIds.resize( m_pModel->GetSizeOfShapeList() );
    ShapeId idCompact( 0 );
    for ( int i = 0; i < m_CompactIds.size( ); ++i )
    {
        m_CompactIds[ i ] = m_pModel->GetConstShape( ShapeId( i ) )
                          ? idCompact++
                          : NO_SHAPE;
    }

    out << endl;
    out << L"NrOfShapes = " << idCompact << endl;
    out << endl;

    m_pModel->Apply2All<BaseKnot>( [&]( BaseKnot & shape ) { WriteShape( out, shape ); } );
    m_pModel->Apply2All<Pipe    >( [&]( Pipe     & shape ) { WriteShape( out, shape ); } );

    out << endl;

    m_pModel->Apply2All<InputNeuron>
    (
        [&]( InputNeuron & inpNeuron )
        { 
            out << L"ShapeParameter InputNeuron " << getCompactIdVal( inpNeuron.GetId() ) << L" "
                << GetParameterName( tParameter::pulseRate ) 
                << L" = " << inpNeuron.GetPulseFreq( )
                << endl; 
        }
    );

    out << endl;

    m_pModel->Apply2All<Neuron>
    ( 
        [&]( Neuron & neuron ) 
        { 
            if ( neuron.HasTriggerSound( ) )
            {
                out << L"TriggerSound " << getCompactIdVal( neuron.GetId() ) << L" "
                    << neuron.GetTriggerSoundFrequency() << L" Hertz "
                    << neuron.GetTriggerSoundDuration()  << L" msec "
                    << endl; 
            }
        } 
    );

    timer.Stop();
    fMicroSecs const usTilStart { timer.GetMicroSecsTilStart( ) }; //for tests only

    setUnsavedChanges( false );  // no unsaved changes
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

/////////////////////////////////////////////

bool NNetModelStorage::AskAndSave( )
{
    if ( m_bUnsavedChanges )
    {
        int iRes = MessageBox( nullptr, L"Save now?", L"Unsaved changes", MB_YESNOCANCEL );
        if ( iRes == IDYES )
            SaveModel( );
        else if ( iRes == IDNO )
            return true;
        else if ( iRes == IDCANCEL )
            return false;
    }
    return true;
}

bool NNetModelStorage::AskModelFile( )
{
    wstring wstrPath = AskForFileName( GetPathOfExecutable( ), L"*.mod", L"Model files", tFileMode::read );
    if ( wstrPath != L"" )
    {
        m_wstrPathOfOpenModel = wstrPath;
        return true;
    }
    return false;
}

void NNetModelStorage::writeModel( )
{
    wofstream modelFile( m_wstrPathOfOpenModel );
    Write( modelFile );
    modelFile.close( );
    setUnsavedChanges( false );
}

bool NNetModelStorage::SaveModelAs( )
{
    if ( m_wstrPathOfOpenModel == L"" )
    {
        m_wstrPathOfOpenModel = std::filesystem::path( GetPathOfExecutable( ) ).parent_path();
        m_wstrPathOfOpenModel += L"\\std.mod";
        writeModel();
        return true;
    }
    else
    {
        m_wstrPathOfOpenModel = AskForFileName( m_wstrPathOfOpenModel, L"*.mod", L"Model files", tFileMode::write );
        bool const bRes = m_wstrPathOfOpenModel != L"";
        if ( bRes )
            writeModel( );
        return bRes;
    }
}

bool NNetModelStorage::SaveModel( )
{
    if ( m_wstrPathOfOpenModel == L"" )
    {
        return SaveModelAs( );
    }
    else
    {
        writeModel( );
        return true;
    }
}

void NNetModelStorage::ResetModelPath( ) 
{ 
    m_wstrPathOfOpenModel = L""; 
    setUnsavedChanges( true );
}
