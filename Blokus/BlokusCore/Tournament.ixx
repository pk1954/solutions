// Tournament.ixx
//
// BlokusCore

export module BlokusCore:Tournament;

import std;
import :PlayerId;
import :Game;

using std::array;

export class Tournament
{
public:
    void Start(int const iNrOfGames)
    {
        for (auto &it : m_winners)
            it = 0;

        Game game;
        for (int i = 0; i < iNrOfGames; ++i)
        {
            game.Initialize();
            while (game.NextPlayer()) {};
            PlayerId idWinner { game.WinnerId() };
            ++m_winners[idWinner.GetValue() - 1]; 
        }
    }

private:
    int m_iNrOfGames { 0 };

    array<int, NR_OF_PLAYERS> m_winners;
};