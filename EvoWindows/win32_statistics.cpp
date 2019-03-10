// win32_statistics.cpp :
//

#include "stdafx.h"
#include "Strsafe.h"
#include "XArray.h"
#include "gridRect.h"
#include "EvolutionCore.h"
#include "win32_baseWindow.h"
#include "win32_statistics.h"
#include "win32_stopwatch.h"

StatisticsWindow::StatisticsWindow( ):
    TextWindow( )
{ }

void StatisticsWindow::Start
(
    HWND                  const hwndParent,
    EvolutionCore const * const pCore
) 
{
    m_pCore = pCore;
    StartTextWindow
	( 
		hwndParent, 
		// PixelRect{ 200_PIXEL_X, 200_PIXEL_Y, 600_PIXEL_X, 745_PIXEL_Y }, 
		PixelRect{ 200_PIXEL_X, 200_PIXEL_Y, 600_PIXEL_X, 900_PIXEL_Y }, 
		L"StatisticsWindow", 
		100, 
		TRUE 
	);
}

StatisticsWindow::~StatisticsWindow( )
{
	m_pCore = nullptr;
}

class XaCounter: public XArray< unsigned int, Strategy::COUNT  >
{
public:

    void printGeneLine( TextBuffer & textBuf, wchar_t const * const data )
    {
        textBuf.nextLine( data );
 		Apply2All( [&](unsigned int elem) { textBuf.printNumber( elem ); } );
    };
};

class XaFloatStat : public XArray< float, Strategy::COUNT >
{
public:

    void printFloatLine( TextBuffer & textBuf, wchar_t const * const data )
    {
		textBuf.nextLine( data );
 		Apply2All( [&](float elem) { textBuf.printFloat( elem ); } );
    };
};

class AllGenesStat
{
public:
    AllGenesStat( )  // constructor initializing all members to zero
    {
        m_auiMemSize.fill( 0 );
    };

    void add2option( Strategy::Id const s, Action::Id const action, short const sValue )
    {
        m_axaGenePoolStrategy.at( static_cast<int>(action) ).Add( static_cast<int>( s ), static_cast<float>( sValue ) );
    }

    void add2Gene( Strategy::Id const s, GeneType::Id const geneType, long const lGenoType )
    {
        assert( lGenoType >= 0 );
        m_aGeneStat.at( static_cast<int>(geneType) ).Add( static_cast<int>( s ), static_cast<unsigned int>( lGenoType ) );
    }

    void addMemSize( Strategy::Id const s, MEM_INDEX const memSize )
    {
        m_auiMemSize.at( static_cast<int>( s ) ) += memSize.GetValue();
    }

    void addAge( Strategy::Id const s, EVO_GENERATION genAge )
    {
        m_gsAverageAge.Add( static_cast<int>( s ), genAge.GetValue() );
    }

    void incCounter( Strategy::Id const s )
    {
        m_gsCounter.Add( static_cast<int>( s ), 1 );
    }

	void aquireData( EvolutionCore const * const pCore, GridPoint const gp )
	{
		Strategy::Id   const strategy { pCore->GetStrategyId( gp ) };
		EVO_GENERATION const age      { pCore->GetAge( gp ) };
		MEM_INDEX      const memSize  { pCore->GetMemSize( gp ) };

		if (  // can happen due to race conditions 
				( strategy != Strategy::Id::empty ) &&
				( age.IsNotNull()  )
			)
		{
			GeneType::Apply2All
			(
				[&]( auto geneType )
				{
					Action::Id action { GetRelatedAction( geneType ) };
					if ( EvolutionCore::IsEnabled( action ) )
						add2option( strategy, action, pCore->GetDistr( gp, geneType ) );
				}
			);

			GeneType::Apply2All
			(
				[&]( auto geneType )
				{ add2Gene( strategy, geneType, pCore->GetGenotype( gp, geneType ) ); }
			);

			addMemSize( strategy, memSize );
			addAge    ( strategy, age );
			incCounter( strategy );
		}
	}

