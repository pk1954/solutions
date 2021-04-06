#pragma once

#include <limits.h>

class EVO_GENERATION
{
public:
    EVO_GENERATION() : m_nr( -1 ) {}

    EVO_GENERATION( long l ) : m_nr( l ) {}

    void Set2Infinite() { m_nr = LONG_MAX; }
    bool IsInfinite() const { return m_nr == LONG_MAX; }

    EVO_GENERATION operator+ ( EVO_GENERATION const & a ) const { return m_nr + a.m_nr; }
    EVO_GENERATION operator- ( EVO_GENERATION const & a ) const { return m_nr - a.m_nr; }

    EVO_GENERATION operator++ () { return ++m_nr; }
    EVO_GENERATION operator-- () { return --m_nr; }

    EVO_GENERATION operator= ( long l ) { return m_nr = l; }

    bool operator== ( EVO_GENERATION const & a ) const { return m_nr == a.m_nr; }
    bool operator!= ( EVO_GENERATION const & a ) const { return m_nr != a.m_nr; }
    bool operator>= ( EVO_GENERATION const & a ) const { return m_nr >= a.m_nr; }
    bool operator<= ( EVO_GENERATION const & a ) const { return m_nr <= a.m_nr; }
    bool operator>  ( EVO_GENERATION const & a ) const { return m_nr >  a.m_nr; }
    bool operator<  ( EVO_GENERATION const & a ) const { return m_nr <  a.m_nr; }

    bool operator>= ( long const & l ) const { return m_nr >= l; }
    bool operator<= ( long const & l ) const { return m_nr <= l; }
    bool operator>  ( long const & l ) const { return m_nr >  l; }
    bool operator<  ( long const & l ) const { return m_nr <  l; }

    long const GetLong() const { return m_nr; }

private:
    long m_nr;
};
