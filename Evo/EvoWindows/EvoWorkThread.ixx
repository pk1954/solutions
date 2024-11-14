// EvoWorkThread.ixx
//
// EvoWindows

export module EvoWorkThread;

import Types;
import WinBasics;
import WorkThread;
import EvoCoreLib;
import HistoryLib;
import ColorManager;
import RootWindow;
import WinManager;
import HistoryLib;
import EvoHistGlueLib;
import EventInterface;

export class EvoWorkThreadMessage
{
public:
	enum class Id : UINT
	{
		DO_EDIT = WorkThreadMessage::FIRST_APP_MESSAGE,
		GOTO_ORIGIN,
		GOTO_DEATH,
		BENCHMARK,
		SET_BRUSH_RADIUS,
		SET_BRUSH_INTENSITY,
		SET_BRUSH_SHAPE,
		SET_BRUSH_OPERATOR,
		SET_BRUSH_MODE,
		SET_STRATEGY_COLOR,
		SET_SELECTION_COLOR,
		SET_HIGHLIGHT_COLOR,
		SET_POI,
		FIRST = DO_EDIT,
		LAST = SET_POI
	};

	static bool IsValid(EvoWorkThreadMessage::Id msg)
	{
		return (EvoWorkThreadMessage::Id::FIRST <= msg) && (msg <= EvoWorkThreadMessage::Id::LAST);
	}
};

export class EvoWorkThread: public WorkThread
{
public:
	EvoWorkThread
	(
		HWND                const,
		ColorManager      * const,
		EventInterface    * const,
		ObserverInterface * const,
		EvoHistorySysGlue * const
	);
	~EvoWorkThread();
	
	void Start
    (
		HWND                const,
		ColorManager      * const,
		EventInterface    * const,
		ObserverInterface * const, 
	    EvoHistorySysGlue * const
   );

    void PostDoEdit(GridPoint const);
    void PostSetPOI(GridPoint const);
    void PostSetBrushMode(tBrushMode const);
    void PostSetBrushShape(tShape);
	void PostSetBrushManipulator(tManipulator const);
    void PostSetBrushIntensity(PERCENT const);
    void PostSetColor(COLORREF const, tColorObject const, Strategy::Id const);
    void PostSetBrushRadius(GridCoord const);
	void PostBenchmark(int const);
	void PostGotoOrigin(GridPoint const);
	void PostGotoDeath (GridPoint const);

private:
	GenerationCmd EvoCmd(EvoGenerationCmd::Id const cmd, Int24 const param)
	{ 
		return GenerationCmd::ApplicationCmd(static_cast<GenerationCmd::Id>(cmd), param);  
	}  

	void editorCommand(EvoGenerationCmd::Id const cmd, WPARAM const wParam)
	{
		EditorCommand(static_cast<GenerationCmd::Id>(cmd), wParam);
	}

	void editorCommand(EvoGenerationCmd::Id const cmd, GridPoint24 const gp24)
	{
		GetHistorySystem()->CreateAppCommand(EvoCmd(cmd, gp24));
	}

	virtual bool Dispatch(MSG const);

	ColorManager      * m_pColorManager;
    EvoHistorySysGlue * m_pEvoHistGlue;
};
