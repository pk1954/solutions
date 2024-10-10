// WorkThreadInterface.cpp
//
// EvoWindows

module WorkThreadInterface;

//import StopWatch;
import std;
import Debug;
import SaveCast;
import Resource;
import RunTime;
import Win32_Util_Resource;
import EventInterface;
import Thread;

using std::wostream;
using std::wcout;
using std::endl;

WorkThread::WorkThread
(
	HWND                  const hwndApplication,
	EventInterface      * const pEvent,
	ObserverInterface   * const pObserver,
	HistorySystem       * const pHistSystem,
	EvolutionCore       * const pModel,
	WorkThreadInterface * const pWorkThreadInterface,
	bool                  const bAsync
) :
	//m_pComputeTimer       (pActionTimer),
	m_pEventPOI           (pEvent),   
	m_pObserver           (pObserver),   
	m_pHistorySystem      (pHistSystem),
	m_pModel              (pModel),
	m_pWorkThreadInterface(pWorkThreadInterface),
	m_hwndApplication     (hwndApplication),
	m_bContinue           (FALSE),
	m_genDemanded         (0)
{
	Assert((m_pHistorySystem != nullptr) || (m_pModel != nullptr)); // one of them must be != nullptr
	Assert((m_pHistorySystem == nullptr) || (m_pModel == nullptr)); // one of them must be == nullptr
	StartThread(L"WorkerThread", bAsync); 
}

WorkThread::~WorkThread()
{
	m_hwndApplication      = nullptr;
	m_pWorkThreadInterface = nullptr;
	//m_pComputeTimer        = nullptr;
	m_pEventPOI            = nullptr;
	m_pObserver            = nullptr;
	m_pHistorySystem       = nullptr;
}

bool WorkThread::userWantsHistoryCut() const
{
	HistGeneration genCurrent  = m_pHistorySystem->GetCurrentGeneration();
	HistGeneration genYoungest = m_pHistorySystem->GetYoungestGeneration();
	std::wostringstream wBuffer;
	wBuffer << L"Gen " << (genCurrent + 1) << L" - " << genYoungest << L" will be deleted.";
	int iRes = MessageBox(nullptr, L"Cut off history?", wBuffer.str().c_str(), MB_OKCANCEL | MB_SYSTEMMODAL);
	return (iRes == IDOK);
}

// WorkMessage - process incoming messages from main thread

void WorkThread::WorkMessage
(
	bool                  const isEditOperation,
	WorkThreadMessage::Id const msg, 
	WPARAM                const wparam, 
	LPARAM                const lparam
)
{
	if (isEditOperation && m_pHistorySystem &&  m_pHistorySystem->IsInHistoryMode() )
	{
		if (userWantsHistoryCut())
		{
			m_pHistorySystem->ClearHistory(m_pHistorySystem->GetCurrentGeneration());  // cut off future generations
		}
		else 
		{
			return;            // user answered no, cancel operation
		}
	}

	PostThreadMsg(static_cast<UINT>(msg), wparam, lparam);
}

void WorkThread::ThreadStartupFunc() 
{ 
	SetThreadAffinityMask(0x0002);
}

void WorkThread::ThreadMsgDispatcher(MSG const msg )
{
	try
	{
		if (
			  WorkThread::Dispatch(msg)  // Give WorkThread a chance to handle message by itself. 
			  || Dispatch(msg)           // Otherwise hand over to application
		  )                              // If one of these could handle the message, ...
		{
			if (m_pObserver != nullptr)            // ... notify main thread, that model has changed.
				m_pObserver->Notify(m_bContinue);  // Continue immediately, if in run mode and
		}
		else  // Nobody could handle message
		{
			// Sometimes strange messages arrive. e.g. uiMsg 1847
	    }   // I cannot find a reason, so I ignore them
	}
	catch (HistoryBufferException e)
	{
		PostMessage(m_hwndApplication, WM_COMMAND, IDM_HIST_BUFFER_FULL, 0);
	}
}

