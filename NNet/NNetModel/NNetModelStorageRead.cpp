// NNetModelStorageRead.cpp
//
// NNetModel

#include "stdafx.h"
#include <filesystem>
#include <assert.h>
#include "ModelDescription.h"
#include "SignalFactory.h"
#include "MonitorData.h"
#include "NNetError.h"
#include "NNetWrapperHelpers.h"
#include "NNetParameters.h"
#include "InputNeuron.h"
#include "win32_script.h"
#include "win32_thread.h"
#include "NNetModelStorage.h"

using std::filesystem::exists;

class WrapProtocol : public Script_Functor
{
public:
    virtual void operator() ( Script & script ) const 
    {
        script.ScrReadString( L"version" );
        double dVersion = script.ScrReadFloat();
    }
};

class WrapDescription : public Script_Functor
{
public:
    WrapDescription( wstring * const pwstrDescription ) :
        m_pwstrDescription( pwstrDescription )
    { };

    virtual void operator() ( Script & script ) const 
    {
        wstring const wstrDescription { script.ScrReadString( ) };
        * m_pwstrDescription += wstrDescription + L"\r\n";
    }

private:
    wstring * m_pwstrDescription { nullptr };
};

class WrapCreateShape : public Script_Functor
{
public:
    WrapCreateShape( NNetModelWriterInterface * const pNNetModel ) :
        m_pMWI( pNNetModel )
    { };

    virtual void operator() ( Script & script ) const
    {   
        ShapeId   const idFromScript{ script.ScrReadLong() };
        ShapeType const shapeType   { static_cast<ShapeType::Value>(script.ScrReadInt( )) };
        UPShape         upShape     { 
                                       shapeType.IsPipeType()
                                       ? createPipe( script )
                                       : createBaseKnot( script, shapeType ) 
                                    };
        if ( upShape )
        {
            upShape.get()->SetId( idFromScript );
            m_pMWI->SetInModel( idFromScript, move(upShape) );
        }
    }

private:

    UPShape createPipe( Script & script ) const
    {
        script.ScrReadSpecial( Pipe::OPEN_BRACKET );
        ShapeId const idStart { script.ScrReadLong() };
        for ( int i = 0; i < Pipe::SEPARATOR.length( ); i++ )
            script.ScrReadSpecial( Pipe::SEPARATOR[i] );
        ShapeId const idEnd { script.ScrReadLong() };
        script.ScrReadSpecial( Pipe::CLOSE_BRACKET );
        if ( idStart == idEnd )
        {
            wcout << "+++ Pipe has identical start and end point" << endl;
            wcout << "+++ " << L": " << idStart << L" -> " << idEnd << endl;
            wcout << "+++ Pipe ignored" << endl;
            return nullptr;
        }
        else
        { 
            BaseKnot * const pKnotStart { m_pMWI->GetShapePtr<BaseKnot *>( idStart ) };
            BaseKnot * const pKnotEnd   { m_pMWI->GetShapePtr<BaseKnot *>( idEnd   ) };
            unique_ptr<Pipe> upPipe { make_unique<Pipe>( pKnotStart, pKnotEnd ) };
            pKnotStart->m_connections.AddOutgoing( upPipe.get() );
            pKnotEnd  ->m_connections.AddIncoming( upPipe.get() );
            return move(upPipe);
        }
    }

    UPShape createBaseKnot( Script & script, ShapeType const shapeType ) const 
    {
        MicroMeterPoint const umPosition(ScrReadMicroMeterPoint( script ) );
        switch ( shapeType.GetValue() )
        {
        case ShapeType::Value::inputNeuron:
            return make_unique<InputNeuron>( umPosition );

        case ShapeType::Value::neuron:
            return make_unique<Neuron>( umPosition );

        case ShapeType::Value::knot:
            return make_unique<Knot>( umPosition );

        default:
            assert( false );
            return nullptr;
        }
    }

    NNetModelWriterInterface * m_pMWI;
};

class WrapGlobalParameter : public Script_Functor
{
public:
    WrapGlobalParameter( NNetModelWriterInterface * const pNNetModel ) :
        m_pMWI( pNNetModel )
    { };

    virtual void operator() ( Script & script ) const 
    {
        tParameter const param( static_cast< tParameter >( script.ScrReadUint() ) );
        script.ScrReadSpecial( L'=' );
        float const fValue { Cast2Float( script.ScrReadFloat() ) };
        m_pMWI->SetParam( param, fValue );
    }

private:
    NNetModelWriterInterface * m_pMWI;
};

class WrapNrOfShapes : public Script_Functor
{
public:
    WrapNrOfShapes( NNetModelWriterInterface * const pNNetModel ) :
        m_pMWI( pNNetModel )
    { };

    virtual void operator() ( Script & script ) const 
    {
        script.ScrReadSpecial( L'=' );
        m_pMWI->IncShapeList( script.ScrReadLong() );
    }

private:
    NNetModelWriterInterface * m_pMWI;
};

class WrapShapeParameter : public Script_Functor
{
public:
    WrapShapeParameter( NNetModelWriterInterface * const pModel ) :
        m_pMWI( pModel )
    { };

