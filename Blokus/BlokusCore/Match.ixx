// Match.ixx
//
// BlokusCore

export module BlokusCore:Match;

import std;
import Types;
import Color;
import HiResTimer;
import :BlokusMove;
import :Board;
import :Components;
import :MatchProtocol;
import :Player;
import :PlayerId;
import :RuleServerInterface;
import :StrategyRandom;
import :StrategyTakeFirst;
import :StrategyHuman;

using std::array;
using std::vector;
using std::unique_ptr;
using std::make_unique;

StrategyHuman     StrategyRed;
StrategyTakeFirst StrategyGreen;
StrategyTakeFirst StrategyBlue;
StrategyRandom    StrategyYellow;

export using PLAYERS = array<Player, NR_OF_PLAYERS>;

export class Match
{
public:
    Match();

    static unique_ptr<Match> CreateNewMatch()
    {
        unique_ptr<Match> upMatch { make_unique<Match>() };
        return upMatch;
    }

    void Reset();
    void ResetTimers();

    Player       &GetPlayer (PlayerId const id)       { return m_players.at(id.GetValue()); }
    Player const &GetPlayerC(PlayerId const id) const { return m_players.at(id.GetValue()); }
    Player       &ActivePlayer ()                     { return GetPlayer (m_idActivePlayer); }
    Player const &ActivePlayerC()               const { return GetPlayerC(m_idActivePlayer); }

    PlayerId NextPlayer();
    PlayerId PrevPlayer();

    bool AnyShapeCellsBlocked(BlokusMove const&) const;

    PlayerId      m_idActivePlayer { 0 };
    PLAYERS       m_players;
    Board         m_board;
    MatchProtocol m_protocol;
    HiResTimer    m_timerFindContactPnts;
    HiResTimer    m_timer;
};
