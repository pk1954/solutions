// NNetModelCommands.h
//
// NNetModel

#pragma once

#include <string>
#include "boolOp.h"
#include "tParameter.h"
#include "MoreTypes.h"
#include "MonitorData.h"
#include "ShapeId.h"

class Param;
class Observable;
class CommandStack;
class NNetModelStorage;
class NNetModelReaderInterface;
class NNetModelWriterInterface;

struct SoundDescr;

using std::wcout;
using std::wstring;
using std::wostream;

class NNetModelCommands
{
public:
    void Initialize
    ( 
        NNetModelWriterInterface * const,
        Param                    * const,
        CommandStack             * const,
        NNetModelStorage         * const,
        Observable               * const
    );

    void AddIncoming2Knot    ( ShapeId const, MicroMeterPoint const & );
    void AddIncoming2Pipe    ( ShapeId const, MicroMeterPoint const & );
    void AddOutgoing2Knot    ( ShapeId const, MicroMeterPoint const & );
    void AddOutgoing2Pipe    ( ShapeId const, MicroMeterPoint const & );
    void AnalyzeAnomalies    ( );
    void AnalyzeLoops        ( );
    void AppendInputNeuron   ( ShapeId const );
    void AppendNeuron        ( ShapeId const );
    void ClearBeepers        ( );
    void Connect             ( ShapeId const, ShapeId const );
    void CopySelection       ( );
    void DeleteSelection     ( );
    void DeleteShape         ( ShapeId const );
    void Disconnect          ( ShapeId const );
    void InsertNeuron        ( ShapeId const, MicroMeterPoint const & );
    void MoveSelection       ( MicroMeterPoint const & );
    void MoveShape           ( ShapeId const, MicroMeterPoint const & );
    void NewInputNeuron      ( MicroMeterPoint const & );
    void NewNeuron           ( MicroMeterPoint const & );
    void ReadModel           ( bool, wstring const );
    void ResetModel          ( );
    void SelectAll           ( tBoolOp const );
    void SelectAllBeepers    ( );
    void SelectShape         ( ShapeId const, tBoolOp const );
    void SelectShapesInRect  ( MicroMeterRect const & );
    void SelectSubtree       ( ShapeId const, tBoolOp const );
    void SetParameter        ( tParameter const, float const );
    void SetPulseRate        ( ShapeId    const, fHertz const );
    void SetTriggerSound     ( ShapeId const, SoundDescr const & );
    void ToggleStopOnTrigger ( ShapeId const );

    void UndoCommand();
    void RedoCommand();

private:
    void deleteShape( ShapeId const );

    bool       IsTraceOn  ( ) const { return m_bTrace; }
    wostream & TraceStream( )       { return wcout; }

    bool                       m_bTrace                  { true };
    CommandStack             * m_pCmdStack               { nullptr };
    NNetModelWriterInterface * m_pMWI                    { nullptr };
    NNetModelStorage         * m_pStorage                { nullptr };
    Param                    * m_pParam                  { nullptr };
    Observable               * m_pDynamicModelObservable { nullptr };
};