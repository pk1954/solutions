// DisconnectBaseKnotCommand.h
//
// Commands

#pragma once

#include <vector>
#include "NNetModelWriterInterface.h"
#include "Command.h"
#include "BaseKnot.h"
#include "Knot.h"

using std::vector;

// DisconnectBaseKnot and DeleteBaseKnot are nearly identical
// for Delete... functioniality create DisconnectBaseKnot 
// with 3rd parameter set to true

class DisconnectBaseKnotCommand : public Command
{
public:
    DisconnectBaseKnotCommand
    ( 
        NNetModelWriterInterface * const pModel, 
        BaseKnot                 * const pBaseKnot, 
        bool                       const bDelete 
    )
      :	m_pBaseKnot( pBaseKnot ),
        m_bDelete( bDelete )
    { 
        MicroMeterPoint umPos { m_pBaseKnot->GetPosition() };
        m_pBaseKnot->m_connections.Apply2AllInPipes
        ( 
            [&]( Pipe & pipe ) // every incoming Pipe needs a new end knot
            { 
                Knot * pKnotNew { pModel->NewBaseKnot<Knot>( umPos ) };
                pKnotNew->m_connections.AddIncoming( & pipe );
                m_endKnots.push_back( pKnotNew );
                pipe.DislocateEndPoint( );
            } 
        );
        m_pBaseKnot->m_connections.Apply2AllOutPipes
        ( 
            [&]( Pipe & pipe ) // every incoming Pipe needs a new end knot
            { 
                Knot * pKnotNew { pModel->NewBaseKnot<Knot>( umPos ) };
                pKnotNew->m_connections.AddOutgoing( & pipe );
                m_startKnots.push_back( pKnotNew );
                pipe.DislocateStartPoint( );
            } 
        );
    }

    ~DisconnectBaseKnotCommand( )
    {
        for ( Knot * pKnot : m_startKnots )
            delete pKnot;
        for ( Knot * pKnot : m_endKnots )
            delete pKnot;
    }

    virtual void Do( NNetModelWriterInterface * const pModel )
    {
        for ( Knot * pKnot : m_startKnots )
        {
            pKnot->m_connections.GetFirstOutgoing().SetStartKnot( pKnot );
            pModel->Store2Model( pKnot );
        }
        for ( Knot * pKnot : m_endKnots )
        {
            pKnot->m_connections.GetFirstIncoming().SetEndKnot( pKnot );
            pModel->Store2Model( pKnot );
        }
        m_pBaseKnot->ClearConnections();
        if ( m_bDelete || m_pBaseKnot->IsKnot() )
            pModel->RemoveFromModel( m_pBaseKnot );
    }

    virtual void Undo( NNetModelWriterInterface * const pModel )
    {
        for ( Knot * pKnot : m_startKnots )
        {
            pKnot->m_connections.GetFirstOutgoing().SetStartKnot( m_pBaseKnot );
            pModel->RemoveFromModel( pKnot );
        }
        for ( Knot * pKnot : m_endKnots )
        {
            pKnot->m_connections.GetFirstIncoming().SetEndKnot( m_pBaseKnot );
            pModel->RemoveFromModel( pKnot );
        }
        pModel->Store2Model( m_pBaseKnot );
    }

private:
    BaseKnot     * m_pBaseKnot;
    vector<Knot *> m_startKnots { };
    vector<Knot *> m_endKnots   { };
    bool           m_bDelete; // true: delete BaseKnot, false: disconnect only
};
