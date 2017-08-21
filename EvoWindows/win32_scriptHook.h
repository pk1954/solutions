// win32_scriptHook.h
//

#pragma once

#include "win32_status.h"
#include "script.h"

class ScriptHook : public Script_Functor
{
public:

    explicit ScriptHook( StatusBar * const pStatusBar ) : m_pStatusBar( pStatusBar ) {};

    virtual void operator( ) ( Script & script ) const
    {
        if ( m_pStatusBar != nullptr )
        {
            wstring const & wszPath = script.GetActPath( );
            wstring const & wszLine = script.GetActLine( );
            int     const   iLineNr = script.GetActLineNr( );
            m_pStatusBar->DisplayScriptLine( wszPath, iLineNr, wszLine );
        }
    }

private:

    StatusBar * m_pStatusBar;
};
