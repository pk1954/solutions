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
    WrapProtocol( NNetModelStorage * const pNNetModelStorage ) { };

    virtual void operator() ( Script & script ) const 
    {
        script.ScrReadString( L"version" );
        double dVersion = script.ScrReadFloat();
    }
};

class WrapDescription : public Script_Functor
{
public:
    WrapDescription( NNetModelStorage * const pNNetModelStorage ) :
        m_pNMS( pNNetModelStorage )
    { };

    virtual void operator() ( Script & script ) const 
    {
        wstring const wstrDescription { script.ScrReadString( ) };
        if ( ! m_pNMS->AddMode() )
            m_pNMS->AddDescription( wstrDescription );
    }

private:
    NNetModelStorage * m_pNMS;
};

class WrapCreateShape : public Script_Functor
{
public:
    WrapCreateShape( NNetModelStorage * const pNNetModelStorage ) :
        m_pNMS( pNNetModelStorage )
    { };

    virtual void operator() ( Script & script ) const
    {   
        ShapeId   const idFromScript{ m_pNMS->ScrReadShapeId(script) };
        ShapeType const shapeType   { static_cast<ShapeType::Value>(script.ScrReadInt( )) };
        UPShape         upShape     { 
                                       shapeType.IsPipeType()
                                       ? createPipe( script )
                                       : createBaseKnot( script, shapeType ) 
                                    };
        if ( upShape )
        {
            Shape * pShape { upShape.get() };
            pShape->SetId( idFromScript );
            if ( m_pNMS->AddMode() )
                pShape->Select( tBoolOp::opTrue );
            m_pNMS->GetWriterInterface().SetInModel( idFromScript, move(upShape) );
        }
    }

private:

    UPShape createPipe( Script & script ) const
    {
        script.ScrReadSpecial( Pipe::OPEN_BRACKET );
        ShapeId const idStart { m_pNMS->ScrReadShapeId(script) };
        for ( int i = 0; i < Pipe::SEPARATOR.length( ); i++ )
            script.ScrReadSpecial( Pipe::SEPARATOR[i] );
        ShapeId const idEnd { m_pNMS->ScrReadShapeId(script) };
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
            BaseKnot * const pKnotStart { m_pNMS->GetWriterInterface().GetShapePtr<BaseKnot *>( idStart ) };
            BaseKnot * const pKnotEnd   { m_pNMS->GetWriterInterface().GetShapePtr<BaseKnot *>( idEnd   ) };
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

    NNetModelStorage * m_pNMS;
};

class WrapGlobalParameter : public Script_Functor
{
public:
    WrapGlobalParameter( NNetModelStorage * const pNNetModelStorage ) :
        m_pNMS( pNNetModelStorage )
    { };

    virtual void operator() ( Script & script ) const 
    {
        tParameter const param( static_cast< tParameter >( script.ScrReadUint() ) );
        script.ScrReadSpecial( L'=' );
        float const fValue { Cast2Float( script.ScrReadFloat() ) };
        if ( ! m_pNMS->AddMode() )
            m_pNMS->GetWriterInterface().SetParam( param, fValue );
    }

private:
    NNetModelStorage * m_pNMS;
};

class WrapNrOfShapes : public Script_Functor
{
public:
    WrapNrOfShapes(NNetModelStorage * const pNNetModelStorage ) :
        m_pNMS( pNNetModelStorage )
    { };

    virtual void operator() ( Script & script ) const 
    {
        script.ScrReadSpecial( L'=' );
        m_pNMS->GetWriterInterface().IncShapeList( script.ScrReadLong() );
    }

private:
    NNetModelStorage * m_pNMS;
};

class WrapShapeParameter : public Script_Functor
{
public:
    WrapShapeParameter( NNetModelStorage * const pNNetModelStorage ) :
        m_pNMS( pNNetModelStorage )
    { };

    virtual void operator() ( Script & script ) const 
    {
        script.ScrReadString( L"InputNeuron" );
        ShapeId    const id   ( m_pNMS->ScrReadShapeId(script) );
        tParameter const param( static_cast< tParameter >( script.ScrReadUint() ) );
        assert( param == tParameter::pulseRate );
        script.ScrReadSpecial( L'=' );
        float const fValue { Cast2Float( script.ScrReadFloat() ) };
        if ( ! m_pNMS->AddMode() )
            m_pNMS->GetWriterInterface().GetShapePtr<InputNeuron *>( id )->SetPulseFrequency( fHertz( fValue ) );
    }

private:
    NNetModelStorage * m_pNMS;
};

class WrapTriggerSound : public Script_Functor
{
public:
    WrapTriggerSound(NNetModelStorage * const pNNetModelStorage ) :
        m_pNMS( pNNetModelStorage )
    { };

