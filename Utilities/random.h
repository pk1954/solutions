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
    double       NextWeightedDistribution( void ); 

private:
    unsigned long m_ulRandomSeed;    // random generator
    bool          m_bValid;          // random generator for standard distribution
    double        m_dX2;             // random generator for standard distribution
};
