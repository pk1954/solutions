// win32_editor.h 
//

#pragma once

#include "win32_baseDialog.h"

class WorkThreadInterface;
class DspOptWindow;
class EvolutionModelData;

class EditorWindow : public BaseDialog
{
public:
    EditorWindow( );
    ~EditorWindow( );

    void Start( HWND const, WorkThreadInterface * const, EvolutionModelData * const, DspOptWindow * const );

    void    UpdateEditControls( );
    BOOL    IsInEditMode( )  const;
    LRESULT SendClick( int ) const;

private:
    virtual INT_PTR UserProc( UINT const, WPARAM const, LPARAM const );

	EvolutionModelData  * m_pModelWork;
    WorkThreadInterface * m_pWorkThreadInterface;
    DspOptWindow        * m_pDspOptWindow;
};
