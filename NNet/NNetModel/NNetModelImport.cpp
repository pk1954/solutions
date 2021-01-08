// NNetModelImport.cpp
//
// NNetModel

#include "stdafx.h"
#include <filesystem>
#include <assert.h>
#include "SignalFactory.h"
#include "MonitorData.h"
#include "NNetError.h"
#include "NNetWrapperHelpers.h"
#include "NNetParameters.h"
#include "InputNeuron.h"
#include "win32_script.h"
#include "win32_thread.h"
#include "NNetModelStorage.h"
#include "NNetModelImport.h"

using std::filesystem::exists;

class WrapProtocol : public Script_Functor
{
public:
    WrapProtocol( ) { };

    virtual void operator() ( Script & script ) const 
    {
        script.ScrReadString( L"version" );
        double dVersion = script.ScrReadFloat();
    }
};

class WrapDescription : public Script_Functor
{
public:
    WrapDescription( NNetModelWriterInterface & nmwi ) :
        m_nmwi( nmwi )
    { };

    virtual void operator() ( Script & script ) const 
    {
        wstring const wstrDescription { script.ScrReadString( ) };
        m_nmwi.AddDescriptionLine( wstrDescription );
    }

private:
    NNetModelWriterInterface & m_nmwi;
};

class WrapCreateShape : public Script_Functor
{
public:
    WrapCreateShape( NNetModelWriterInterface & nmwi ) :
        m_nmwi( nmwi )
    { };

    virtual void operator() ( Script & script ) const
    {   
        ShapeId   const idFromScript{ ScrReadShapeId(script) };
        ShapeType const shapeType   { static_cast<ShapeType::Value>(script.ScrReadInt( )) };
        UPShape         upShape     { 
                                       shapeType.IsPipeType()
                                       ? createPipe( script )
                                       : createBaseKnot( script, shapeType ) 
                                    };
        if ( upShape )
        {
            upShape->SetId( idFromScript );
            upShape->Select( tBoolOp::opTrue );
            m_nmwi.GetShapes().SetShape2Slot( idFromScript, move(upShape) );
        }
    }

private:

    UPShape createPipe( Script & script ) const
    {
        script.ScrReadSpecial( Pipe::OPEN_BRACKET );
        ShapeId const idStart { ScrReadShapeId(script) };
        for ( int i = 0; i < Pipe::SEPARATOR.length( ); i++ )
            script.ScrReadSpecial( Pipe::SEPARATOR[i] );
        ShapeId const idEnd { ScrReadShapeId(script) };
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
            BaseKnot * const pKnotStart { m_nmwi.GetShapePtr<BaseKnot *>( idStart ) };
            BaseKnot * const pKnotEnd   { m_nmwi.GetShapePtr<BaseKnot *>( idEnd   ) };
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

    NNetModelWriterInterface & m_nmwi;
};

class WrapGlobalParameter : public Script_Functor
{
public:
    WrapGlobalParameter( NNetModelWriterInterface & nmwi ) :
        m_nmwi( nmwi )
    { };

    virtual void operator() ( Script & script ) const 
    {
        tParameter const param( static_cast< tParameter >( script.ScrReadUint() ) );
        script.ScrReadSpecial( L'=' );
        float const fValue { Cast2Float( script.ScrReadFloat() ) };
        m_nmwi.SetParam( param, fValue );
    }

private:
    NNetModelWriterInterface & m_nmwi;
};

class WrapNrOfShapes : public Script_Functor
{
public:
    WrapNrOfShapes(ShapeList & shapeList ) :
        m_shapeList( shapeList )
    { };

    virtual void operator() ( Script & script ) const 
    {
        script.ScrReadSpecial( L'=' );
        long lNrOfShapes { script.ScrReadLong() };
        m_shapeList.Resize( m_shapeList.Size() + lNrOfShapes );
    }

private:
    ShapeList & m_shapeList;
};

class WrapShapeParameter : public Script_Functor
{
public:
    WrapShapeParameter( NNetModelWriterInterface & nmwi ) :
        m_nmwi( nmwi )
    { };

    virtual void operator() ( Script & script ) const 
    {
        script.ScrReadString( L"InputNeuron" );
        ShapeId    const id   ( ScrReadShapeId(script) );
        tParameter const param( static_cast< tParameter >( script.ScrReadUint() ) );
        assert( param == tParameter::pulseRate );
        script.ScrReadSpecial( L'=' );
        float const fValue { Cast2Float( script.ScrReadFloat() ) };
        m_nmwi.GetShapePtr<InputNeuron *>( id )->SetPulseFrequency( fHertz( fValue ) );
    }

private:
    NNetModelWriterInterface & m_nmwi;
};

class WrapTriggerSound : public Script_Functor
{
public:
    WrapTriggerSound( NNetModelWriterInterface & nmwi ) :
        m_nmwi( nmwi )
    { };

    virtual void operator() ( Script & script ) const 
    {
        ShapeId const id      { ScrReadShapeId(script) };
        Neuron      * pNeuron { m_nmwi.GetShapePtr<Neuron *>( id ) };
        Hertz   const freq    { script.ScrReadUlong() };
        script.ScrReadString( L"Hertz" );
        MilliSecs const msec { script.ScrReadUlong() };
        script.ScrReadString( L"msec" );
        pNeuron->SetTriggerSound( SoundDescr{ true, freq, msec } );
    }

private:
    NNetModelWriterInterface & m_nmwi;
};

class WrapNrOfTracks : public Script_Functor
{
public:
    WrapNrOfTracks( MonitorData & data ) :
        m_monitorData( data )
    { };

