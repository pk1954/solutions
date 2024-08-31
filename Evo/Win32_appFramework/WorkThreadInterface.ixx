// WorkThreadInterface.ixx
//
// Win32_appFramework

export module WorkThreadInterface;

import std;
import BoolOp;
import SaveCast;
import EventInterface;
import ObserverInterface;
import RootWindow;
import WinManager;
import Thread;
import WinBasics;
import HistoryLib;
import EvoCoreLib;

using std::wostream;

class WorkThreadMessage
{
public:
	enum class Id : UINT
	{
		REFRESH = WM_APP,
		STOP,
		REPEAT_NEXT_GENERATION,  // only used internally, not part of procedural interface
		GENERATION_RUN,
		GOTO_GENERATION,
		NEXT_GENERATION,
		PREV_GENERATION,
		UNDO_OPERATION,
		REDO_OPERATION,
		RESET_MODEL,
		FIRST = REFRESH,
		LAST = RESET_MODEL
	};

	static UINT const FIRST_APP_MESSAGE = static_cast<UINT>(Id::LAST) + 1;
};

class WorkThreadInterface;

class WorkThread: public Thread
{
public:
	WorkThread
	(
		HWND                  const,
		//ActionTimer         * const,   //TODO
		EventInterface      * const,
		ObserverInterface   * const,
		HistorySystem       * const,
		EvolutionCore       * const,
		WorkThreadInterface * const,
		bool                  const
	);
	~WorkThread();

	virtual void ThreadStartupFunc();
	virtual void ThreadMsgDispatcher(MSG const);
	
	// WorkMessage - process incoming messages from main thread

	void WorkMessage(bool const, WorkThreadMessage::Id const, WPARAM const, LPARAM const);

	void NGenerationSteps(int); 

	HistGeneration GetGenDemanded() const 
	{ 
		return m_genDemanded; 
	}

	bool IsRunning() const
	{
		return m_bContinue;
	}

	void Continue()
	{
		if (m_pEventPOI != nullptr)
			m_pEventPOI->Continue();     // trigger worker thread if waiting on POI event
	}

	bool IsEditorCommand(HistGeneration const gen)
	{
		return GetHistorySystem()->GetGenerationCmd(gen).IsAppCommand();
	}

	HistGeneration GetCurrentGeneration()
	{
		return GetHistorySystem()->GetCurrentGeneration();
	}

protected:
	virtual bool Dispatch(MSG const);

	void GotoGeneration(HistGeneration const);

	HistorySystem * GetHistorySystem() { return m_pHistorySystem; }

	void EditorCommand(GenerationCmd::Id const cmd, WPARAM const wParam)
	{
		GetHistorySystem()->CreateAppCommand(makeGenerationCmd(cmd, Int24(Cast2UnsignedInt(wParam))));
	}

	virtual void Compute() { m_pModel->Compute(); }

	virtual void WaitTilNextActivation() = 0;

	virtual void SetRunModeHook(bool const) {};  // hook for application when run mode starts/stops

private:

	void generationRun(bool const);
	void generationStop();

	void setRunMode(bool const bState)
	{
		m_bContinue = bState;
		SetRunModeHook(bState);
	}

	GenerationCmd makeGenerationCmd(GenerationCmd::Id const cmd, Int24 const param)
	{ 
		return GenerationCmd::ApplicationCmd(static_cast<GenerationCmd::Id>(cmd), param);  
	}  

	bool userWantsHistoryCut() const;

	//ActionTimer         * m_pComputeTimer;   //TODO
    EventInterface      * m_pEventPOI;
	ObserverInterface   * m_pObserver;
    HistorySystem       * m_pHistorySystem;   // if HistorySystem is not used: nullptr 
	EvolutionCore       * m_pModel;           // if HistorySystem is     used: nullptr
	WorkThreadInterface * m_pWorkThreadInterface;
    HistGeneration        m_genDemanded;
    bool                  m_bContinue;
	HWND                  m_hwndApplication;
};

export class WorkThreadInterface
{
public:
	WorkThreadInterface();
    ~WorkThreadInterface();

	void Initialize(wostream *);

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
	bool IsAsyncThread() const	{ return true; }

protected:

	bool       IsTraceOn  () const { return   m_bTrace; }
	wostream & TraceStream()       { return * m_pTraceStream; }

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

	WorkThread * m_pWorkThread;
    wostream   * m_pTraceStream;
	bool         m_bTrace;
}; 
