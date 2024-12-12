// Tournament.cpp
//
// BlokusCore

module BlokusCore:Tournament;

using std::wstring;
using std::array;
using std::wcout;
using std::endl;

void Tournament::Start(int const iNrOfMatches)
{
    m_ticksAtStart = PerfCounter::Read();
    for (auto &it : m_wins)
        it = 0;
    m_iNrOfMatches = iNrOfMatches;
    m_iMatch = 1;
    m_active = true;
    m_mwi.SetMatch(&m_match);
    m_mwi.Reset();
    m_mwi.ResetTimers();
}

void Tournament::NextTournamentMove()
{
	m_timer.BeforeAction();
    BlokusMove move { m_mwi.SelectMove() };  // may finish if no more valid moves
   if (move.IsDefined())
    {
        m_mwi.DoMove(move);
    }
    if (m_mwi.HasFinished())
    {
        PlayerId const idWinner { m_mwi.WinnerId() };
        ++wins(idWinner);
        m_mwi.Reset();
        if (++m_iMatch > m_iNrOfMatches)
        {
            m_active = false;
            m_iMatch = 0;
            m_ticksAtEnd = PerfCounter::Read();
        }
        NotifyAll(false);
    }
    else
    {
    	m_mwi.NextPlayer();
    }
    m_timer.AfterAction();
    wcout << L"Tournament NextMove " << m_timer.Average2wstring() << endl;
}

wstring const &Tournament::GetStrategyName(PlayerId const id) const 
{ 
    Player   const &player   { m_mwi.GetPlayerC(id) };
    Strategy const &strategy { player.GetStrategy() }; 
    return strategy.GetName();
}

Ticks Tournament::GetTournamentTime() const
{
    Ticks const ticks 
    { 
        m_active 
        ? PerfCounter::Read()
        : m_ticksAtEnd
    };
    return ticks - m_ticksAtStart; 
}

Ticks Tournament::GetTimeUsed(PlayerId const id) const 
{ 
    return m_mwi.GetPlayerC(id).GetTicks();
}
