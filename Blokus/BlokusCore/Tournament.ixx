// Tournament.ixx
//
// BlokusCore
//
// Interface to TournamentWindow
//
// Starts Tournament either by 
// directly calling TournamentLoop
// or by delegation to TournamentThreadManager
//
// CPU cycles for tournaments are either taken from game loop (DoGameStuff)
// or happen in worker threads

export module BlokusCore:Tournament;

import std;
import HiResTimer;
import Observable;
import IoConstants;
import PerfCounter;
import :Match;
import :Player;
import :PlayerId;
import :Strategy;

using std::array;
using std::wstring;
using std::unique_ptr;

export class Tournament : public Observable
{
public:
    Tournament();

    void SetStrategy(PlayerId const, wstring const&);
    
    void Start(int const, int const = 0);
    void Stop() { m_active = false; }
    void NextTournamentMove();
    void NewResult(PlayerId const);

    bool IsActive()    const { return m_active; }
    int  MatchNumber() const { return m_iMatch; }
    int  NrOfMatches() const { return m_iNrOfMatches; }

    int  NrOfWins(PlayerId const id) { return winsC(id); }

    void DoGameStuff();

    wstring const &GetStrategyName(PlayerId const) const;
    Ticks          GetTimeUsed    (PlayerId const) const; 
    Ticks          GetTournamentTime()             const; 

private:
    HiResTimer m_timerMoves;

    unique_ptr<Match> m_upMatch; 
    int               m_iNrOfMatches   { 0 };
    int               m_iNrOfThreads   { 0 };
    int               m_iMatch         { 0 };
    bool              m_active         { false };
    PlayerId          m_idActivePlayer { 0 };
    PlayerId          m_idStartPlayer  { 0 };
    Ticks             m_ticksAtStart;
    Ticks             m_ticksAtEnd;

    array<wstring const*,       NR_OF_PLAYERS> m_selectedStrategyNames;
    array<unique_ptr<Strategy>, NR_OF_PLAYERS> m_selectedStrategies;

    array<int, NR_OF_PLAYERS> m_wins;

    int const &winsC(PlayerId const id) const { return m_wins.at(id.GetValue()); }
    int       &wins (PlayerId const id)       { return m_wins.at(id.GetValue()); }
};