    void scaleAllGenesStat( )
    {
		m_gsAverageAge.DivNonZero( m_gsCounter );

		GeneType::Apply2All
		(
			[&]( GeneType::Id geneType )
			{
				unsigned int uiGeneType = static_cast<int>(geneType);
				Strategy::Apply2All
				(
					[&]( Strategy::Id strategy )
					{
						unsigned int uiStrategy = static_cast<unsigned int>(strategy);
						DivNonZero( m_aGeneStat[uiGeneType][uiStrategy], m_gsCounter[uiStrategy] );
					}
				);
				DivNonZero( m_aGeneStat[uiGeneType].General(), m_gsCounter.General() );
			}
		);

		float fSum { 0.0 };
        for ( auto & floatStat: m_axaGenePoolStrategy )
            fSum += floatStat.General();
        
        for ( auto & floatStat : m_axaGenePoolStrategy )
            Scale( floatStat.General(), fSum );
            
		Strategy::Apply2All
		( 
			[&]( Strategy::Id strategy )
			{
			float fSum { 0.0 };
				for ( auto & floatStat : m_axaGenePoolStrategy )
					fSum += floatStat[ static_cast<int>(strategy) ];

				for ( auto & floatStat : m_axaGenePoolStrategy )
					Scale( floatStat[ static_cast<int>(strategy) ], fSum );
			}
		);

		Strategy::Apply2All
		( 
			[&]( Strategy::Id strategy )
			{ DivNonZero( m_auiMemSize.at( static_cast<int>(strategy) ), m_gsCounter[ static_cast<int>(strategy) ] ); }
		);
    };

    void printCounter( TextBuffer & textBuf, wchar_t const * const data )
    {
        m_gsCounter.printGeneLine( textBuf, data );
    }

    void printAvAge( TextBuffer & textBuf, wchar_t const * const data )
    {
        m_gsAverageAge.printGeneLine( textBuf, data );
    }

    void printCounters( TextBuffer & textBuf, EvolutionCore const * pCore, Action::Id const action )
    {
		ACTION_COUNT sum { 0 };
		textBuf.nextLine( Action::GetName( action ) );
		Strategy::Apply2All
		( 
			[&]( Strategy::Id strat )
			{
				ACTION_COUNT counter { pCore->GetActionCounter( strat, action ) };
				textBuf.printNumber( counter.GetValue() );
				sum += counter;
			}
		);
		textBuf.printNumber( sum.GetValue() );
    }

    void printIncidence( TextBuffer & textBuf, EvolutionCore const * pCore )
    {
		Action::Apply2All
		(
			[&]( Action::Id action ) -> Action::Id
			{
				if ( EvolutionCore::IsEnabled( action ) )
				{
					printCounters( textBuf, pCore, action );
				}
				return Action::Id::undefined;
			}
		);
	}

    void printProbabilities( TextBuffer & textBuf, EvolutionCore const * pCore )
    {
		Action::Apply2All
		(
			[&]( Action::Id action ) -> Action::Id
			{
				if ( EvolutionCore::IsEnabled( action ) )
				{
					if ( GeneType::IsDefined( GetRelatedGeneType( action ) )  )
						m_axaGenePoolStrategy[ static_cast<int>(action) ].printFloatLine( textBuf, Action::GetName( action ) );
				}
				return Action::Id::undefined;
			}
		);

	}

    void printGeneStat( TextBuffer & textBuf, EvolutionCore const * pCore )
    {
		GeneType::Apply2All
		(
			[&]( GeneType::Id geneType )
			{
			Action::Id action { GetRelatedAction( geneType ) };
				if ( EvolutionCore::IsEnabled( action ) )
					m_aGeneStat[ static_cast<int>(geneType) ].printGeneLine( textBuf, GeneType::GetName( geneType ) );
			}
		);
	}

