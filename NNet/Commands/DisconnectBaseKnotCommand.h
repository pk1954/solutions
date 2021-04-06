// DisconnectBaseKnotCommand.h
//
// Commands

#pragma once

#include <vector>
#include "NNetModelWriterInterface.h"
#include "Command.h"
#include "ShapeId.h"
#include "BaseKnot.h"
#include "Knot.h"

using std::vector;
using std::unique_ptr;

// DisconnectBaseKnot and DeleteBaseKnot are nearly identical.
// For Delete... functioniality create DisconnectBaseKnot 
// with 3rd parameter set to true.

class DisconnectBaseKnotCommand : public Command
{
public:
    DisconnectBaseKnotCommand( ShapeId const, bool const );

    ~DisconnectBaseKnotCommand() {}

    virtual void Do  ( NNetModelWriterInterface & );
    virtual void Undo( NNetModelWriterInterface & );

private:
    void init( NNetModelWriterInterface & );

    BaseKnot               * m_pBaseKnot { nullptr };
    unique_ptr<BaseKnot>     m_upBaseKnot;

    vector<unique_ptr<Knot>> m_startKnots  { };
    vector<unique_ptr<Knot>> m_endKnots    { };
    vector<ShapeId>          m_idStartKnots{ };
    vector<ShapeId>          m_idEndKnots  { };

    ShapeId            const m_idBaseKnot;
    bool                     m_bDelete; // true: delete BaseKnot, false: disconnect only
    bool                     m_bInitialized { false };
};
