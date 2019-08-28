// win32_editor.h 
//

#pragma once

#include "ViewCollection.h"
#include "EvolutionTypes.h"
#include "win32_baseDialog.h"

class WorkThreadInterface;
class DspOptWindow;
class ReadBuffer;
class StatusBar;

class EditorWindow : public BaseDialog
{
public:
    EditorWindow( );
    ~EditorWindow( );

    void Start
	( 
		HWND const, 
		WorkThreadInterface * const, 
		ReadBuffer          * const, 
		DspOptWindow        * const
	);

	void Stop( );

	void    UpdateEditControls( );
	LRESULT SendClick( int ) const;

	void RegisterObserver( ObserverInterface * const pObserver )
	{
		m_observers.Register( pObserver );
	}

private:
    virtual INT_PTR UserProc( UINT const, WPARAM const, LPARAM const );

	void setBrushMode       ( WORD const ) const;
	void setBrushShape      ( WORD const ) const;
	void setBrushManipulator( WORD const ) const;

	void updateOperationButtons( tBrushMode const ) const;

	ReadBuffer          * m_pReadBuffer;
    WorkThreadInterface * m_pWorkThreadInterface;
    DspOptWindow        * m_pDspOptWindow;
	ViewCollection        m_observers;    // observers who want to know if edit window becomes visible or hidden
};
