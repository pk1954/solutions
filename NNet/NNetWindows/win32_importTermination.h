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

	void Reaction(ImportTermination::Result const res, wstring const & name) final
	{
		switch (res)
		{
		using enum ImportTermination::Result;
		case ok:     	   SendMessage(m_hwndApp, WM_COMMAND, m_msgImportFinished, 0); break;
		case fileNotFound: SendMessage(m_hwndApp, WM_COMMAND, IDX_FILE_NOT_FOUND,  0); break;
		case errorInFile:  SendMessage(m_hwndApp, WM_COMMAND, IDX_ERROR_IN_FILE,   0); break;
		default: assert(false);
		}
	};

private:
	HWND m_hwndApp;
	int  m_msgImportFinished;
};
