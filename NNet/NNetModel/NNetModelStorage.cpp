// NNetModelStorage.cpp
//
// NNetModel

#include "stdafx.h"
#include <string>
#include <filesystem>
#include <assert.h>
#include "NNetError.h"
#include "win32_script.h"
#include "NNetModelStorage.h"

using std::filesystem::path;

void NNetModelStorage::Initialize
( 
    NNetModelReaderInterface * const pModelR,
    NNetModelWriterInterface * const pModelW,
    Param                    * const pParam,
    Observable               * const unsavedChangesObservable,
    Script                   * const pScript, 
    ReadModelResult          * const pResult,
    ModelDescription         * const pDescription
)
{
    m_pModelReaderInterface    = pModelR;
    m_pModelWriterInterface    = pModelW;
    m_pParam                   = pParam;
    m_unsavedChangesObservable = unsavedChangesObservable;
    m_pScript                  = pScript;
    m_pResult                  = pResult;
    m_pDescription             = pDescription;
}

void NNetModelStorage::setUnsavedChanges( bool const bState )
{
    m_bUnsavedChanges = bState;
    m_unsavedChangesObservable->NotifyAll( false );
}

bool NNetModelStorage::AskAndSave( )
{
    if ( m_bUnsavedChanges )
    {
        int iRes = MessageBox( m_hwndApp, L"Save now?", L"Unsaved changes", MB_YESNOCANCEL );
        if ( iRes == IDYES )
            SaveModel( );
        else if ( iRes == IDNO )
            return true;
        else if ( iRes == IDCANCEL )
            return false;
    }
    return true;
}

bool NNetModelStorage::AskModelFile( )
{
    wstring wstrPath = ScriptFile::AskForFileName( L"mod", L"Model files", tFileMode::read );
    if ( wstrPath != L"" )
    {
        m_wstrPathOfOpenModel = wstrPath;
        return true;
    }
    return false;
}

bool NNetModelStorage::SaveModelAs( )
{
    if ( m_wstrPathOfOpenModel == L"" )
    {
        m_wstrPathOfOpenModel = path( ScriptFile::GetPathOfExecutable( ) ).parent_path();
        m_wstrPathOfOpenModel += L"\\std.mod";
        writeModel();
        return true;
    }
    else
    {
        m_wstrPathOfOpenModel = ScriptFile::AskForFileName( L"mod", L"Model files", tFileMode::write );
        bool const bRes = m_wstrPathOfOpenModel != L"";
        if ( bRes )
            writeModel( );
        return bRes;
    }
}

bool NNetModelStorage::SaveModel( )
{
    if ( m_wstrPathOfOpenModel == L"" )
    {
        return SaveModelAs( );
    }
    else
    {
        writeModel( );
        return true;
    }
}

void NNetModelStorage::ResetModelPath( ) 
{ 
    m_wstrPathOfOpenModel = L""; 
    setUnsavedChanges( true );
}