    virtual void operator() ( Script & script ) const 
    {
        unsigned int const uiNrOfTracks { script.ScrReadUint() };
        for ( unsigned int ui = 0; ui < uiNrOfTracks; ++ui )
            m_monitorData.InsertTrack( TrackNr(0) );
    }
private:
    MonitorData & m_monitorData;
};

class WrapSignal : public Script_Functor
{
public:
    WrapSignal( MonitorData & data ) :
        m_monitorData( data )
    { };

    virtual void operator() ( Script & script ) const 
    {
        MicroMeterCircle umCircle;
        TrackNr          trackNr { ScrReadTrackNr(script) };
        script.ScrReadString( L"source" );
        unsigned long    ulSigSrc { script.ScrReadUlong( ) };
        if ( ulSigSrc == NNetModelStorage::SIGSRC_CIRCLE )
        {
            umCircle = ScrReadMicroMeterCircle( script );
        }
        else if ( ulSigSrc == NNetModelStorage::SIGSRC_SHAPE_NR )
        {
            ScrReadShapeId(script);  ///// legacy
        }
        else
        {
            assert( false );  // unexpected signal source 
        }
        m_monitorData.AddSignal( trackNr, umCircle );
    }

private:
    MonitorData & m_monitorData;
};

void NNetModelImport::Initialize
(
    NNetModelWriterInterface * const pNMWI, 
    Script                   * const pScript,       
    ReadModelResult          * const pResult
)
{
    m_pMWI    = pNMWI;
    m_pScript = pScript;
    m_pResult = pResult;
}

void NNetModelImport::prepareForReading( )
{
    SymbolTable::ScrDefConst( L"shape",           NNetModelStorage::SIGSRC_SHAPE_NR );  // legacy
    SymbolTable::ScrDefConst( L"circle",          NNetModelStorage::SIGSRC_CIRCLE   );
    SymbolTable::ScrDefConst( L"Description",     new WrapDescription    ( GetWriterInterface() ) );
    SymbolTable::ScrDefConst( L"Protocol",        new WrapProtocol       ( ) );
    SymbolTable::ScrDefConst( L"GlobalParameter", new WrapGlobalParameter( GetWriterInterface() ) );
    SymbolTable::ScrDefConst( L"NrOfShapes",      new WrapNrOfShapes     ( GetWriterInterface().GetShapes() ) );
    SymbolTable::ScrDefConst( L"CreateShape",     new WrapCreateShape    ( GetWriterInterface() ) );
    SymbolTable::ScrDefConst( L"ShapeParameter",  new WrapShapeParameter ( GetWriterInterface() ) );
    SymbolTable::ScrDefConst( L"TriggerSound",    new WrapTriggerSound   ( GetWriterInterface() ) );
    SymbolTable::ScrDefConst( L"NrOfTracks",      new WrapNrOfTracks     ( GetMonitorData() ) );
    SymbolTable::ScrDefConst( L"Signal",          new WrapSignal         ( GetMonitorData() ) );

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

bool NNetModelImport::readModel( wstring const wstrPath, NNetModelWriterInterface & nmwi ) 
{
    if ( ProcessNNetScript( m_pScript, nmwi.GetShapes(), wstrPath ) )
    {
        nmwi.RemoveOrphans();
        nmwi.SetModelFilePath( wstrPath );
        nmwi.StaticModelChanged();
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
    NNetModelImport * pModelImport { reinterpret_cast<NNetModelImport *>( data ) };
//    pModelImport->readModel( );  XXXXX
    return 0;
}

bool NNetModelImport::Import
( 
    wstring     const wstrPath, 
    ShapeList * const pShapeList,
    bool        const bAsync
)
{
// prepare new architecture /////
    NNetModel newModel;
    newModel.Initialize
    (
        nullptr,
        nullptr,
        nullptr,
        nullptr
    );

    NNetModelWriterInterface newNMWI;
    newNMWI.Start( & newModel );
// end new architecture ///

    bool bRes { true };
    if ( ! m_bPreparedForReading )
        prepareForReading( );

    //m_wstrPathOfNewModel = ( wstrPath == L"" ) ? m_pModelStorage->GetModelPath() : wstrPath;
    //if ( exists( m_wstrPathOfNewModel ) )
    //{
    //    m_pMWI->ResetModel();
    //    m_wstrDescription.clear();

    //    wcout << L"*** Reading file " << m_wstrPathOfNewModel << endl;
    //    if ( bAsync )
    //    {
    //        UINT threadId { 0 };
    //        Util::RunAsAsyncThread( readModelThreadProc, static_cast<void *>(this), & threadId );
    //    }
    //    else
    //    {
    //        bRes = readModel( );
    //    }
    //}
    //else
    //{
    //    m_pResult->Reaction( ReadModelResult::tResult::fileNotFound, wstrPath );
    //    bRes = false;
    //}
    return bRes;
}
