// INdividualId.h
//

#pragma once

#include <sstream>

//lint -sem(IndividualId::ResetIndId,initializer)

class IndividualId
{
public:
	static const IndividualId NO_INDIVIDUAL;

    IndividualId( ) { ResetIndId( ); };

    inline IndividualId & operator++() { ++m_lIdentifier; return *this; }; 
    inline bool           operator==( IndividualId const & id ) const { return (id.m_lIdentifier == m_lIdentifier); };

    inline bool IsDefined( ) const 
    { 
        return (m_lIdentifier != ID_NO_INDIVIDUAL); 
    };
    
    inline void ResetIndId( )       
    {
        m_lIdentifier = ID_NO_INDIVIDUAL;  
    };

    inline long GetLong( )   const { return m_lIdentifier; };

private:
    static long const ID_NO_INDIVIDUAL = -1;

    long m_lIdentifier;
};

std::wostream & operator << ( std::wostream & , IndividualId const & );  // implemented in dump.cpp
