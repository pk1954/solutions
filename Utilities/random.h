// random.h : 
//

#pragma once

class Random
{
public:
    virtual ~Random( ) {};

    void         InitializeRandom( );
    unsigned int NextRandomNumber( void );
    bool         NextBooleanValue( void );

	unsigned int NextRandomNumberScaledTo( unsigned int const uiFactor )
	{
		unsigned int const uiRandom  = NextRandomNumber( );
		unsigned int const uiProduct = uiRandom * uiFactor;
		unsigned int const uiResult  = uiProduct >> 15;
		return uiResult;
	}

    double       NextWeightedDistribution( ); 

private:
    unsigned long m_ulRandomSeed;    // random generator
    bool          m_bValid;          // random generator for standard distribution
    double        m_dX2;             // random generator for standard distribution
};
