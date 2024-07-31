// win32_worker_thread.h
//
// Win32_appFramework

module;

//#include "util.h"
import GenerationCmd;
import ModelInterface;
import HistorySystem.h;
import Thread;
import HistGeneration;
import ActionTimer;
import EvoEditorWindow;
import RootWindow;
import WinManager;
import HistorySystem;
import EventInterface;
import ObserverInterface;
import WorkThreadInterface;

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

class WorkThread: public Util::Thread
{
public:
	WorkThread
	(
		HWND                  const,
		ActionTimer         * const,
		EventInterface      * const,
		ObserverInterface   * const,
		HistorySystem       * const,
		ModelInterface      * const,
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

	ActionTimer         * m_pComputeTimer;
    EventInterface      * m_pEventPOI;
	ObserverInterface   * m_pObserver;
    HistorySystem       * m_pHistorySystem;   // if HistorySystem is not used: nullptr 
	ModelInterface      * m_pModel;           // if HistorySystem is     used: nullptr
	WorkThreadInterface * m_pWorkThreadInterface;
    HistGeneration       m_genDemanded;
    bool                  m_bContinue;
	HWND                  m_hwndApplication;
};
