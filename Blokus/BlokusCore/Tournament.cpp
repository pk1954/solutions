// Tournament.cpp
//
// BlokusCore

module BlokusCore:Tournament;

using std::wstring;
using std::array;
using std::wcout;
using std::endl;
using std::unique_ptr;
using std::make_unique;

import :StrategyHuman;
import :StrategyRandom;
import :StrategyTakeFirst;
import :StrategyBigFirstRandom;

unique_ptr<Strategy> CreateStrategy(wstring const &strategyName) 
{ 
    if (strategyName == StrategyHuman::NAME) 
        return make_unique<StrategyHuman>();
    else if (strategyName == StrategyRandom::NAME) 
        return make_unique<StrategyRandom>();
    else if (strategyName == StrategyTakeFirst::NAME) 
        return make_unique<StrategyTakeFirst>();
    else if (strategyName == StrategyBigFirstRandom::NAME) 
        return make_unique<StrategyBigFirstRandom>();
    return nullptr;
}

Tournament::Tournament()
{
    for (int i = 0; i < NR_OF_PLAYERS; ++i) 
        SetStrategy(PlayerId(i), StrategyRandom::NAME);
}

void Tournament::SetStrategy(PlayerId const id, wstring const& strategyName)
{
    m_selectedStrategyNames.at(id.GetValue()) = &strategyName;
}

void Tournament::Start
(
    int const iNrOfMatches,
    int const iNrOfThreads
)
{
    for (int i = 0; i < NR_OF_PLAYERS; ++i) 
        m_selectedStrategies[i] = CreateStrategy(*m_selectedStrategyNames[i]);
    m_upMatch = make_unique<Match>
    (
        m_selectedStrategies[0].get(),
        m_selectedStrategies[1].get(),
        m_selectedStrategies[2].get(),
        m_selectedStrategies[3].get()
    );
    m_ticksAtStart = PerfCounter::Read();
    m_iNrOfMatches = iNrOfMatches;
    m_iNrOfThreads = iNrOfThreads;
    m_iMatch = 1;
    m_active = true;
    m_upMatch->Reset();
    m_upMatch->ResetTimers();
    if (m_iNrOfThreads > 0)
    {
        // start worker threads 
    }
    else
    {
        // do computing here
    }
}

void Tournament::DoGameStuff()
{
    if (m_iNrOfThreads == 0)
    {
        NextTournamentMove();
    }
}

void Tournament::NewResult(PlayerId const idWinner)
{
    if (IsActive())
        ++(wins(idWinner));
    ++m_iMatch;
}

void Tournament::NextTournamentMove()
{
	m_timerMoves.BeforeAction();
    BlokusMove move { m_upMatch->SelectMove(m_idActivePlayer) };  // may finish if no more valid moves
    if (move.IsDefined())
    {
        m_upMatch->DoMove(move);
    }
    if (m_upMatch->GameHasFinished())
    {
        PlayerId const idWinner { m_upMatch->WinnerId() };
        NewResult(idWinner);
        m_upMatch->Reset();
        if (m_iMatch >= m_iNrOfMatches)  // Tournament finished?
        {
            m_active = false;
            m_iMatch = 0;
            m_ticksAtEnd = PerfCounter::Read();
        }
        else
        {
   	        m_idStartPlayer  = NextPlayer(m_idStartPlayer);
   	        m_idActivePlayer = m_idStartPlayer;
        }
    }
    else // skip finished players, go to next player still in game
    {
        do
    	    m_idActivePlayer = NextPlayer(m_idActivePlayer);
        while (m_upMatch->GetPlayerC(m_idActivePlayer).HasFinished());
    }
    m_timerMoves.AfterAction();
    //wcout << L"Tournament NextMove " << m_timerMoves.Average2wstring() << endl;
}

wstring const &Tournament::GetStrategyName(PlayerId const id) const 
{ 
    return *m_selectedStrategyNames.at(id.GetValue());
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
    return m_upMatch ? m_upMatch->GetPlayerC(id).GetTicks() : Ticks(0);
}
