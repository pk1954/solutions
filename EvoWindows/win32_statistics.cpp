// win32_statistics.cpp :
//

#include "stdafx.h"
#include "Strsafe.h"
#include "XArray.h"
#include "gridRect.h"
#include "EvolutionModelData.h"
#include "EvolutionCore.h"
#include "win32_baseWindow.h"
#include "win32_statistics.h"

using namespace std;

class GeneStat: public XArray< unsigned int, NR_STRATEGIES  >
{
public:

    void printGeneLine( TextWindow & textWin, wchar_t const * const data )
    {
        textWin.nextLine( data );

        for ( unsigned int uiStrategy = 0; uiStrategy < NR_STRATEGIES; ++uiStrategy )
            textWin.printNumber( ( *this )[ uiStrategy ] );

        textWin.printNumber( General( ) );
    };
};

class FloatStat : public XArray< float, NR_STRATEGIES  >
{
public:

    void printFloatLine( TextWindow & textWin, wchar_t const * const data )
    {
        textWin.nextLine( data );

        for ( unsigned int uiStrategy = 0; uiStrategy < NR_STRATEGIES; ++uiStrategy )
        {
            if ( ( *this )[ uiStrategy ] > 0 )
                textWin.printFloat( ( *this )[ uiStrategy ] );
            else
                textWin.printString( L"-" );
        }

        textWin.printFloat( ( *this ).General( ) );
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

    void add2option( tStrategyId const s, unsigned int const uiOption, short const sValue )
    {
        m_axaGenePoolStrategy[ uiOption ].Add( static_cast<int>( s ), static_cast<float>( sValue ) );
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
        m_gsAverageAge.Add( static_cast<int>( s ), genAge );
    }

    void incCounter( tStrategyId const s )
    {
        m_gsCounter.Add( static_cast<int>( s ), 1 );
    }

    void printCounter( TextWindow & textWin, wchar_t const * const data )
    {
        m_gsCounter.printGeneLine( textWin, data );
    }

    void printAvAge( TextWindow & textWin, wchar_t const * const data )
    {
        m_gsAverageAge.printGeneLine( textWin, data );
    }

    void printGeneStat( TextWindow & textWin )
    {
        for ( unsigned int uiOption = 0; uiOption < NR_ACTIONS; ++uiOption )
		{
			tAction action = static_cast<tAction>( uiOption );
			if ( EvolutionCore::IsEnabled( action ) )
				m_axaGenePoolStrategy[ uiOption ].printFloatLine( textWin, GetActionTypeName( action ) );
		}

        for ( unsigned int uiGene = 0; uiGene < NR_GENES; ++uiGene )
		{
			tGeneType gene = static_cast<tGeneType>( uiGene );
			if ( EvolutionCore::IsEnabled( gene ) )
	            m_aGeneStat[ uiGene ].printGeneLine( textWin, GetGeneName( gene ) );
		}
    }

    void printAvFood( TextWindow & textWin, wchar_t const * const data )
    {
        FloatStat fsAvFood;

        for ( unsigned int uiStrategy = 0; uiStrategy < NR_STRATEGIES; ++uiStrategy )
            fsAvFood[ uiStrategy ] = m_aGeneStat[ static_cast<int>( tGeneType::appetite ) ][ uiStrategy ] * m_axaGenePoolStrategy[ static_cast<int>( tAction::eat ) ][ uiStrategy ] / 100;

        fsAvFood.General( ) = m_aGeneStat[ static_cast<int>( tGeneType::appetite ) ].General( ) * m_axaGenePoolStrategy[ static_cast<int>( tAction::eat ) ].General( ) / 100 ;

        fsAvFood.printFloatLine( textWin, data );
    }

    void printMemory( TextWindow & textWin, wchar_t const * const data )
    {
        textWin.nextLine( data );

        for ( const auto & val : m_auiMemSize )
            textWin.printNumber( val );
    }

private:
    GeneStat m_gsCounter;          // counter for strategies and sum counter 
    GeneStat m_gsAverageAge;       // average age of all individuals

    array < FloatStat,    NR_ACTIONS    > m_axaGenePoolStrategy;
    array < unsigned int, NR_STRATEGIES > m_auiMemSize;
    array < GeneStat,     NR_GENES      > m_aGeneStat;
};

StatisticsWindow::StatisticsWindow( ):
    TextWindow( )
{ }

void StatisticsWindow::Start
(
    HWND                       const hWndParent,
    EvolutionModelData const * const pModel
) 
{
    HWND hWnd = StartTextWindow( hWndParent, L"StatisticsWindow", 100 );
    Move( 200, 200, 400, 430, TRUE );
    m_pModelWork = pModel;
}

StatisticsWindow::~StatisticsWindow( )
{
	m_pModelWork = nullptr;
}

void StatisticsWindow::DoPaint( )
{
    // aquire and prepare data 

    AllGenesStat genesStat;
	GridRect     gridRectSel = m_pModelWork->GetSelection( );

    Apply2Rect
	( 
		[&](GridPoint const & gp, short const s)
		{
			if ( m_pModelWork->IsAlive( gp ) )
			{
				tStrategyId const s = m_pModelWork->GetStrategyId( gp );
				for ( unsigned int uiOption = 0; uiOption < NR_ACTIONS; ++uiOption )
					if ( EvolutionCore::IsEnabled( static_cast<tAction>( uiOption ) ) )
						genesStat.add2option( s, uiOption, m_pModelWork->GetDistr( gp, static_cast<tAction>( uiOption ) ) );

				for ( unsigned int uiGene = 0; uiGene < NR_GENES; ++uiGene )
					genesStat.add2Gene( s, uiGene, m_pModelWork->GetGenotype( gp, static_cast<tGeneType>( uiGene ) ) );

				genesStat.incCounter( s );
				genesStat.addMemSize( s, m_pModelWork->GetMemSize( gp ) );
				genesStat.addAge    ( s, m_pModelWork->GetAge( gp ) );
			}
		},
		gridRectSel.IsEmpty( ) ? GridRect::GRID_RECT_FULL : gridRectSel
	);

    genesStat.scaleAllGenesStat( );

    // start printing

    setHorizontalPos( 2 );                                 // header
    for ( unsigned int uiStrategy = 0; uiStrategy < NR_STRATEGIES; ++uiStrategy )
        printString( GetStrategyName( (tStrategyId)uiStrategy ) );
    printString( L"all" );                               

    genesStat.printCounter ( * this, L"#individuals" );  // number of individuals
    genesStat.printAvAge   ( * this, L"av. age" );       // average age
    genesStat.printGeneStat( * this );                   // percentage numbers for options

	if ( EvolutionCore::IsEnabled( tAction::eat ) )
		genesStat.printAvFood  ( * this, L"av. food" );      // average food consumption 

	genesStat.printMemory  ( * this, L"memory" );        // memory size counters

    // maximum memory size

    nextLine( L"max mem" );
    setHorizontalPos( 4 );
    printNumber( EvolutionCore::GetMaxPartnerMemory( ) );

	if ( EvolutionCore::IsEnabled( tAction::interact ) )
	{
		// nr of interactions with known culprits (Tit4Tat only)

		nextLine( L"known" );
		setHorizontalPos( 4 );
		printNumber( EvolutionCore::GetNrInteractionsWithKnownCulprit( ) );

		// nr of interactions with unknown culprits (Tit4Tat only)

		nextLine( L"unknown" );
		setHorizontalPos( 4 );
		printNumber( EvolutionCore::GetNrInteractionsWithUnknownCulprit( ) );
	}

	if ( EvolutionCore::IsEnabled( tAction::eat ) )
	{
		// average food growth

		nextLine( L"food growth" );
		setHorizontalPos( 4 );
		printNumber( m_pModelWork->GetAverageFoodGrowth( ) );
	}
}
