// win32_editor.h 
//

#pragma once

#include "win32_baseDialog.h"

class WorkThread;
class DspOptWindow;
class ModelData;

class EditorWindow : public BaseDialog
{
public:
    EditorWindow( );
    ~EditorWindow( );

    void Start( HWND const, WorkThread * const, ModelData * const, DspOptWindow * const );

    void    UpdateControls( );
    void    SetEditMode( BOOL fMode ) { m_editMode = fMode; };
    BOOL    IsInEditMode    ( ) const { return m_editMode; };
    LRESULT SendClick( int ) const;

protected:
    virtual INT_PTR UserProc( UINT const, WPARAM const, LPARAM const );

private:

    ModelData    * m_pModelWork;
    WorkThread   * m_pWorkThread;
    DspOptWindow * m_pDspOptWindow;
    BOOL           m_editMode;
};

