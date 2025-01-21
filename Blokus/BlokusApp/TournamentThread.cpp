// TournamentThread.cpp
//
// BlokusApp

module TournamentThread;

bool TournamentThread::nextMove()
{
	//m_timerMoves.BeforeAction();
    BlokusMove move { m_match.SelectMove(m_idPlayer) };  // may finish if no more valid moves
    if (move.IsDefined())
    {
        m_match.DoMove(move);
    }
    if (m_match.GameHasFinished())
    {
        return false;
    }
    else // skip finished players, go to next player still in game
    {
        do
    	    m_idPlayer = NextPlayer(m_idPlayer);
        while (m_match.GetPlayerC(m_idPlayer).HasFinished());
    }
    return true;
    //m_timerMoves.AfterAction();
    //wcout << L"Tournament NextMove " << m_timerMoves.Average2wstring() << endl;
}
