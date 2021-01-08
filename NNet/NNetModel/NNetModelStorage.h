// NNetModelStorage.h 
//
// NNetModel

#pragma once

#include <string>

using std::wstring;

class ReadModelResult
{
public:
	virtual ~ReadModelResult() {};

	enum class tResult
	{
		ok,
		fileNotFound,
		errorInFile
	};

	virtual void Reaction( tResult const, wstring const = wstring() ) = 0;
};

class NNetModelStorage
{
public:

	inline static unsigned long const SIGSRC_SHAPE_NR { 100 };  // legacy
	inline static unsigned long const SIGSRC_CIRCLE   { 101 };

	inline static double const PROTOCOL_VERSION { 1.7 };
};
