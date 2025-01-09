// Tournament.cpp
//
// BlokusCore

module BlokusCore:Tournament;

using std::wstring;
using std::array;
using std::wcout;
using std::endl;
using std::make_unique;

void Tournament::Start
(
    Strategy * const pS1, 
    Strategy * const pS2, 
    Strategy * const pS3, 
    Strategy * const pS4,
    int        const iNrOfMatches
)
{
    m_ticksAtStart = PerfCounter::Read();
    for (auto &it : m_wins)
        it = 0;
    m_upMatch = make_unique<Match>(pS1, pS2, pS3, pS4);
    m_iNrOfMatches = iNrOfMatches;
    m_iMatch = 1;
    m_active = true;
    m_upMatch->Reset();
    m_upMatch->ResetTimers();
}

void Tournament::NextTournamentMove()
{
	m_timer.BeforeAction();
    BlokusMove move { m_upMatch->SelectMove(m_idPlayer) };  // may finish if no more valid moves
    if (move.IsDefined())
    {
        m_upMatch->DoMove(move);
    }
    if (m_upMatch->GameHasFinished())
    {
        PlayerId const idWinner { m_upMatch->WinnerId() };
        ++(wins(idWinner));
        m_upMatch->Reset();
        if (++m_iMatch > m_iNrOfMatches)
        {
            m_active = false;
            m_iMatch = 0;
            m_ticksAtEnd = PerfCounter::Read();
        }
        else
        {
   	        m_idStartPlayer = NextPlayer(m_idStartPlayer);
   	        m_idPlayer = m_idStartPlayer;
        }
        NotifyAll(false);
    }
    else // skip finished players, go to next player still in game
    {
        do
    	    m_idPlayer = NextPlayer(m_idPlayer);
        while (m_upMatch->GetPlayerC(m_idPlayer).HasFinished());
        m_upMatch->GetPlayerC(m_idPlayer).Prepare();
    }
    m_timer.AfterAction();
    wcout << L"Tournament NextMove " << m_timer.Average2wstring() << endl;
}

wstring const &Tournament::GetStrategyName(PlayerId const id) const 
{ 
    Player   const &player   { m_upMatch->GetPlayerC(id) };
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
    return m_upMatch->GetPlayerC(id).GetTicks();
}