    virtual void operator() ( Script & script ) const 
    {
        script.ScrReadString( L"InputNeuron" );
        ShapeId const id ( script.ScrReadUint() );
        tParameter const param( static_cast< tParameter >( script.ScrReadUint() ) );
        assert( param == tParameter::pulseRate );
        script.ScrReadSpecial( L'=' );
        float const fValue { Cast2Float( script.ScrReadFloat() ) };
        m_pMWI->GetShapePtr<InputNeuron *>( id )->SetPulseFrequency( fHertz( fValue ) );
    }

private:
    NNetModelWriterInterface * m_pMWI;
};

class WrapTriggerSound : public Script_Functor
{
public:
    WrapTriggerSound( NNetModelWriterInterface * const pNNetModel ) :
        m_pMWI( pNNetModel )
    { };

    virtual void operator() ( Script & script ) const 
    {
        ShapeId const id      { script.ScrReadLong () };
        Neuron      * pNeuron { m_pMWI->GetShapePtr<Neuron *>( id ) };
        Hertz   const freq    { script.ScrReadUlong() };
        script.ScrReadString( L"Hertz" );
        MilliSecs const msec { script.ScrReadUlong() };
        script.ScrReadString( L"msec" );
        SoundDescr oldValue { pNeuron->SetTriggerSound( SoundDescr{ true, freq, msec } ) };
    }

private:
    NNetModelWriterInterface * m_pMWI;
};

class WrapNrOfTracks : public Script_Functor
{
public:
    WrapNrOfTracks( NNetModelWriterInterface * const pNNetModel ) :
        m_pMWI( pNNetModel )
    { };

    virtual void operator() ( Script & script ) const 
    {
        unsigned int  const uiNrOfTracks { script.ScrReadUint() };
        MonitorData * const pMonitorData { m_pMWI->GetMonitorData() };
        for ( unsigned int ui = 0; ui < uiNrOfTracks; ++ui )
            pMonitorData->InsertTrack( TrackNr(0) );
    }
private:
    NNetModelWriterInterface * m_pMWI;
};

class WrapSignal : public Script_Functor
{
public:
    WrapSignal( NNetModelWriterInterface * const pNNetModel ) :
        m_pMWI( pNNetModel )
    { };

    virtual void operator() ( Script & script ) const 
    {
        MicroMeterCircle const umCircle { ScrReadMicroMeterCircle( script ) };
        TrackNr          const trackNr  { ScrReadTrackNr(script) };
        m_pMWI->GetMonitorData()->AddSignal( trackNr, umCircle );
    }

private:
    NNetModelWriterInterface * m_pMWI;
};

void NNetModelStorage::prepareForReading( )
{
    SymbolTable::ScrDefConst( L"Description", new WrapDescription( & m_wstrDescription ) );
    SymbolTable::ScrDefConst( L"Protocol"   , new WrapProtocol(  ) );

#define DEF_NNET_FUNC(name) SymbolTable::ScrDefConst( L#name, new Wrap##name##( m_pMWI ) )
    DEF_NNET_FUNC( GlobalParameter );
    DEF_NNET_FUNC( NrOfShapes );
    DEF_NNET_FUNC( CreateShape );
    DEF_NNET_FUNC( ShapeParameter );
    DEF_NNET_FUNC( TriggerSound );
    DEF_NNET_FUNC( NrOfTracks );
    DEF_NNET_FUNC( Signal );
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

bool NNetModelStorage::readModel( ) 
{
    if ( ProcessNNetScript( m_pScript, m_pMWI, m_wstrPathOfNewModel ) )
    {
        m_pMWI->RemoveOrphans();
        m_pDescription->SetDescription( m_wstrDescription );
        m_wstrPathOfOpenModel = m_wstrPathOfNewModel;
        m_pMWI->StaticModelChanged();
        setUnsavedChanges( false );
        m_pResult->Reaction( ReadModelResult::tResult::ok );
        return true;
    }
    else
    {
        m_pResult->Reaction( ReadModelResult::tResult::errorInFile );
        return false;
    }
}

static unsigned int __stdcall readModelThreadProc( void * data ) 
{
    SetThreadDescription( GetCurrentThread(), L"ReadModel" );
    reinterpret_cast<NNetModelStorage *>( data )->readModel( );
    return 0;
}

bool NNetModelStorage::Read( bool bConcurrently, wstring const wstrPath )
{
    bool bRes { true };
    if ( ! m_bPreparedForReading )
        prepareForReading( );

    m_wstrPathOfNewModel = ( wstrPath == L"" ) ? m_wstrPathOfOpenModel : wstrPath;
    if ( exists( m_wstrPathOfNewModel ) )
    {
        m_pMWI->ResetModel();
        m_wstrDescription.clear();
        wcout << L"*** Reading file " << m_wstrPathOfNewModel << endl;
        if ( bConcurrently )
        {
            UINT threadId { 0 };
            Util::RunAsAsyncThread( readModelThreadProc, static_cast<void *>(this), & threadId );
        }
        else
        {
            bRes = readModel( );
        }
    }
    else
    {
        m_pResult->Reaction( ReadModelResult::tResult::fileNotFound, wstrPath );
        bRes = false;
    }
    return bRes;
}

void NNetModelStorage::ReadAsync( wstring const wstrPath )
{
    Read( true, wstrPath );
}
