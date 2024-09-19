// Game.ixx
//
// BlokusCore

export module BlokusCore:Game;

import std;
import Types;
import Color;
import :Player;
import :Components;

using std::array;

export using PlayerId = NamedType<int, struct PlayerId_Parameter>;

export inline PlayerId const NO_PLAYER { -1 };

export class Game
{
public:
    inline static Coord const MAX_COORD     { Components::BOARD_SIZE - 1 };
    inline static int   const NR_OF_PLAYERS { 4 };

    void Initialize()
    {
        for (Player &player : m_players)
            player.InitPieces();
    }

    Player const& ActivePlayer() const
    {
        return m_players[m_activePlayer.GetValue()];
    }

private:
    PlayerId m_activePlayer { 0 };

    Color const COL_RED    { Color(1.0f, 0.2f, 0.2f) };
    Color const COL_GREEN  { Color(0.0f, 1.0f, 0.0f) };
    Color const COL_BLUE   { Color(0.4f, 0.4f, 1.0f) };
    Color const COL_YELLOW { Color(0.8f, 0.8f, 0.0f) };

    array<Player, NR_OF_PLAYERS> m_players
    { 
        Player(CoordPos(  0_COORD,   0_COORD), COL_RED),
        Player(CoordPos(MAX_COORD,   0_COORD), COL_GREEN), 
        Player(CoordPos(MAX_COORD, MAX_COORD), COL_BLUE),
        Player(CoordPos(  0_COORD, MAX_COORD), COL_YELLOW) 
    };
};