bool WorkThread::Dispatch(MSG const msg)
{
	switch (static_cast<WorkThreadMessage::Id>(msg.message))
	{

	case WorkThreadMessage::Id::REDO_OPERATION:
		{
			HistGeneration genCurrent  = GetHistorySystem()->GetCurrentGeneration();
			HistGeneration genYoungest = GetHistorySystem()->GetYoungestGeneration();

			if ((genCurrent < genYoungest) && IsEditorCommand(genCurrent + 1))
				GotoGeneration(genCurrent + 1);
			else
				(void)MessageBeep(MB_OK);  // first generation reached
		}
		break;

	case WorkThreadMessage::Id::UNDO_OPERATION:
		{
			HistGeneration genCurrent = GetHistorySystem()->GetCurrentGeneration();

			if ((genCurrent > 0) && IsEditorCommand(genCurrent - 1))
				GotoGeneration(genCurrent - 1);
			else
				(void)MessageBeep(MB_OK);  // first generation reached
		}
		break;

	case WorkThreadMessage::Id::RESET_MODEL:
		GetHistorySystem()->CreateAppCommand(GenerationCmd::ResetCmd(Cast2UnsignedInt(msg.wParam)));
		if (static_cast<bool>(msg.wParam))
			GetHistorySystem()->ClearAllHistory();
		break;

	case WorkThreadMessage::Id::GENERATION_RUN:
		generationRun(static_cast<bool>(msg.lParam));
		break;

	case WorkThreadMessage::Id::STOP:
		generationStop();
		return FALSE;      // do not notify observers, because model has not changed  

	case WorkThreadMessage::Id::REPEAT_NEXT_GENERATION:
	    GotoGeneration(m_genDemanded);
		break;

	case WorkThreadMessage::Id::NEXT_GENERATION:
		if (m_pHistorySystem)
			GotoGeneration(m_pHistorySystem->GetCurrentGeneration() + 1);
		else
			Compute();  // compute next generation
		break;

	case WorkThreadMessage::Id::GOTO_GENERATION:
		GotoGeneration(HistGeneration(static_cast<long>(msg.lParam)));
		break;

	case WorkThreadMessage::Id::PREV_GENERATION:
		{
			Assert(m_pHistorySystem);

			HistGeneration gen = m_pHistorySystem->GetCurrentGeneration();

			if (gen > 0)
				GotoGeneration(gen - 1);
			else
				(void)MessageBeep(MB_OK);  // first generation reached
		}
		break;

	case WorkThreadMessage::Id::REFRESH:
		break;

	default:
		return FALSE; // could not handle message
	} 

	return TRUE; // message handled, all done
}

// GotoGeneration - perform one history step towards demanded generation
//                - update editor state if neccessary

void WorkThread::GotoGeneration(HistGeneration const gen)   
{
	Assert(m_pHistorySystem);

	m_genDemanded = gen;

	if (m_pHistorySystem->GetCurrentGeneration() != m_genDemanded)  // is something to do at all?
	{
		if (
				(m_pHistorySystem->GetCurrentGeneration() < m_genDemanded)  &&
				(m_pHistorySystem->GetCurrentGeneration() == m_pHistorySystem->GetYoungestGeneration())
			)     
		{                                                    // Normal case: Compute next generation
			//if (m_pComputeTimer != nullptr)
			//	m_pComputeTimer->TimerStart();               // prepare for time measurement

			m_pHistorySystem->CreateAppCommand(GenerationCmd::NextGenCmd());  //////// here the real work is done! ////////////////

			//if (m_pComputeTimer != nullptr)
			//	m_pComputeTimer->TimerStop();                     // measure computation time
		}
		else  // we are somewhere in history
		{
			m_pHistorySystem->ApproachHistGen(m_genDemanded);   // Get a stored generation from history system
		}

		WorkMessage(FALSE, WorkThreadMessage::Id::REFRESH, 0, 0);     // refresh all views
    
		if (m_pHistorySystem->GetCurrentGeneration() != m_genDemanded) // still not done?
			m_pWorkThreadInterface->PostRepeatGenerationStep();        // Loop! Will call indirectly GotoGeneration again
	}
}

void WorkThread::NGenerationSteps(int iNrOfGenerations)  // for benchmarks only
{
	//Stopwatch stopwatch;
	//stopwatch.Start();
	for (int i = 0; i < iNrOfGenerations; ++i)
	{
		if (m_pHistorySystem)
			m_pHistorySystem->CreateAppCommand(GenerationCmd::NextGenCmd());
		else
			Compute();  // compute next generation
		WorkMessage(FALSE, WorkThreadMessage::Id::REFRESH, 0, 0);   // refresh all views
	}
	//stopwatch.Stop(L"benchmark");
}

