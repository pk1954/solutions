// Players.ixx
//
// BlokusCore

export module BlokusCore:Players;

import std;
import Color;
import :Components;
import :Strategy;
import :BlokusCoords;
import :Player;
import :PlayerId;
import :PlayerTypes;
import :StrategyRandom;
import :StrategyTakeFirst;

using std::array;
using std::wstring;

StrategyTakeFirst StrategyRed;
StrategyTakeFirst    StrategyGreen;
StrategyTakeFirst    StrategyBlue;
StrategyRandom    StrategyYellow;

export class Players
{
public:
    void Reset()
    {
        m_players[0].Reset(PlayerTypes::GetPlayerType(PlayerId(0)), &StrategyRed);
        m_players[1].Reset(PlayerTypes::GetPlayerType(PlayerId(1)), &StrategyGreen); 
        m_players[2].Reset(PlayerTypes::GetPlayerType(PlayerId(2)), &StrategyBlue);
        m_players[3].Reset(PlayerTypes::GetPlayerType(PlayerId(3)), &StrategyYellow); 
    }

    void ResetTimers()
    {
        Apply2AllPlayers([](Player &player) { player.ResetTimer(); });
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
        return m_players.at(id.GetValue());
    }

    Player& GetPlayer(PlayerId const id)
    {
        return m_players.at(id.GetValue());
    }

private:

    array<Player, NR_OF_PLAYERS> m_players;
};