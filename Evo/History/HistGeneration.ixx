// HistGeneration.ixx
//
// History

export module HistGeneration;

import std;

using std::wostream;
using std::ostream;

export class HistGeneration
{
public:
    HistGeneration() : m_nr(NULL_VAL) {}
    HistGeneration(long const l) : m_nr(l) {}

    void Set2Infinite() { m_nr = INFINITE_VAL; }

    bool IsInfinite () const { return m_nr == INFINITE_VAL; }
    bool IsDefined  () const { return m_nr != NULL_VAL; }
    bool IsUndefined() const { return m_nr == NULL_VAL; }

    HistGeneration operator+ (HistGeneration const& a) const { return m_nr + a.m_nr; }
    HistGeneration operator- (HistGeneration const& a) const { return m_nr - a.m_nr; }

    HistGeneration operator++ () { return ++m_nr; }
    HistGeneration operator-- () { return --m_nr; }

    HistGeneration operator++ (int) { return m_nr++; }
    HistGeneration operator-- (int) { return m_nr--; }

    HistGeneration operator= (long const l) { return m_nr = l; }

    bool operator== (HistGeneration const& a) const { return m_nr == a.m_nr; }
    bool operator!= (HistGeneration const& a) const { return m_nr != a.m_nr; }
    bool operator>= (HistGeneration const& a) const { return m_nr >= a.m_nr; }
    bool operator<= (HistGeneration const& a) const { return m_nr <= a.m_nr; }
    bool operator>  (HistGeneration const& a) const { return m_nr > a.m_nr; }
    bool operator<  (HistGeneration const& a) const { return m_nr < a.m_nr; }

    long const GetLong() const { return m_nr; }

private:
    static long const INFINITE_VAL { std::numeric_limits<long>::max() };
    static long const NULL_VAL     { std::numeric_limits<long>::min() };

    long m_nr;
};

export wostream& operator << (wostream&, HistGeneration const&);
export ostream & operator << (ostream &, HistGeneration const&);