void WorkThread::generationRun(bool const bFirst)
{
	if (bFirst)               // if first RUN message ...
		setRunMode(TRUE);

	if (m_bContinue)
	{
		if (m_pHistorySystem)
			GotoGeneration(m_pHistorySystem->GetCurrentGeneration() + 1);
		else
			Compute();  // compute next generation

		//WaitTilNextActivation();

		//if (m_pWorkThreadInterface->IsAsyncThread())
		//{
		//	m_pWorkThreadInterface->PostRunGenerations(false);
		//}
		//else
		//{
			PostMessage(m_hwndApplication, WM_COMMAND, IDM_RUN, 0);
		//}
	}
}

void WorkThread::generationStop()
{
	if (m_pHistorySystem)
		m_genDemanded = m_pHistorySystem->GetCurrentGeneration();
	setRunMode(FALSE);
	RunTime::StopProcessing();
}

////////////////// Work thread interface /////////////////////////////

WorkThreadInterface::WorkThreadInterface() :
	m_pWorkThread (nullptr),
	m_pTraceStream(nullptr),
	m_bTrace      (TRUE)
{ 
}

WorkThreadInterface::~WorkThreadInterface()
{
	m_pWorkThread  = nullptr;
    m_pTraceStream = nullptr;
}

void WorkThreadInterface::Initialize(wostream * pTraceStream) 
{ 
	m_pTraceStream = pTraceStream;
}

void WorkThreadInterface::Start
(
	WorkThread * const pWorkThread
)
{
	m_pWorkThread = pWorkThread;
}

void WorkThreadInterface::Stop()
{
	m_pWorkThread->Terminate();
	m_pWorkThread = nullptr;
}

void WorkThreadInterface::postGotoGeneration(HistGeneration const gen)
{
    Assert(gen >= 0);

    m_pWorkThread->WorkMessage(FALSE, WorkThreadMessage::Id::GOTO_GENERATION, 0, static_cast<LPARAM>(gen.GetLong()));
}

// procedural interface of worker thread

void WorkThreadInterface::PostReset(bool bResetHistSys)
{
	if (IsTraceOn())
		* m_pTraceStream << __func__ << (bResetHistSys ? 1 : 0) << endl;
	WorkMessage(TRUE, static_cast<WorkThreadMessage::Id>(WorkThreadMessage::Id::RESET_MODEL), bResetHistSys, 0);
}

void WorkThreadInterface::PostGenerationStep()
{
    if (m_bTrace)
        * m_pTraceStream << __func__ << endl;

	m_pWorkThread->Continue();     // trigger worker thread if waiting on POI event
	
	m_pWorkThread->WorkMessage(FALSE, WorkThreadMessage::Id::NEXT_GENERATION, 0, 0);
}

void WorkThreadInterface::PostRunGenerations(bool const bFirst)
{
    //if (m_bTrace)
    //    * m_pTraceStream << L"PostGenerationStep" << endl;
	m_pWorkThread->WorkMessage(FALSE, WorkThreadMessage::Id::GENERATION_RUN, 0, bFirst);
}

void WorkThreadInterface::PostRepeatGenerationStep()
{
    //if (m_bTrace)
    //    * m_pTraceStream << L"PostGenerationStep" << endl;
    m_pWorkThread->WorkMessage(FALSE, WorkThreadMessage::Id::REPEAT_NEXT_GENERATION, 0, 0);
}

void WorkThreadInterface::PostRedo()
{
	if (IsTraceOn())
		TraceStream() << __func__ << endl;
	m_pWorkThread->WorkMessage(FALSE, WorkThreadMessage::Id::REDO_OPERATION, 0, 0);
}

void WorkThreadInterface::PostUndo()
{
	if (IsTraceOn())
		TraceStream() << __func__ << endl;
	m_pWorkThread->WorkMessage(FALSE, WorkThreadMessage::Id::UNDO_OPERATION, 0, 0);
}

void WorkThreadInterface::PostPrevGeneration()
{
    if (m_bTrace)
        * m_pTraceStream << __func__ << endl;

	m_pWorkThread->Continue();     // trigger worker thread if waiting on POI event

	m_pWorkThread->WorkMessage(FALSE, WorkThreadMessage::Id::PREV_GENERATION, 0, 0);
}

void WorkThreadInterface::PostGotoGeneration(HistGeneration const gen)
{
    if (m_bTrace)
        * m_pTraceStream << __func__ << L" " << gen << endl;
	postGotoGeneration(gen);
}

void WorkThreadInterface::PostStopComputation()
{
	m_pWorkThread->WorkMessage(FALSE, WorkThreadMessage::Id::STOP, 0, 0);
}
