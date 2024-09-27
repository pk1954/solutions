// GameResult.ixx
//
// BlokusCore

export module BlokusCore:GameResult;

import std;
import :PlayerId;

using std::array;

using POINTS = signed char;

using Ranking = array<PlayerId, NR_OF_PLAYERS>; 

export class GameResult
{
public:
    void Add
    (
        PlayerId const id,
        POINTS   const points
    )
    {
        m_results[id.GetValue() - 1] = points;
    }

    Ranking GetRanking()
    {

    }

private:

    array<POINTS, NR_OF_PLAYERS> m_results;
};