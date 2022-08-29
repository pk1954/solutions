// Analyzer.ixx
//
// NNetModel
//
// analyze model and find suspicious structures

module;

#include <string>
#include <vector>
#include "NNetModel.h"
#include "Nob.h"
#include "NNetModelReaderInterface.h"

export module Analyzer;

import DisplayFunctor;
import Knot;

using std::vector;
using std::wstring;

export using NobStack = vector<Nob const*>;

export class ModelAnalyzer
{
public:
	static void SetStatusBarDisplay(DisplayFunctor* const func) { m_pStatusBarDisplay = func; }
	static void SetEscFunc(auto const& func) { m_pEscFunc = func; }

	static NobStack FindLoop(NNetModelReaderInterface const&);
	static NobStack FindAnomaly(NNetModelReaderInterface const&);

private:

	inline static DisplayFunctor* m_pStatusBarDisplay{ nullptr };
	inline static bool             m_bStop{ false };
	inline static int              m_iRecDepth{ 0 };
	inline static NobStack         m_nobStack{ };

	inline static bool (*m_pEscFunc)() { nullptr };

	static bool findLoop(Nob const&);
	static bool hasAnomaly(Knot const&);

	static void statusDisplay(wstring const& str)
	{
		if (m_pStatusBarDisplay)
			(*m_pStatusBarDisplay)(str);
	}

	static void statusDisplay(wchar_t const* pStr)
	{
		statusDisplay(wstring(pStr));
	}
};
