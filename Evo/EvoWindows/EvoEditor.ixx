// EvoEditor.ixx
//
// EvoWindows

export module EvoEditor;

import EvolutionTypes;
import EvoCoreLib;
import BaseDialog;
import EvoWorkThread;
import DspOptWindow;
import StatusBar;

export class EvoEditorWindow : public BaseDialog
{
public:
    EvoEditorWindow();
    ~EvoEditorWindow();

    void Start
	(
		HWND const, 
		EvoWorkThread * const, 
		EvoReadBuffer * const, 
		DspOptWindow  * const
	);

	void Stop();

	void    UpdateEditControls();
	LRESULT SendClick(int) const;


private:
    virtual bool UserProc(UINT const, WPARAM const, LPARAM const);

	void setBrushMode       (WORD const) const;
	void setBrushShape      (WORD const) const;
	void setBrushManipulator(WORD const) const;

	void updateOperationButtons(tBrushMode const) const;

	EvoReadBuffer * m_pReadBuffer;
    EvoWorkThread * m_pWorkThread;
    DspOptWindow  * m_pDspOptWindow;
};
