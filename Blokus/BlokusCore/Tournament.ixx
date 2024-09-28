// Tournament.ixx
//
// BlokusCore

export module BlokusCore:Tournament;

import std;
import IoConstants;
import :Player;
import :PlayerId;
import :Game;

using std::array;
using std::wcout;
using std::endl;

export class Tournament
{
public:
    void Start(int const iNrOfGames)
    {
        for (auto &it : m_wins)
            it = 0;

        Game game;
        for (int i = 0; i < iNrOfGames; ++i)
        {
            game.Initialize();
            while (game.NextPlayer()) {};
            PlayerId idWinner { game.WinnerId() };
            ++m_wins[idWinner.GetValue() - 1]; 
        }

        Apply2AllPlayerIds
        (
            [this, &game](PlayerId const id)
            {
                Player const &player { game.GetPlayerC(id) };
                wcout << m_wins[id.GetValue()-1] << SPACE << player.GetName() << endl;
            }
        );
    }

private:
    int m_iNrOfGames { 0 };

    array<int, NR_OF_PLAYERS> m_wins;
};