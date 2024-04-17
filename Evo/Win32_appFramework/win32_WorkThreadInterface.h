// win32_workThreadInterface.h
//
// Win32_appFramework

module;

#include <fstream>
#include "win32_WorkThread.h"

import BoolOp;
import HistGeneration;

class WorkThreadInterface
{
public:
	WorkThreadInterface();
    ~WorkThreadInterface();

	void Initialize(std::wostream *);

	void Start(WorkThread * const);
	void Stop();

	void PostUndo();
	void PostRedo();
	void PostReset(bool);
	void PostRunGenerations(bool const);
	void PostStopComputation();
	void PostPrevGeneration();
	void PostGotoGeneration(HistGeneration const);
	void PostGenerationStep();
	void PostRepeatGenerationStep();       // Do not call! Used by WorkThread only;

	HistGeneration GetGenDemanded      () const { return m_pWorkThread->GetGenDemanded      (); }
	HistGeneration GetCurrentGeneration() const { return m_pWorkThread->GetCurrentGeneration(); }

	bool IsRunning    () const	{ return m_pWorkThread->IsRunning    (); }
	bool IsAsyncThread() const	{ return m_pWorkThread->IsAsyncThread(); }

protected:

	bool            IsTraceOn  () const { return   m_bTrace; }
	std::wostream & TraceStream()       { return * m_pTraceStream; }

	void WorkMessage
	(
		bool                  const isEditOperation,
		WorkThreadMessage::Id const msg,
		WPARAM                const wparam, 
		LPARAM                const lparam
	)
	{
		m_pWorkThread->WorkMessage(isEditOperation, msg, wparam, lparam);
	}

	void Continue() { m_pWorkThread->Continue(); }

private:
	void postGotoGeneration(HistGeneration const);

	WorkThread     * m_pWorkThread;
    std::wostream  * m_pTraceStream;
	bool             m_bTrace;
}; 
