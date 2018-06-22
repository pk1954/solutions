// gridField.h : 
//

#pragma once

#include <iostream>
#include "EvolutionTypes.h"
#include "gridPoint.h"
#include "interaction.h"
#include "individual.h"

class GridField
{
public:
    static void InitClass( );

    void InitGridFieldStructure( GridPoint const & );
    void ResetGridField( short const );

    void  Fertilize     ( short const );
    short GetConsumption( short const ) const;

    short          GetMutationRate( ) const { return m_sMutatRate; }
    short          GetFoodStock( )    const { return m_sFoodStock + m_sFertilizer; }
    short          GetFertility( )    const { return m_sFertility; }
    short          GetFertilizer( )   const { return m_sFertilizer; }
    EVO_GENERATION GetGenBirth( )     const { return m_Individual.GetGenBirth( ); }
    tAction        GetLastAction( )   const { return m_Individual.GetLastAction( ); }
    IndId          GetId( )           const { return m_Individual.GetId( ); }
    tOrigin        GetOrigin( )       const { return m_Individual.GetOrigin( ); }
    short          GetEnergy( )       const { return m_Individual.GetEnergy( ); }
    bool           IsDead( )          const { return m_Individual.IsDead( ); }
    bool           IsAlive( )         const { return m_Individual.IsAlive( ); };
    bool           IsDefined( )       const { return m_Individual.IsDefined( ); };
    tStrategyId    GetStrategyId( )   const { return m_Individual.GetStrategyId( ); }
    MEM_INDEX      GetMemSize( )      const { return m_Individual.GetMemSize  ( ); }
    MEM_INDEX      GetMemUsed( )      const { return m_Individual.GetMemUsed( ); }
    Genome const & GetGenome( )       const { return m_Individual.GetGenome( ); }

    short  const   GetAllele( tGeneType const geneType ) const { return GetGenome( ).GetAllele( geneType ); }

    long GetMemEntry( MEM_INDEX const i ) const { return m_Individual.GetMemEntry( i ).GetLong( ); }
    void ResetIndividual( )                     { m_Individual.ResetIndividual( ); }
    void SetEnergy( short const sInc )          { m_Individual.SetEnergy( sInc ); }
    void DecEnergy( short const sDec )          { m_Individual.DecEnergy( sDec ); }
    void IncEnergy( short const sInc )          { m_Individual.IncEnergy( sInc ); }
    void SetLastAction( tAction const at )      { m_Individual.SetLastAction( at ); }

    void CreateIndividual( IndId const id, EVO_GENERATION const genBirth, tStrategyId const s )
    {
        m_Individual.Create( id, genBirth, s );
    }

    void Donate( GridField & gfDonator, short sDonation )
    {
        gfDonator.DecEnergy( sDonation );
        IncEnergy( sDonation );
    }

    void CloneIndividual( IndId const & id, EVO_GENERATION const genBirth, Random & random, GridField & gfParent )
    {
        m_Individual.Clone( id, genBirth, m_sMutatRate, random, gfParent.m_Individual );
        long lDonationRate = static_cast<long>( gfParent.GetAllele( tGeneType::cloneDonation ) );
        long lParentEnergy = static_cast<long>( gfParent.GetEnergy( ) );
        long lDonation = ( lDonationRate * lParentEnergy ) / SHRT_MAX;
        assert( lDonation <= SHRT_MAX );
        Donate( gfParent, static_cast<short>( lDonation ) );
    }

    void BreedIndividual( IndId const & id, EVO_GENERATION const genBirth, Random & random, GridField & gfParentA, GridField & gfParentB )
    {
        m_Individual.Breed( id, genBirth, m_sMutatRate, random, gfParentA.m_Individual, gfParentB.m_Individual );
        Donate( gfParentA, gfParentA.GetEnergy( ) / 3 );   //TODO:  Make variable, Gene?
        Donate( gfParentB, gfParentB.GetEnergy( ) / 3 );   //TODO:  Make variable, Gene?
    }

    void MoveIndividual( GridField & gfSrc )
    {
        m_Individual = gfSrc.m_Individual;
        gfSrc.m_Individual.ResetIndividual( );
    }

    static void Interact( GridField & gfA, GridField & gfB )
    {
        INTERACTION::Interact( gfA.m_Individual, gfB.m_Individual );
        gfA.SetLastAction( tAction::interact );
    };

    void SetMutationRate( short const );
    void SetFertilizer  ( short const );
    void SetFoodStock   ( short const );
    void SetFertility   ( short const );

    void IncFertilizer  ( short const );
    void IncFoodStock   ( short const );
    void DecFoodStock   ( short const );
    void IncFertility   ( short const );
    void IncMutationRate( short const );

    void ReduceFertilizer( ) { m_sFertilizer /= 2; }

    GridPoint const & GetGridPoint( ) const { return m_gp; }

    GridPoint const & GetSeniorGp( ) const { return m_gpSenior; }
    GridPoint const & GetJuniorGp( ) const { return m_gpJunior; }

    bool IsOldest  ( ) const { return m_gpSenior.IsNull( ); }
    bool IsYoungest( ) const { return m_gpJunior.IsNull( ); }

    void SetSeniorGp( GridPoint const & gp ) { m_gpSenior = gp; }
    void SetJuniorGp( GridPoint const & gp ) { m_gpJunior = gp; }

    void CutConnections( )
    {
        m_gpJunior.Set2Null( );
        m_gpSenior.Set2Null( );
    }

private:
    // data for management of neighborhood relation and list of living individuals

    GridPoint  m_gp;            //   4 byte     will stay unchanged after initialization
    GridPoint  m_gpSenior;      //   4 byte
    GridPoint  m_gpJunior;      //   4 byte

// configuraton data, changed only by user 

    short      m_sMutatRate;    //   2 byte
    short      m_sFertility;    //   2 byte     normal fertility of soil

// data changed by algorithm

    Individual m_Individual;    // 162 byte
    short      m_sFoodStock;    //   2 byte 
    short      m_sFertilizer;   //   2 byte
                    // sum         186 byte

// static members for caching frequently used configuration items

    static int   m_iFertilizerYield;
    static int   m_iMaxFertilizer;
    static short m_sFoodReserve;
    static short m_sMaxFood;
};

std::wostream & operator << ( std::wostream &, GridField const & );

class Decision_Functor
{
public:
    virtual ~Decision_Functor() {};
    virtual bool operator() ( GridField const & ) const = 0; 
};

class IsDead_Functor : public Decision_Functor
{
public:
    virtual ~IsDead_Functor() {};
    virtual bool operator() ( GridField const & gf ) const { return gf.IsDead( ); }; 
};

class IsAlive_Functor : public Decision_Functor
{
public:
    virtual ~IsAlive_Functor() {};
    virtual bool operator() ( GridField const & gf ) const { return gf.IsAlive( ); }; 
};

class IsCompatible_Functor : public Decision_Functor
{
public:
    explicit IsCompatible_Functor( tStrategyId const strat ) : m_strat( strat ) { };
    virtual ~IsCompatible_Functor() {};
    virtual bool operator() ( GridField const & gf ) const 
    { 
        return gf.IsAlive() && ( gf.GetStrategyId() == m_strat ); 
    }; 
private:
    tStrategyId m_strat;
};
