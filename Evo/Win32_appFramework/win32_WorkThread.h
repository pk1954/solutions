// win32_worker_thread.h
//
// Win32_appFramework

module;

#include "util.h"
#include "GenerationCmd.h"
#include "ModelInterface.h"
#include "HistorySystem.h"
import HistGeneration;
#include "win32_thread.h"

class ActionTimer;
class EvoEditorWindow;
class RootWindow;
class WinManager;
class HistorySystem;
class EventInterface;
class ObserverInterface;
class WorkThreadInterface;

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
		BOOL                  const
	);
	~WorkThread();

	virtual void ThreadStartupFunc();
	virtual void ThreadMsgDispatcher(MSG const);
	
	// WorkMessage - process incoming messages from main thread

	void WorkMessage(BOOL const, WorkThreadMessage::Id const, WPARAM const, LPARAM const);

	void NGenerationSteps(int); 

	HistGeneration GetGenDemanded() const 
	{ 
		return m_genDemanded; 
	}

	BOOL IsRunning() const
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
	virtual BOOL Dispatch(MSG const);

	void GotoGeneration(HistGeneration const);

	HistorySystem * GetHistorySystem() { return m_pHistorySystem; }

	void EditorCommand(GenerationCmd::Id const cmd, WPARAM const wParam)
	{
		GetHistorySystem()->CreateAppCommand(makeGenerationCmd(cmd, Int24(Cast2UnsignedInt(wParam))));
	}

	virtual void Compute() { m_pModel->Compute(); }

	virtual void WaitTilNextActivation() = 0;

	virtual void SetRunModeHook(BOOL const) {};  // hook for application when run mode starts/stops

private:

	void generationRun(bool const);
	void generationStop();

	void setRunMode(BOOL const bState)
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
    BOOL                  m_bContinue;
	HWND                  m_hwndApplication;
};
