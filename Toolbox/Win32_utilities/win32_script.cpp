// Win32_Script.cpp
//
// Win32_utilities

module;

#include <Windows.h>
#include "commdlg.h"
#include "pathcch.h"
#include <ShObjIdl.h> 

module ScriptFile;

import std;
import RunTime;
import Commands;
import IoConstants;

using std::wcout;
using std::vector;
using std::wstring;
using std::wostringstream;
using std::endl;

wstring ScriptFile::GetPathOfExecutable()
{
	int iBufSize { 256 };
	vector<wchar_t> buffer;
	do
	{
		iBufSize *= 2;
		buffer.resize(iBufSize);
	} while (GetModuleFileName(nullptr, buffer.data(), iBufSize) == iBufSize);

	return wstring(buffer.data());
}

ScriptFile::ScriptFile()
{
    HRESULT hr = CoInitializeEx(NULL, COINIT_APARTMENTTHREADED|COINIT_DISABLE_OLE1DDE);
    //assert(SUCCEEDED(hr));
}

ScriptFile::~ScriptFile()
{
    CoUninitialize();
}

IFileDialog * ScriptFile::createOpenDialog()
{
    IFileDialog * pFileDlg { nullptr }; 
    HRESULT hr = CoCreateInstance 
    (
        CLSID_FileOpenDialog, 
        NULL, CLSCTX_ALL, 
        IID_IFileOpenDialog, 
        reinterpret_cast<void**>(& pFileDlg)
   );
    if (! SUCCEEDED(hr))
    {
        wcout << L" +++ ScriptFile constructor: CoCreateInstance(CLSID_FileOpenDialog ...) failed." << endl;
        wcout << L" +++ error code: " << hr << endl;
        return nullptr;
    }
    hr = pFileDlg->SetTitle(L"Open file");
    //assert(SUCCEEDED(hr));
    return pFileDlg;
}

IFileDialog * ScriptFile::createSaveDialog(wstring const &wstrFileName)
{
    IFileDialog * pFileDlg { nullptr }; 
    HRESULT hr = CoCreateInstance
    (
        CLSID_FileSaveDialog, 
        NULL, CLSCTX_ALL, 
        IID_IFileSaveDialog, 
        reinterpret_cast<void**>(& pFileDlg)
   );
    if (! SUCCEEDED(hr))
    {
        wcout << L" +++ ScriptFile constructor: CoCreateInstance(CLSID_FileSaveDialog ...) failed." << endl;
        wcout << L" +++ error code: " << hr << endl;
        return nullptr;
    }

    hr = pFileDlg->SetTitle(L"Save file");
    //assert(SUCCEEDED(hr));
    if (wstrFileName != L"")
        hr = pFileDlg->SetFileName(wstrFileName.c_str());
    //assert(SUCCEEDED(hr));
    return pFileDlg;
}

bool ScriptFile::setFileTypes
(
    IFileDialog   * pFileDlg, 
    wstring const & extension, 
    wstring const & description 
)
{
    wstring           filter { L"*." + extension };
    COMDLG_FILTERSPEC rgSpec { description.c_str(), filter.c_str() };
    HRESULT           hr     { pFileDlg->SetFileTypes(1, &rgSpec) };
    if (! SUCCEEDED(hr))
    {
        wcout << L" +++ AskForFileName: SetFileTypes" 
              << OPEN_BRACKET << "1," << SPACE
                << CurlyBrackets(Quoted(description) + L", " + Quoted(filter))
              << CLOSE_BRACKET << "failed." << endl;
        wcout << L" +++ error code : " << hr << endl;
        return false;
    }
    return true;
}

bool ScriptFile::setDefaultExtension
(
    IFileDialog * pFileDlg, 
    wstring const & extension
)
{
    HRESULT hr { pFileDlg->SetDefaultExtension(extension.c_str()) };
    if (! SUCCEEDED(hr))
    {
        wcout << L" +++ AskForFileName: SetDefaultExtension" << Brackets(Quoted(extension)) << " failed." << endl;
        wcout << L" +++ error code : " << hr << endl;
        return false;
    }
    return true;
}

wstring ScriptFile::getResult(IFileDialog * const pFileDlg)
{
    HRESULT      hr;
    wstring      wstrRes { };
    IShellItem * pItem   { nullptr };
    hr = pFileDlg->GetResult(& pItem);                        
    if (! SUCCEEDED(hr))
    {
        wcout << L" +++ AskForFileName: GetResult(& pItem) failed." << endl;
        wcout << L" +++ error code: " << hr << endl;
        return wstrRes;
    }

    PWSTR pszPath { nullptr };
    hr = pItem->GetDisplayName(SIGDN_FILESYSPATH, & pszPath); 
    if (! SUCCEEDED(hr))
    {
        wcout << L" +++ AskForFileName: GetDisplayName(SIGDN_FILESYSPATH, & pszPath) failed." << endl;
        wcout << L" +++ error code: " << hr << endl;
        return wstrRes;
    }

    wstrRes = pszPath;
    CoTaskMemFree(pszPath);
    pItem->Release();
    return wstrRes;
}

wstring ScriptFile::AskForFileName
(
	wstring   const & extension, 
    wstring   const & wstrFileName,
	wstring   const & description,
	tFileMode const   mode
)
{
    IFileDialog * pFileDlg { nullptr }; 
    wstring       wstrRes  { };

    if (mode == tFileMode::read)
        pFileDlg = createOpenDialog();
    else
        pFileDlg = createSaveDialog(wstrFileName);

    if (! pFileDlg)
        return wstrRes;

    if (! setDefaultExtension(pFileDlg, extension))
        return wstrRes;

    if (! setFileTypes(pFileDlg, extension, description))
        return wstrRes;

    HRESULT hr = pFileDlg->Show(NULL); 
    if (SUCCEEDED(hr))
        wstrRes = getResult(pFileDlg);

    pFileDlg->Release();

    return wstrRes;
}

void StartScript
(
    wstring       const& wstrFile,
    ScriptFunctor const& scriptHook
)
{
    wcout << COMMENT_START + L"Processing script file " << wstrFile << endl;
    Script* pScript { ScriptStack::OpenScript() };
    if (pScript && pScript->ScrOpen(wstrFile))
    {
        pScript->ScrSetNewLineHook(&scriptHook);  // TODO: check if NextScriptCommand needed
  //      WinCommand::NextScriptCommand();  // start reading script file
    }
}
