// HistoryGeneration.h
//

#pragma once

#include "assert.h"
#include "limits.h"
#include <iostream>
#include <iomanip>

class HIST_GENERATION
{
public:
    HIST_GENERATION( )              : m_nr( NULL_VAL ) {}
    HIST_GENERATION( long const l ) : m_nr( l ) {}

    void Set2Infinite( ) { m_nr = INFINITE_VAL; }

    bool IsInfinite  ( ) const { return m_nr == INFINITE_VAL; }
    bool IsDefined   ( ) const { return m_nr != NULL_VAL; }
    bool IsUndefined ( ) const { return m_nr == NULL_VAL; }

    HIST_GENERATION operator+ ( HIST_GENERATION const & a ) const { return m_nr + a.m_nr; }
    HIST_GENERATION operator- ( HIST_GENERATION const & a ) const { return m_nr - a.m_nr; }

    HIST_GENERATION operator++ ( ) { return ++m_nr; }
    HIST_GENERATION operator-- ( ) { return --m_nr; }

    HIST_GENERATION operator++ (int) { return m_nr++; }
    HIST_GENERATION operator-- (int) { return m_nr--; }

    HIST_GENERATION operator= ( long const l ) { return m_nr = l; }

    bool operator== ( HIST_GENERATION const & a ) const { return m_nr == a.m_nr; }
    bool operator!= ( HIST_GENERATION const & a ) const { return m_nr != a.m_nr; }
    bool operator>= ( HIST_GENERATION const & a ) const { return m_nr >= a.m_nr; }
    bool operator<= ( HIST_GENERATION const & a ) const { return m_nr <= a.m_nr; }
    bool operator>  ( HIST_GENERATION const & a ) const { return m_nr >  a.m_nr; }
    bool operator<  ( HIST_GENERATION const & a ) const { return m_nr <  a.m_nr; }

    long const GetLong( ) const { return m_nr; }

private:
    static long const INFINITE_VAL = LONG_MAX;
    static long const NULL_VAL     = LONG_MIN;

    long m_nr;
};

std::wostream & operator << ( std::wostream &, HIST_GENERATION const & );
std:: ostream & operator << ( std:: ostream &, HIST_GENERATION const & );