    void printAvFood( TextBuffer & textBuf, wchar_t const * const data )
    {
        XaFloatStat fsAvFood;

		int iEat      { static_cast<int>( Action::Id::eat ) };
		int iAppetite { static_cast<int>( GeneType::Id::appetite ) };

		Strategy::Apply2All
		( 
			[&]( Strategy::Id strategy )
			{ 
			int iStrategy { static_cast<int>( strategy ) };
				fsAvFood[ iStrategy ] = m_aGeneStat[ iAppetite ][ iStrategy ] * m_axaGenePoolStrategy[ iEat ][ iStrategy ] / 100; 
			}
		);

        fsAvFood.General() = m_aGeneStat[ iAppetite ].General() * m_axaGenePoolStrategy[ iEat ].General() / 100 ;

        fsAvFood.printFloatLine( textBuf, data );
    }

    void printMemory( TextBuffer & textBuf, wchar_t const * const data )
    {
        textBuf.nextLine( data );

        for ( const auto & val : m_auiMemSize )
            textBuf.printNumber( val );
    }

private:
    XaCounter m_gsCounter;          // counter for strategies and sum counter 
    XaCounter m_gsAverageAge;       // average age of all individuals

    std::array < XaFloatStat,  Action::COUNT   > m_axaGenePoolStrategy;
    std::array < XaCounter,    GeneType::COUNT > m_aGeneStat;
    std::array < unsigned int, Strategy::COUNT > m_auiMemSize;
};

Stopwatch stopwatch;

void StatisticsWindow::DoPaint( TextBuffer & textBuf )
{
//	stopwatch.Start();
    // aquire and prepare data 

    AllGenesStat genesStat;

    Apply2Rect
	( 
		[&](GridPoint const gp)
		{
			if ( m_pCore->IsAlive( gp ) )
				genesStat.aquireData( m_pCore, gp );
		},
		m_pCore->GetSelection( )
	);

    genesStat.scaleAllGenesStat( );

    // start printing

    textBuf.setHorizontalPos( 2_TEXT_POSITION );          // header
	Strategy::Apply2All
	( 
		[&]( Strategy::Id const & id ) 
		{ textBuf.printString( Strategy::GetName( id ) ); }
	);
    textBuf.printString( L"all" );                               

    genesStat.printCounter ( textBuf, L"#individuals" );  // number of individuals
    genesStat.printAvAge   ( textBuf, L"av. age" );       // average age
    genesStat.printIncidence( textBuf, m_pCore );
    genesStat.printProbabilities( textBuf, m_pCore );
    genesStat.printGeneStat( textBuf, m_pCore );          // percentage numbers for options

	if ( EvolutionCore::IsEnabled( Action::Id::eat ) )
		genesStat.printAvFood( textBuf, L"av. food" );  // average food consumption 

	genesStat.printMemory( textBuf, L"memory" );        // memory size counters

    // maximum memory size

    textBuf.nextLine( L"max mem" );
    textBuf.setHorizontalPos( 4_TEXT_POSITION );
    textBuf.printNumber( EvolutionCore::GetMaxPartnerMemory( ) );

	if ( EvolutionCore::IsEnabled( Action::Id::interact ) )
	{
		// nr of interactions with known culprits (Tit4Tat only)

		textBuf.nextLine( L"known" );
		textBuf.setHorizontalPos( 4_TEXT_POSITION );
		textBuf.printNumber( EvolutionCore::GetNrInteractionsWithKnownCulprit( ) );

		// nr of interactions with unknown culprits (Tit4Tat only)

		textBuf.nextLine( L"unknown" );
		textBuf.setHorizontalPos( 4_TEXT_POSITION );
		textBuf.printNumber( EvolutionCore::GetNrInteractionsWithUnknownCulprit( ) );
	}

	if ( EvolutionCore::IsEnabled( Action::Id::eat ) )
	{
		// average food growth

		textBuf.nextLine( L"food growth" );
		textBuf.setHorizontalPos( 4_TEXT_POSITION );
		textBuf.printNumber( m_pCore->GetAverageFoodGrowth( ).GetValue() );
	}
//	stopwatch.Stop( L"Statistics" );
}
