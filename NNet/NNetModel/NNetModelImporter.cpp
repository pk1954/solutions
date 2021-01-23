// NNetModelImporter.cpp
//
// NNetModel

#include "stdafx.h"
#include <filesystem>
#include <assert.h>
#include "SignalFactory.h"
#include "Knot.h"
#include "MonitorData.h"
#include "NNetError.h"
#include "NNetWrapperHelpers.h"
#include "NNetParameters.h"
#include "InputNeuron.h"
#include "win32_script.h"
#include "win32_thread.h"
#include "NNetModelStorage.h"
#include "NNetModelImporter.h"

using std::filesystem::exists;
using std::make_unique;

class WrapBase : public Script_Functor
{
public:
    WrapBase( NNetModelImporter & modelImporter ) :
        m_modelImporter( modelImporter )
    { };

protected:
    NNetModelWriterInterface & GetWriterInterface() const { return m_modelImporter.getWriterInterface(); }
    MonitorData              & GetMonitorData()     const { return m_modelImporter.getWriterInterface().GetMonitorData(); }
    ShapeList                & GetShapes()          const { return m_modelImporter.getWriterInterface().GetShapes(); }

    NNetModelImporter & m_modelImporter;
};

class WrapProtocol : public WrapBase
{
public:
    WrapProtocol(NNetModelImporter & m) : WrapBase(m) { };

    virtual void operator() ( Script & script ) const
    {
        script.ScrReadString( L"version" );
        double dVersion = script.ScrReadFloat();
    }
};

class WrapDescription : public WrapBase
{
public:
    WrapDescription(NNetModelImporter & m) : WrapBase(m) { };

    virtual void operator() ( Script & script ) const
    {
        wstring const wstrDescription { script.ScrReadString( ) };
        GetWriterInterface().AddDescriptionLine( wstrDescription );
    }
};

class WrapCreateShape : public WrapBase
{
public:
    WrapCreateShape(NNetModelImporter & m) : WrapBase(m) { };

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
            GetShapes().SetShape2Slot( idFromScript, move(upShape) );
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
            BaseKnot * const pKnotStart { GetWriterInterface().GetShapePtr<BaseKnot *>( idStart ) };
            BaseKnot * const pKnotEnd   { GetWriterInterface().GetShapePtr<BaseKnot *>( idEnd   ) };
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
};

class WrapGlobalParameter : public WrapBase
{
public:
    WrapGlobalParameter(NNetModelImporter & m) : WrapBase(m) { };

    virtual void operator() ( Script & script ) const
    {
        ParameterType::Value const param( static_cast< ParameterType::Value >( script.ScrReadUint() ) );
        script.ScrReadSpecial( L'=' );
        float const fValue { Cast2Float( script.ScrReadFloat() ) };
        GetWriterInterface().SetParam( param, fValue );
    }
};

class WrapNrOfShapes : public WrapBase
{
public:
    WrapNrOfShapes(NNetModelImporter & m) : WrapBase(m) { };

    virtual void operator() ( Script & script ) const
    {
        script.ScrReadSpecial( L'=' );
        long lNrOfShapes { script.ScrReadLong() };
        GetShapes().Resize( GetShapes().Size() + lNrOfShapes );
    }
};

class WrapShapeParameter : public WrapBase
{
public:
    WrapShapeParameter(NNetModelImporter & m) : WrapBase(m) { };

    virtual void operator() ( Script & script ) const
    {
        script.ScrReadString( L"InputNeuron" );
        ShapeId    const id   ( ScrReadShapeId(script) );
        ParameterType::Value const param( static_cast< ParameterType::Value >( script.ScrReadUint() ) );
        assert( param == ParameterType::Value::pulseRate );
        script.ScrReadSpecial( L'=' );
        float const fValue { Cast2Float( script.ScrReadFloat() ) };
        GetWriterInterface().GetShapePtr<InputNeuron *>( id )->SetPulseFrequency( fHertz( fValue ) );
    }
};

class WrapTriggerSound : public WrapBase
{
public:
    WrapTriggerSound(NNetModelImporter & m) : WrapBase(m) { };

    virtual void operator() ( Script & script ) const  
    {
        ShapeId const id      { ScrReadShapeId(script) };
        Neuron      * pNeuron { GetWriterInterface().GetShapePtr<Neuron *>( id ) };
        Hertz   const freq    { script.ScrReadUlong() };
        script.ScrReadString( L"Hertz" );
        MilliSecs const msec { script.ScrReadUlong() };
        script.ScrReadString( L"msec" );
        pNeuron->SetTriggerSound( SoundDescr{ true, freq, msec } );
    }
};

