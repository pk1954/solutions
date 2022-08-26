// InputOutputUI.h 
//
// ModelIO

#pragma once

#include <string>

import Script;

using std::wstring;

class InputOutputUI
{
public:
	enum class Result
	{
		ok,
		fileNotFound,
		errorInFile
	};

	virtual ~InputOutputUI() = default;

	virtual void JobFinished(InputOutputUI::Result const, wstring const &) const = 0;
	virtual void ReadProgressReport(Script *)         { /* default: no report */ };
	virtual void WriteProgressReport(wstring const &) { /* default: no report */ };
};
