// Tournament.ixx
//
// BlokusCore

export module BlokusCore:Tournament;

import std;
import Observable;
import IoConstants;
import PerfCounter;
import :Player;
import :PlayerId;
import :Match;

using std::wstring;
using std::array;

export class Tournament : public Observable
{
public:
    void Start(int const);
    void NextMove();

    bool HasFinished() const { return m_iMatch >= m_iNrOfMatches; }
    bool IsActive()    const { return m_active; }
    int  MatchNumber() const { return m_iMatch; }
    int  NrOfMatches() const { return m_iNrOfMatches; }

    int            NrOfWins       (PlayerId const id) const { return winsC(id); }
    wstring const &GetStrategyName(PlayerId const)    const;
    Ticks          GetTimeUsed    (PlayerId const)    const; 
    Ticks          GetTournamentTime()                const; 

private:
    int   m_iNrOfMatches { 0 };
    int   m_iMatch       { 0 };
    bool  m_active       { false };
    Ticks m_ticksAtStart;
    Ticks m_ticksAtEnd;
	Match m_match;

    int const &winsC(PlayerId const id) const { return m_wins.at(id.GetValue()); }
    int       &wins (PlayerId const id)       { return m_wins.at(id.GetValue()); }

    array<int, NR_OF_PLAYERS> m_wins;
};