class WrapNrOfTracks : public WrapBase
{
public:
    WrapNrOfTracks(NNetModelImporter & m) : WrapBase(m) { };

    virtual void operator() ( Script & script ) const 
    {
        unsigned int const uiNrOfTracks { script.ScrReadUint() };
        for ( unsigned int ui = 0; ui < uiNrOfTracks; ++ui )
            GetMonitorData().InsertTrack( TrackNr(0) );
    }
};

class WrapSignal : public WrapBase
{
public:
    WrapSignal(NNetModelImporter & m) : WrapBase(m) { };

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
        GetMonitorData().AddSignal( trackNr, umCircle );
    }
};

void NNetModelImporter::Initialize( Script * const pScript )
{
    m_pScript = pScript;
    SymbolTable::ScrDefConst( L"shape",           NNetModelStorage::SIGSRC_SHAPE_NR );  // legacy
    SymbolTable::ScrDefConst( L"circle",          NNetModelStorage::SIGSRC_CIRCLE   );
    SymbolTable::ScrDefConst( L"Description",     new WrapDescription    ( * this ) );
    SymbolTable::ScrDefConst( L"Protocol",        new WrapProtocol       ( * this ) );
    SymbolTable::ScrDefConst( L"GlobalParameter", new WrapGlobalParameter( * this ) );
    SymbolTable::ScrDefConst( L"NrOfShapes",      new WrapNrOfShapes     ( * this ) );
    SymbolTable::ScrDefConst( L"CreateShape",     new WrapCreateShape    ( * this ) );
    SymbolTable::ScrDefConst( L"ShapeParameter",  new WrapShapeParameter ( * this ) );
    SymbolTable::ScrDefConst( L"TriggerSound",    new WrapTriggerSound   ( * this ) );
    SymbolTable::ScrDefConst( L"NrOfTracks",      new WrapNrOfTracks     ( * this ) );
    SymbolTable::ScrDefConst( L"Signal",          new WrapSignal         ( * this ) );

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
    ParameterType::Apply2AllParameters
    ( 
        [&]( ParameterType::Value const & param ) 
        {
            SymbolTable::ScrDefConst
            ( 
                ParameterType::GetName( param ), 
                static_cast<unsigned long>( param ) 
            );
        }
    );
}

void NNetModelImporter::import( ) 
{
    ImportTermination::Result res;

    if ( ProcessNNetScript( * m_pScript, m_ImportedNMWI.GetShapes(), m_wstrFile2Read ) )
    {
        m_ImportedNMWI.RemoveOrphans();
        m_ImportedNMWI.SetModelFilePath( m_wstrFile2Read );
        m_ImportedNMWI.DescriptionComplete( );
        res = ImportTermination::Result::ok;
    }
    else
    {
        res = ImportTermination::Result::errorInFile;
    }
    m_upTermination->Reaction( res, m_wstrFile2Read );
}

static unsigned int __stdcall importModelThreadProc( void * data ) 
{
    SetThreadDescription( GetCurrentThread(), L"ImportModel" );
    NNetModelImporter * pModelImporter { reinterpret_cast<NNetModelImporter *>( data ) };
    pModelImporter->import( );
    return 0;
}

bool NNetModelImporter::Import
( 
    wstring                 const wstrPath,
    unique_ptr<ImportTermination> upTermination
)
{
    if ( wstrPath.empty() )
        return false;

    if ( m_upImportedModel.get() ) 
        return false;       // another import is already running

    if ( ! exists( wstrPath ) )
    {
        m_upTermination->Reaction( ImportTermination::Result::fileNotFound, wstrPath );
        return false;
    }

    m_upTermination   = move(upTermination);
    m_upImportedModel = make_unique<NNetModel>(); // do not initialize here
    m_ImportedNMWI.Start( m_upImportedModel.get() );
    m_wstrFile2Read = wstrPath;
    wcout << L"*** Reading file " << wstrPath << endl;
    Util::RunAsAsyncThread( importModelThreadProc, static_cast<void *>(this) );
    return true;
}

unique_ptr<NNetModel> NNetModelImporter::GetImportedModel( ) 
{ 
    return move( m_upImportedModel ); 
}
