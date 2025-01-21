// TournamentThread.ixx
//
// BlokusApp

export module TournamentThread;

import std;
import HiResTimer;
import BlokusCore;
import ResultReceiver;

using std::unique_ptr;
using std::make_unique;

export class TournamentThread
{
public:

	TournamentThread
	(
		Match          &match,
		ResultReceiver &receiver
	)
		: m_match(match),
		  m_receiver(receiver)
	{
		//m_ticksAtStart = PerfCounter::Read();
		m_match.Reset();
		m_match.ResetTimers();
		while (m_receiver.TournamentRunning())
		{
			while (nextMove());
			m_receiver.DeliverResult(m_match.WinnerId());
			m_match.Reset();
			m_idStartPlayer = NextPlayer(m_idStartPlayer);  // next player may start
			m_idPlayer      = m_idStartPlayer;
		}
	}


private:
	HiResTimer      m_timer;
	ResultReceiver &m_receiver;
	Match          &m_match;
    PlayerId        m_idPlayer       { 0 };
    PlayerId        m_idStartPlayer  { 0 };

	bool nextMove();
};

		//m_timer.BeforeAction();
		//m_timer.AfterAction();
		//wcout << L"DoGameStuff " << m_timer.Average2wstring() << endl;
