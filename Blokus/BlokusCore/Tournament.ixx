// Tournament.ixx
//
// BlokusCore

export module BlokusCore:Tournament;

import std;
import Observable;
import IoConstants;
import :Player;
import :PlayerId;
import :Match;

using std::array;
using std::wcout;
using std::endl;

export class Tournament : public Observable
{
public:
    void Start(int const iNrOfMatches)
    {
        for (auto &it : m_wins)
            it = 0;
        m_iNrOfMatches = iNrOfMatches;
        m_iMatch = 1;
        m_active = true;
    }

    void MatchFinished(PlayerId const idWinner)
    {
        ++wins(idWinner);
        NotifyAll(true);
        if (++m_iMatch > m_iNrOfMatches)
        {
            m_active = false;
            m_iMatch = 0;
        }
    }

    bool Finished()    const { return m_iMatch >= m_iNrOfMatches; }
    bool Active()      const { return m_active; }
    int  MatchNumber() const { return m_iMatch; }
    int  NrOfMatches() const { return m_iNrOfMatches; }

    int  NrOfWins(PlayerId const id) const { return winsC(id); }

private:
    int m_iNrOfMatches { 0 };
    int m_iMatch       { 0 };
    bool m_active      { false };

    int const &winsC(PlayerId const id) const { return m_wins.at(id.GetValue()); }
    int       &wins (PlayerId const id)       { return m_wins.at(id.GetValue()); }

    array<int, NR_OF_PLAYERS> m_wins;
};