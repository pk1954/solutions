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
    DisconnectBaseKnotCommand( NNetModelWriterInterface &, ShapeId const, bool const );

    ~DisconnectBaseKnotCommand( ) {}

    virtual void Do  ( NNetModelWriterInterface & );
    virtual void Undo( NNetModelWriterInterface & );


private:
    BaseKnot               * m_pBaseKnot;
    unique_ptr<BaseKnot>     m_upBaseKnot;
    vector<unique_ptr<Knot>> m_startKnots  { };
    vector<unique_ptr<Knot>> m_endKnots    { };
    vector<ShapeId>          m_idStartKnots{ };
    vector<ShapeId>          m_idEndKnots  { };
    bool                     m_bDelete; // true: delete BaseKnot, false: disconnect only
};
