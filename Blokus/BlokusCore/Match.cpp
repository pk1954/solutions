// Match.cpp
//
// BlokusCore

module BlokusCore:Match;

import PerfCounter;
import :BlokusPreferences;

using std::wcout;
using std::endl;
using std::vector;

Match::Match()
{
    m_players[0].Initialize(m_board, PlayerId(0), &StrategyRed);
    m_players[1].Initialize(m_board, PlayerId(1), &StrategyGreen); 
    m_players[2].Initialize(m_board, PlayerId(2), &StrategyBlue);
    m_players[3].Initialize(m_board, PlayerId(3), &StrategyYellow); 
    Reset();
}

void Match::Reset()
{
    m_board.Reset();
    for (Player &player: m_players)
        player.Reset(); 
    m_protocol.Reset();
    m_idActivePlayer = FIRST_PLAYER;
    m_timer.Reset();
}

void Match::ResetTimers()
{
    for (Player &player: m_players)
        player.ResetTimer(); 
}

PlayerId Match::NextPlayer()
{
    if (++m_idActivePlayer > LAST_PLAYER)
        m_idActivePlayer = FIRST_PLAYER;    
    ActivePlayer().Prepare();              // Prepare for selection of next move
    return m_idActivePlayer;
}

PlayerId Match::PrevPlayer()
{
    if (--m_idActivePlayer <  FIRST_PLAYER)
        m_idActivePlayer = LAST_PLAYER;    
    return m_idActivePlayer;
}

bool Match::AnyShapeCellsBlocked(BlokusMove const& move) const
{
	Player const &player { GetPlayerC(move.GetPlayerId()) };
    return player.AnyShapeCellsBlocked(move);
}
