// win32_editor.h 
//

#pragma once

#include "EvolutionTypes.h"
#include "win32_baseDialog.h"

class WorkThreadInterface;
class DspOptWindow;
class EvolutionCore;
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
		EvolutionCore       * const, 
		DspOptWindow        * const
	);
	void Stop( );

	void UpdateEditControls( );
	void SetSimuMode( bool const );

private:
    virtual INT_PTR UserProc( UINT const, WPARAM const, LPARAM const );

    LRESULT sendClick          ( int )        const;
	void    setBrushMode       ( WORD const ) const;
	void    setBrushShape      ( WORD const ) const;
	void    setBrushManipulator( WORD const ) const;

    void updateEditControls( EvolutionCore const * const );
	void updateOperationButtons( tBrushMode const ) const;

	EvolutionCore       * m_pCore;
    WorkThreadInterface * m_pWorkThreadInterface;
    DspOptWindow        * m_pDspOptWindow;
};
