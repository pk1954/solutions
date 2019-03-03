// plannedActivity.h
//
// EvolutionCoreInterface

#pragma once

#include "gridPoint.h"
#include "EvolutionTypes.h"

class PlannedActivity
{ 
public:
    PlannedActivity() 
      : m_gpActor  ( GridPoint::NULL_VAL() ),
        m_gpTarget ( GridPoint::NULL_VAL() ),
        m_gpPartner( GridPoint::NULL_VAL() ),
        m_at( tAction::undefined ),
        m_iBaseConsumption( 0 ),
        m_bValid( false ) 
    {}

    void SetInvalid( ) { m_bValid = false; }
    void SetValid  ( ) { m_bValid = true; }

    bool IsValid( ) const { return m_bValid; }

	bool HasTarget ( ) const { return m_gpTarget .IsNotNull(); }
	bool HasPartner( ) const { return m_gpPartner.IsNotNull(); }

    GridPoint GetActor  ( ) const { return m_gpActor; }
    GridPoint GetTarget ( ) const { return m_gpTarget; }
    GridPoint GetPartner( ) const { return m_gpPartner; }

    void SetActor  ( GridPoint const gp ) { m_gpActor   = gp; }
    void SetTarget ( GridPoint const gp ) { m_gpTarget  = gp; }
    void SetPartner( GridPoint const gp ) { m_gpPartner = gp; }

    void NoTarget ( ) { m_gpTarget .Set2Null( ); }
    void NoPartner( ) { m_gpPartner.Set2Null( ); }

    int  GetBaseConsumption( ) const   { return m_iBaseConsumption; }
    void SetBaseConsumption( int const iVal ) { m_iBaseConsumption  = iVal; }
    void IncBaseConsumption( int const iVal ) { m_iBaseConsumption += iVal; }

    tAction GetActionType( ) const { return m_at; }
    void    SetActionType( tAction const at ) { m_at = at;}

private:
    GridPoint m_gpActor;   // acting GridPoint, owner of the plan
    GridPoint m_gpTarget;  // target for move, clone and marry operations
    GridPoint m_gpPartner; // partner for interaction and marry operations
    tAction   m_at;
    int       m_iBaseConsumption;
    bool      m_bValid;
};

std::wostream & operator << ( std::wostream & , PlannedActivity const & );  // implemented in dump.cpp