    virtual void operator() ( Script & script ) const 
    {
        ShapeId const id      { m_pNMS->ScrReadShapeId(script) };
        Neuron      * pNeuron { m_pNMS->GetShapePtr<Neuron *>( id ) };
        Hertz   const freq    { script.ScrReadUlong() };
        script.ScrReadString( L"Hertz" );
        MilliSecs const msec { script.ScrReadUlong() };
        script.ScrReadString( L"msec" );
        if ( ! m_pNMS->AddMode() )
            pNeuron->SetTriggerSound( SoundDescr{ true, freq, msec } );
    }

private:
    NNetModelStorage * m_pNMS;
};

class WrapNrOfTracks : public Script_Functor
{
public:
    WrapNrOfTracks(NNetModelStorage * const pNNetModelStorage ) :
        m_pNMS( pNNetModelStorage )
    { };

    virtual void operator() ( Script & script ) const 
    {
        unsigned int const uiNrOfTracks { script.ScrReadUint() };
        if ( m_pNMS->AddMode() )
            return;
        MonitorData      & monitorData { m_pNMS->GetMonitorData() };
        for ( unsigned int ui = 0; ui < uiNrOfTracks; ++ui )
            monitorData.InsertTrack( TrackNr(0) );
    }
private:
    NNetModelStorage * m_pNMS;
};

class WrapSignal : public Script_Functor
{
public:
    WrapSignal( NNetModelStorage * const pNNetModelStorage ) :
        m_pNMS( pNNetModelStorage )
    { };

    virtual void operator() ( Script & script ) const 
    {
        unsigned long    ulSigSrc;
        MicroMeterCircle umCircle;
        TrackNr          trackNr;
        trackNr  = ScrReadTrackNr(script);
        script.ScrReadString( L"source" );
        ulSigSrc = script.ScrReadUlong( );
        if ( ulSigSrc == NNetModelStorage::SIGSRC_CIRCLE )
        {
            umCircle = ScrReadMicroMeterCircle( script );
        }
        else if ( ulSigSrc == NNetModelStorage::SIGSRC_SHAPE_NR )
        {
            m_pNMS->ScrReadShapeId(script);  ///// legacy
        }
        else
        {
            assert( false );  // unexpected signal source 
        }
        if ( ! m_pNMS->AddMode() )
            m_pNMS->GetMonitorData().AddSignal( trackNr, umCircle );
    }

private:
    NNetModelStorage * m_pNMS;
};

void NNetModelStorage::prepareForReading( )
{
    SymbolTable::ScrDefConst( L"shape",  SIGSRC_SHAPE_NR );  // legacy
    SymbolTable::ScrDefConst( L"circle", SIGSRC_CIRCLE );

#define DEF_NNET_FUNC(name) SymbolTable::ScrDefConst( L#name, new Wrap##name##( this ) )
    DEF_NNET_FUNC( Description );
    DEF_NNET_FUNC( Protocol );
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
    if ( AddMode() )
        m_pMWI->SelectAllShapes( tBoolOp::opFalse );
    if ( ProcessNNetScript( m_pScript, m_pMWI, m_wstrPathOfNewModel ) )
    {
        m_pMWI->RemoveOrphans();
        m_pDescription->SetDescription( m_wstrDescription );
        m_wstrPathOfOpenModel = m_wstrPathOfNewModel;
        m_pMWI->StaticModelChanged();
        setUnsavedChanges( AddMode() ); // read new model or add module?
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

bool NNetModelStorage::Read
( 
    wstring const wstrPath, 
    bool    const bAsync,
    bool    const bResetModel
)
{
    bool bRes { true };
    if ( ! m_bPreparedForReading )
        prepareForReading( );

    m_wstrPathOfNewModel = ( wstrPath == L"" ) ? m_wstrPathOfOpenModel : wstrPath;
    if ( exists( m_wstrPathOfNewModel ) )
    {
        if ( bResetModel )
        {
            m_pMWI->ResetModel();
            m_lShapeIdOffset = 0L;
            m_wstrDescription.clear();
        }
        else
        {
            m_lShapeIdOffset = Cast2Long(m_pMRI->GetSizeOfShapeList());
        }

        wcout << L"*** Reading file " << m_wstrPathOfNewModel << endl;
        if ( bAsync )
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

void NNetModelStorage::ReadAsync( wstring const wstrPath, bool const bReset )
{
    Read( wstrPath, true, bReset );
}
