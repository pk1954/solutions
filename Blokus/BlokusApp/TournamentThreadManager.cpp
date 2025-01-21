// TournamentThreadManager.cpp
//
// BlokusApp

module TournamentThreadManager;

import std;

using std::queue;
using std::mutex;
using std::lock_guard;
using std::unique_lock;
using std::condition_variable;
using std::chrono::milliseconds;


    //for (auto &it : m_wins)
    //    it = 0;
    //// Start multiple worker threads
    //for (int i = 0; i < NR_OF_THREADS; ++i) 
    //    m_workerThreads.emplace_back(TournamentThread(m_receiver, pS1, pS2, pS3, pS4), i); 

    //// Wait for all worker threads to finish
    //for (auto& thread : m_workerThreads)
    //    thread.join();

    //m_workerThreads.clear();

void TournamentThreadManager::DeliverResult(PlayerId const id)
{
    lock_guard<mutex>lock(m_queueMutex);
    m_resultsQueue.push(id);
    m_queueCV.notify_one(); // Notify receiver
}

void TournamentThreadManager::CheckNewResults()
{
    unique_lock<mutex> lock(m_queueMutex);
    m_queueCV.wait_for(lock, milliseconds(0), [this] { return !m_resultsQueue.empty(); }); // Wait for new results
    while (!m_resultsQueue.empty()) 
    {
        PlayerId idWinner = m_resultsQueue.front();
        m_resultsQueue.pop();
        // tournament.NewResult(idWinner);
    }
}
