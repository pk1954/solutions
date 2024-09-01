// EvoWorkThreadInterface.ixx
//
// EvoWindows

export module EvoWorkThreadInterface;

import std;
import BoolOp;
import EvoCoreLib;
import EvolutionTypes;
import ColorManager;
import ActionTimer;
import EventInterface;
import ObserverInterface;
import EvoHistorySysGlue;
import EvoWorkThread;
import WorkThreadInterface;

class EvoWorkThreadInterface : public WorkThreadInterface
{
public:
	EvoWorkThreadInterface();
    ~EvoWorkThreadInterface();

	void Start
    (
		HWND                const,
		ColorManager      * const,
		ActionTimer       * const,
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
	EvoWorkThread * m_pEvoWorkThread;
}; 
