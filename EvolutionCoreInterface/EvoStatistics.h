// EvoStatistics.h :
//
// EvolutionCoreInterface

#pragma once

#include "XArray.h"
#include "EnumArray.h"
#include "TextBuffer.h"
#include "EvolutionCore.h"
#include "EvolutionTypes.h"

class XaCounter: public XArray< unsigned int, Strategy::COUNT  >
{
public:
    unsigned int & operator[] ( Strategy::Id strategy ) 
	{ 
		return XArray::operator[](static_cast<unsigned int>( strategy ));
	}

	void operator= (unsigned int const op)
	{
		Apply2XArray( [&](unsigned int elem) { elem = op; } );
	}

    void Add( Strategy::Id strategy, unsigned int const op )
    {
		XArray::Add( static_cast<unsigned int>(strategy), op );
	}

    void printGeneLine( TextBuffer * pTextBuf, wchar_t const * const data )
    {
        pTextBuf->nextLine( data );
 		Apply2XArray( [&](unsigned int elem) { pTextBuf->printNumber( elem ); } );
    };
};

class XaFloatStat : public XArray< float, Strategy::COUNT >
{
public:
    float & operator[] ( Strategy::Id strategy ) 
	{ 
		return XArray::operator[](static_cast<unsigned int>( strategy ));
	}

	void operator= (float const op)
	{
		Apply2XArray( [&](float elem) { elem = op; } );
	}

    void Add( Strategy::Id strategy, float const op )
    {
		XArray::Add( static_cast<unsigned int>(strategy), op );
	}

    void printFloatLine( TextBuffer * pTextBuf, wchar_t const * const data )
    {
		pTextBuf->nextLine( data );
 		Apply2XArray( [&](float elem) { pTextBuf->printFloat( elem ); } );
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
	void scaleData( );
	void scale( float &, float const );

	EvolutionCore const * m_pCore;
	TextBuffer          * m_pTextBuf;

    XaCounter m_gsCounter;          // counter for strategies and sum counter 
    XaCounter m_gsAverageAge;       // average age of all individuals

	EnumArray<XaFloatStat,  Action >  m_XaAction;
	EnumArray<XaCounter,    GeneType> m_XaGenes;
	EnumArray<unsigned int, Strategy> m_auiMemSize;
};
