// Tournament.ixx
//
// BlokusCore

export module BlokusCore:Tournament;

import std;
import IoConstants;
import :Player;
import :PlayerId;
import :Match;

using std::array;
using std::wcout;
using std::endl;

export class Tournament
{
public:
    void Start(int const iNrOfMatchs)
    {
        for (auto &it : m_wins)
            it = 0;

        Match match;
        for (int i = 0; i < iNrOfMatchs; ++i)
        {
            match.Initialize();
            while (match.NextPlayer()) {};
            PlayerId idWinner { match.WinnerId() };
            ++m_wins[idWinner.GetValue() - 1]; 
        }

        Apply2AllPlayerIds
        (
            [this, &match](PlayerId const id)
            {
                Player const &player { match.GetPlayerC(id) };
                wcout << m_wins[id.GetValue()-1] << SPACE << player.GetName() << endl;
            }
        );
    }

private:
    int m_iNrOfMatchs { 0 };

    array<int, NR_OF_PLAYERS> m_wins;
};