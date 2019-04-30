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
        m_action   ( Action::Id::undefined ),
        m_iBaseConsumption( 0 )
    {}

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

    ENERGY_UNITS GetBaseConsumption( ) const { return m_iBaseConsumption; }

    void SetBaseConsumption( ENERGY_UNITS const iVal ) { m_iBaseConsumption  = iVal; }
    void IncBaseConsumption( ENERGY_UNITS const iVal ) { m_iBaseConsumption += iVal; }

    Action::Id GetActionType( ) const { return m_action; }
    void       SetActionType( Action::Id const action ) { m_action = action;}

private:
    GridPoint    m_gpActor;   // acting GridPoint, owner of the plan
    GridPoint    m_gpTarget;  // target for move, clone and marry operations
    GridPoint    m_gpPartner; // partner for interaction and marry operations
    Action::Id   m_action;
    ENERGY_UNITS m_iBaseConsumption;
};
