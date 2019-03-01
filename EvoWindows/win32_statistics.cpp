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

class GeneStat: public XArray< unsigned int, NR_STRATEGIES  >
{
public:

    void printGeneLine( TextBuffer & textBuf, wchar_t const * const data )
    {
        textBuf.nextLine( data );

        for ( unsigned int uiStrategy = 0; uiStrategy < NR_STRATEGIES; ++uiStrategy )
            textBuf.printNumber( ( *this )[ uiStrategy ] );

        textBuf.printNumber( General( ) );
    };
};

class FloatStat : public XArray< float, NR_STRATEGIES  >
{
public:

    void printFloatLine( TextBuffer & textBuf, wchar_t const * const data )
    {
		textBuf.nextLine( data );

        for ( unsigned int uiStrategy = 0; uiStrategy < NR_STRATEGIES; ++uiStrategy )
        {
            if ( ( *this )[ uiStrategy ] > 0 )
                textBuf.printFloat( ( *this )[ uiStrategy ] );
            else
                textBuf.printString( L"-" );
        }

        textBuf.printFloat( ( *this ).General( ) );
    };
};

class AllGenesStat
{
public:
    AllGenesStat( )  // constructor initializing all members to zero
    {
        m_auiMemSize.fill( 0 );
    };

    void scaleAllGenesStat( )
    {
        for ( auto & g : m_aGeneStat )
            g.DivNonZero( m_gsCounter.General( ) );

        m_gsAverageAge.DivNonZero( m_gsCounter );

        float fSum = 0;
        for ( auto & floatStat: m_axaGenePoolStrategy )
            fSum += floatStat.General( );
        
        for ( auto & floatStat : m_axaGenePoolStrategy )
            Scale( floatStat.General( ), fSum );
            
        for ( unsigned int uiStrategy = 0; uiStrategy < NR_STRATEGIES; ++uiStrategy )
        {
            float fSum = 0;
            for ( auto & floatStat : m_axaGenePoolStrategy )
                fSum += floatStat[ uiStrategy ];

            for ( auto & floatStat : m_axaGenePoolStrategy )
                Scale( floatStat[ uiStrategy ], fSum );
        }

        for ( unsigned int uiStrategy = 0; uiStrategy < NR_STRATEGIES; ++uiStrategy )
            DivNonZero( m_auiMemSize[ uiStrategy ], m_gsCounter[ uiStrategy ] );
    };

    void add2option( tStrategyId const s, unsigned int const uiAction, short const sValue )
    {
        m_axaGenePoolStrategy[ uiAction ].Add( static_cast<int>( s ), static_cast<float>( sValue ) );
    }

    void add2Gene( tStrategyId const s, unsigned int const uiGene, long const lGenoType )
    {
        assert( lGenoType >= 0 );
        m_aGeneStat[ uiGene ].Add( static_cast<int>( s ), static_cast<unsigned int>( lGenoType ) );
    }

    void addMemSize( tStrategyId const s, unsigned int const uiMemSize )
    {
        m_auiMemSize[ static_cast<int>( s ) ] += uiMemSize;
    }

    void addAge( tStrategyId const s, EVO_GENERATION genAge )
    {
        m_gsAverageAge.Add( static_cast<int>( s ), genAge.GetValue() );
    }

    void incCounter( tStrategyId const s )
    {
        m_gsCounter.Add( static_cast<int>( s ), 1 );
    }

    void printCounter( TextBuffer & textBuf, wchar_t const * const data )
    {
        m_gsCounter.printGeneLine( textBuf, data );
    }

    void printAvAge( TextBuffer & textBuf, wchar_t const * const data )
    {
        m_gsAverageAge.printGeneLine( textBuf, data );
    }

    void printCounters( TextBuffer & textBuf, EvolutionCore const * pCore, tAction const action )
    {
		int iCounterSum = 0;
		textBuf.nextLine( GetActionTypeName( action ) );
		for ( unsigned int uiStrategy = 0; uiStrategy < NR_STRATEGIES; ++uiStrategy )
		{
			int iCounter = pCore->GetActionCounter( uiStrategy, action );
			textBuf.printNumber( iCounter );
			iCounterSum += iCounter;
		}
		textBuf.printNumber( iCounterSum );
    }

    void printNrPassOn( TextBuffer & textBuf, EvolutionCore const * pCore )
    {
		if ( EvolutionCore::IsEnabled( tAction::passOn ) )
		{
			printCounters( textBuf, pCore, tAction::passOn );
		}
    }

