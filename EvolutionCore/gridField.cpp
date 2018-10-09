// gridField.cpp :
//

#include "stdafx.h"
#include <iomanip>
#include <iomanip>
#include <algorithm>  // min/max templates
#include "config.h"
#include "gridField.h"

int   GridField::m_iFertilizerYield;
int   GridField::m_iMaxFertilizer;
short GridField::m_sFoodReserve;
short GridField::m_sMaxFood;

void GridField::InitClass( )
{
    m_iFertilizerYield = Config::GetConfigValue( Config::tId::fertilizerYield );
    m_iMaxFertilizer   = Config::GetConfigValue( Config::tId::maxFertilizer );
    m_sFoodReserve     = Config::GetConfigValueShort( Config::tId::reserveFood );
    m_sMaxFood         = Config::GetConfigValueShort( Config::tId::maxFood );
}

void GridField::InitGridFieldStructure( GridPoint const & gp )
{
    m_gp = gp;
}

void GridField::ResetGridField( short const sFood )
{
    CutConnections( );
    m_sMutatRate  = 0;
    m_sFertility  = sFood;
    m_sFoodStock  = sFood;
    m_sFertilizer = 0;
    m_Individual.ResetIndividual( );
}

void GridField::CreateIndividual( IndId const id, EVO_GENERATION const genBirth, tStrategyId const s )
{
    m_Individual.Create( id, genBirth, s );
}

void GridField::Donate( GridField & gfDonator, short sDonation )
{
    gfDonator.DecEnergy( sDonation );
    IncEnergy( sDonation );
}

void GridField::Fertilize( short const sInvest )
{
    assert( sInvest > 0 );
    int const iYield    = (sInvest * m_iFertilizerYield ) / 100;
    int const iNewValue = min( m_sFertilizer + iYield, m_iMaxFertilizer ); 
    setFertilizer( CastToShort( iNewValue ) );
}

void GridField::CloneIndividual( IndId const id, EVO_GENERATION const genBirth, Random & random, GridField & gfParent )
{
    m_Individual.Clone( id, genBirth, m_sMutatRate, random, gfParent.m_Individual );
    long lDonationRate = static_cast<long>( gfParent.GetAllele( tGeneType::cloneDonation ) );
    long lParentEnergy = static_cast<long>( gfParent.GetEnergy( ) );
    long lDonation = ( lDonationRate * lParentEnergy ) / SHRT_MAX;
    Donate( gfParent, CastToShort( lDonation ) );
}

void GridField::PassOn2Child( IndId const id, EVO_GENERATION const genBirth, Random & random )
{
    m_Individual.Clone( id, genBirth, m_sMutatRate, random, m_Individual );
}

void GridField::BreedIndividual( IndId const id, EVO_GENERATION const genBirth, Random & random, GridField & gfParentA, GridField & gfParentB )
{
    m_Individual.Breed( id, genBirth, m_sMutatRate, random, gfParentA.m_Individual, gfParentB.m_Individual );
    Donate( gfParentA, gfParentA.GetEnergy( ) / 3 );   //TODO:  Make variable, Gene?
    Donate( gfParentB, gfParentB.GetEnergy( ) / 3 );   //TODO:  Make variable, Gene?
}

void GridField::MoveIndividual( GridField & gfSrc )
{
    m_Individual = gfSrc.m_Individual;
    gfSrc.m_Individual.ResetIndividual( );
}

void GridField::CutConnections( )
{
    m_gpJunior.Set2Null( );
    m_gpSenior.Set2Null( );
}

void GridField::Interact( GridField & gfA, GridField & gfB )
{
    INTERACTION::Interact( gfA.m_Individual, gfB.m_Individual );
    gfA.SetLastAction( tAction::interact );
};

std::wostream & operator << ( std::wostream & out, GridField const & gf )
{
//lint -e747  Significant prototype coercion with setw
    out << gf.GetGridPoint( );
    out << L" en="           << setw(8) << gf.GetEnergy( );
//  out << L" last action: " << setw(9) << GetActionTypeName( gf.GetLastAction() );
//lint +e747 
    return out;
}

