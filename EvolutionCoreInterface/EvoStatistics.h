// EvoStatistics.h :
//
// EvolutionCoreInterface

#pragma once

#include "XArray.h"
#include "gridRect.h"
#include "TextBuffer.h"
#include "EvolutionCore.h"
#include "EvolutionTypes.h"

class XaCounter: public XArray< unsigned int, Strategy::COUNT  >
{
public:

    void printGeneLine( TextBuffer * pTextBuf, wchar_t const * const data )
    {
        pTextBuf->nextLine( data );
 		Apply2All( [&](unsigned int elem) { pTextBuf->printNumber( elem ); } );
    };
};

class XaFloatStat : public XArray< float, Strategy::COUNT >
{
public:

    void printFloatLine( TextBuffer * pTextBuf, wchar_t const * const data )
    {
		pTextBuf->nextLine( data );
 		Apply2All( [&](float elem) { pTextBuf->printFloat( elem ); } );
    };
};

class EvoStatistics
{
public:
	EvoStatistics::EvoStatistics( EvolutionCore const * const );
	virtual ~EvoStatistics( );

    void Initialize( TextBuffer * const ); 

    void printHeader( );
    void printIncidence( );
    void printProbabilities( );
    void printGeneStat(  );
    void printCounter( wchar_t const * const data );
    void printAvAge  ( wchar_t const * const data );
    void printAvFood ( wchar_t const * const data );
    void printMemory ( wchar_t const * const data );
    void printCounters( Action::Id const action );

private:
	void aquireData( GridPoint const & );
	void scale( );

    void add2option( Strategy::Id const, Action::Id const, short const );
    void add2Gene  ( Strategy::Id const, GeneType::Id const, long const );
    void addMemSize( Strategy::Id const, MEM_INDEX const );
    void addAge    ( Strategy::Id const, EVO_GENERATION );
    void incCounter( Strategy::Id const );

	EvolutionCore const * m_pCore;
	TextBuffer          * m_pTextBuf;

    XaCounter m_gsCounter;          // counter for strategies and sum counter 
    XaCounter m_gsAverageAge;       // average age of all individuals

	ActionArray< XaFloatStat > m_axaGenePoolStrategy;
    GeneTypeArray< XaCounter > m_aGeneStat;
	StrategyArray<unsigned int> m_auiMemSize;
};
