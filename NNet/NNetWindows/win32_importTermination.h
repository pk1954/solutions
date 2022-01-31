// win32_importTermination.h
//
// NNetWindows

#pragma once

#include "Resource.h"
#include "ImportTermination.h"

using std::unique_ptr;
using std::make_unique;

class NNetImportTermination : public ImportTermination
{
public:
	static void Initialize(HWND const hwndApp)
	{
		m_hwndApp = hwndApp;
	}

	static unique_ptr<NNetImportTermination> CreateNew(int const msg)
	{
		return make_unique<NNetImportTermination>(msg);
	}

	explicit NNetImportTermination(int const msg)
	  :	m_msgImportFinished(msg)
	{}

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
	inline static HWND m_hwndApp;

	int m_msgImportFinished;
};