    void printGeneStat( TextBuffer & textBuf, EvolutionCore const * pCore )
    {
        for ( unsigned int uiAction = 0; uiAction < NR_ACTION_GENES; ++uiAction )
		{
			tAction action = static_cast<tAction>( uiAction );
			if ( EvolutionCore::IsEnabled( action ) )
			{
				printCounters( textBuf, pCore, action );
				m_axaGenePoolStrategy[ uiAction ].printFloatLine( textBuf, L"" );
			}
		}

        for ( unsigned int uiGene = 0; uiGene < NR_GENES; ++uiGene )
		{
			tGeneType gene = static_cast<tGeneType>( uiGene );
			if ( EvolutionCore::IsEnabled( gene ) )
	            m_aGeneStat[ uiGene ].printGeneLine( textBuf, GetGeneName( gene ) );
		}
	}

    void printAvFood( TextBuffer & textBuf, wchar_t const * const data )
    {
        FloatStat fsAvFood;

		unsigned int uiEat      = static_cast<unsigned int>( tAction::eat );
		unsigned int uiAppetite = static_cast<unsigned int>(tGeneType::appetite);

        for ( unsigned int uiStrategy = 0; uiStrategy < NR_STRATEGIES; ++uiStrategy )
            fsAvFood[ uiStrategy ] = m_aGeneStat[ uiAppetite ][ uiStrategy ] * m_axaGenePoolStrategy[ uiEat ][ uiStrategy ] / 100;

        fsAvFood.General( ) = m_aGeneStat[ uiAppetite ].General( ) * m_axaGenePoolStrategy[ uiEat ].General( ) / 100 ;

        fsAvFood.printFloatLine( textBuf, data );
    }

    void printMemory( TextBuffer & textBuf, wchar_t const * const data )
    {
        textBuf.nextLine( data );

        for ( const auto & val : m_auiMemSize )
            textBuf.printNumber( val );
    }

private:
    GeneStat m_gsCounter;          // counter for strategies and sum counter 
    GeneStat m_gsAverageAge;       // average age of all individuals

    std::array < FloatStat,    NR_ACTION_GENES > m_axaGenePoolStrategy;
    std::array < unsigned int, NR_STRATEGIES   > m_auiMemSize;
    std::array < GeneStat,     NR_GENES        > m_aGeneStat;
};

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
    StartTextWindow( hwndParent, 200_PIXEL_X, 200_PIXEL_Y, 400_PIXEL_X, 545_PIXEL_Y, L"StatisticsWindow", 100, TRUE );
}

StatisticsWindow::~StatisticsWindow( )
{
	m_pCore = nullptr;
}

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
			{
				tStrategyId const s = m_pCore->GetStrategyId( gp );
				if ( s != tStrategyId::empty ) // can happen due to race conditions 
				{
					for ( unsigned int uiAction = 0; uiAction < NR_ACTION_GENES; ++uiAction )
						if ( EvolutionCore::IsEnabled( static_cast<tAction>( uiAction ) ) )
							genesStat.add2option( s, uiAction, m_pCore->GetDistr( gp, static_cast<tAction>( uiAction ) ) );

					for ( unsigned int uiGene = 0; uiGene < NR_GENES; ++uiGene )
						genesStat.add2Gene( s, uiGene, m_pCore->GetGenotype( gp, static_cast<tGeneType>( uiGene ) ) );

					genesStat.incCounter( s );
					genesStat.addMemSize( s, m_pCore->GetMemSize( gp ).GetValue() );
					genesStat.addAge    ( s, m_pCore->GetAge( gp ) );
				}
			}
		},
		m_pCore->GetSelection( )
	);

    genesStat.scaleAllGenesStat( );

    // start printing

    textBuf.setHorizontalPos( 2_TEXT_POSITION );                                 // header
    for ( unsigned int uiStrategy = 0; uiStrategy < NR_STRATEGIES; ++uiStrategy )
        textBuf.printString( GetStrategyName( (tStrategyId)uiStrategy ) );
    textBuf.printString( L"all" );                               

    genesStat.printCounter ( textBuf, L"#individuals" );  // number of individuals
    genesStat.printAvAge   ( textBuf, L"av. age" );       // average age
	genesStat.printNrPassOn( textBuf, m_pCore );
    genesStat.printGeneStat( textBuf, m_pCore );          // percentage numbers for options

	if ( EvolutionCore::IsEnabled( tAction::eat ) )
		genesStat.printAvFood( textBuf, L"av. food" );  // average food consumption 

	genesStat.printMemory( textBuf, L"memory" );        // memory size counters

    // maximum memory size

    textBuf.nextLine( L"max mem" );
    textBuf.setHorizontalPos( 4_TEXT_POSITION );
    textBuf.printNumber( EvolutionCore::GetMaxPartnerMemory( ) );

	if ( EvolutionCore::IsEnabled( tAction::interact ) )
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

	if ( EvolutionCore::IsEnabled( tAction::eat ) )
	{
		// average food growth

		textBuf.nextLine( L"food growth" );
		textBuf.setHorizontalPos( 4_TEXT_POSITION );
		textBuf.printNumber( m_pCore->GetAverageFoodGrowth( ) );
	}
//	stopwatch.Stop( L"Statistics" );
}
