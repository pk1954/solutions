// Tournament.ixx
//
// BlokusCore

export module BlokusCore:Tournament;

import std;
import HiResTimer;
import Observable;
import IoConstants;
import PerfCounter;
import :MatchWriterInterface;
import :Player;
import :PlayerId;
import :Match;

using std::wstring;
using std::array;
using std::unique_ptr;

export class Tournament : public Observable
{
public:
    void Start(Strategy * const, Strategy * const, Strategy * const, Strategy * const, int const);
    void NextTournamentMove();

    bool TournamentHasFinished() const { return m_iMatch >= m_iNrOfMatches; }
    bool IsActive()              const { return m_active; }
    int  MatchNumber()           const { return m_iMatch; }
    int  NrOfMatches()           const { return m_iNrOfMatches; }

    int            NrOfWins       (PlayerId const id) const { return winsC(id); }
    wstring const &GetStrategyName(PlayerId const)    const;
    Ticks          GetTimeUsed    (PlayerId const)    const; 
    Ticks          GetTournamentTime()                const; 

private:
    HiResTimer m_timer;

    unique_ptr<Match> m_upMatch;
    int               m_iNrOfMatches   { 0 };
    int               m_iMatch         { 0 };
    PlayerId          m_idPlayer       { 0 };
    PlayerId          m_idStartPlayer  { 0 };
    bool              m_active         { false };
    PlayerId          m_idActivePlayer { 0 };
    Ticks             m_ticksAtStart;
    Ticks             m_ticksAtEnd;

    int const &winsC(PlayerId const id) const { return m_wins.at(id.GetValue()); }
    int       &wins (PlayerId const id)       { return m_wins.at(id.GetValue()); }

    array<int, NR_OF_PLAYERS> m_wins;
};