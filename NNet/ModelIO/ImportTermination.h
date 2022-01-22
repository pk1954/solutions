// NNetModelImporter.h 
//
// ModelIO

#pragma once

#include <string>

using std::wstring;

class ImportTermination
{
public:
	enum class Result
	{
		ok,
		fileNotFound,
		errorInFile
	};

	virtual ~ImportTermination() = default;

	virtual void Reaction(ImportTermination::Result const, wstring const &) = 0;
};

