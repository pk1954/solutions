// TournamentThreadManager.ixx
//
// BlokusApp

// Starts TournamentThraeds and
// receives results from them (ResultReceiver interface)
// Is called by Tournament class

export module TournamentThreadManager;

import std;
import ResultReceiver;
import BlokusCore;

using std::thread;
using std::vector;
using std::array;
using std::queue;
using std::mutex;
using std::condition_variable;

export class TournamentThreadManager : public ResultReceiver
{
public:
    void DeliverResult(PlayerId const) final;
    bool TournamentRunning()           final { return m_iMatchesPlayed >= m_iMatchesPlanned; }
    void CheckNewResults();

private:
    queue<PlayerId>    m_resultsQueue;
    mutex              m_queueMutex;
    condition_variable m_queueCV;
    vector<thread>     m_workerThreads;
    int                m_iMatchesPlayed  { 0 };
    int                m_iMatchesPlanned { 1000 };
};
