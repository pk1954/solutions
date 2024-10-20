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
    m_match.Reset();
    m_match.ResetTimers();
}

void Tournament::NextMove()
{
	m_timer.BeforeAction();
    BlokusMove move { m_match.NextMove() };
   	//m_timer.AfterAction();
    //wcout << L"m_match::NextMove() " << m_timer.Average2wstring() << endl;
    if (move.Defined())
    {
    	//m_timer.BeforeAction();
        m_match.FinishMove(move);
       	//m_timer.AfterAction();
        //wcout << L"m_match::FinishMove() " << m_timer.Average2wstring() << endl;
    }
    if (m_match.HasFinished())
    {
        PlayerId const idWinner { m_match.WinnerId() };
        ++wins(idWinner);
        m_match.Reset();
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
    	//m_timer.BeforeAction();
    	m_match.NextPlayer();
       	//m_timer.AfterAction();
        //wcout << L"m_match::NextPlayer() " << m_timer.Average2wstring() << endl;
    }
    m_timer.AfterAction();
    wcout << L"Tournament NextMove " << m_timer.Average2wstring() << endl;
}

wstring const &Tournament::GetStrategyName(PlayerId const id) const 
{ 
    Player   const &player   { m_match.GetPlayerC(id) };
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
    return m_match.GetPlayerC(id).GetTicks();
}
