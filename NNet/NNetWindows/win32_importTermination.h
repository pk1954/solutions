// win32_importTermination.h
//
// NNetWindows

#pragma once

#include "Resource.h"
#include "NNetModelImporter.h"

class NNetImportTermination : public ImportTermination
{
public:
	NNetImportTermination(HWND hwndApp, int msg)
	  : m_hwndApp(hwndApp),
		m_msgImportFinished(msg)
	{ }

	virtual void Reaction(Result const res, wstring const name)
	{
		switch (res)
		{
		case ImportTermination::Result::ok:
			SendMessage(m_hwndApp, WM_COMMAND, m_msgImportFinished, 0);
			break;

		case ImportTermination::Result::fileNotFound:
			MessageBox(nullptr, name.c_str(), L"Could not find model file", MB_OK);
			break;

		case ImportTermination::Result::errorInFile:
			MessageBox
			(
				nullptr, 
				L"Error in model file\r\nSee main_trace.out for details", 
				L"Error", 
				MB_OK 
			);
			break;

		default:
			assert(false);
		}
	};

private:
	HWND m_hwndApp;
	int  m_msgImportFinished;
};
