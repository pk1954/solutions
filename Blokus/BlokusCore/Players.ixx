// Players.ixx
//
// BlokusCore

export module BlokusCore:Players;

import std;
import Color;
import :Components;
import :CoordPos;
import :PlayerId;
import :Player;

using std::array;
using std::wstring;

class Players
{
public:
    void Initialize()
    {
        m_players[0].Initialize(CoordPos(  0_COORD,   0_COORD), COL_RED,    L"RED"   );
        m_players[1].Initialize(CoordPos(MAX_COORD,   0_COORD), COL_GREEN,  L"GREEN" ); 
        m_players[2].Initialize(CoordPos(MAX_COORD, MAX_COORD), COL_BLUE,   L"BLUE"  );
        m_players[3].Initialize(CoordPos(  0_COORD, MAX_COORD), COL_YELLOW, L"YELLOW"); 
    }

    void Apply2AllPlayersC(auto const& func) const
    {
        for (Player const& player: m_players)
            func(player);
    }

    void Apply2AllPlayers(auto const& func)
    {
        for (Player &player: m_players)
            func(player);
    }

    Player const& GetPlayerC(PlayerId const id) const
    {
        return m_players[id.GetValue()];
    }

    Player& GetPlayer(PlayerId const id)
    {
        return m_players[id.GetValue()];
    }

private:

    Color const COL_RED    { Color(1.0f, 0.2f, 0.2f) };
    Color const COL_GREEN  { Color(0.0f, 1.0f, 0.0f) };
    Color const COL_BLUE   { Color(0.4f, 0.4f, 1.0f) };
    Color const COL_YELLOW { Color(0.8f, 0.8f, 0.0f) };

    array<Player, NR_OF_PLAYERS> m_players;
};