// Analyzer.cpp
//
// NNetModel

#include "stdafx.h"
#include "MoreTypes.h"
#include "win32_util.h"
#include "NNetModelReaderInterface.h"
#include "Knot.h"
#include "Analyzer.h"

using std::endl;
using std::to_wstring;

NobStack ModelAnalyzer::FindLoop(NNetModelReaderInterface const & nmri)
{
	int const iNrOfNobs { Cast2Int(nmri.GetSizeOfNobList()) };

	for (int iMaxLoopSize = 5; iMaxLoopSize <= iNrOfNobs + 1; iMaxLoopSize += 2)
	{
		m_iRecDepth = iMaxLoopSize;
		m_bStop     = false;
		statusDisplay(wstring(L"Looking for loop of size ") + to_wstring(iMaxLoopSize) + L". Press ESC to stop.");
		m_nobStack.clear();
		if (nmri.GetUPNobsC().Apply2AllB<BaseKnot>([](BaseKnot const & b) { return findLoop(b); }))
		{
			if (m_bStop)  
			{
				m_nobStack.clear();
				statusDisplay(L"analysis aborted by user");
				return m_nobStack;
			}
			else 
			{
				statusDisplay(L"loop found");
				return m_nobStack;
			}
		}
	}

	statusDisplay(L"no loop found");
	return m_nobStack;
}

// findLoop - try to find a loop in model
//
// return value:
//  - true, if loop found (m_bStop == false) or aborted by user (m_bStop == true). 
//  - false, if analysis completed and no loop found

bool ModelAnalyzer::findLoop(Nob const & nob)
{
	if ((* m_pEscFunc)())
		m_bStop = true;

	if (m_bStop)
		return true;

	if (m_nobStack.size() == m_iRecDepth)
		return false;  // maximum search depth reached

	assert(nob.IsDefined());
	m_nobStack.push_back(& nob);

	bool bResult { false };

	if (
		  (m_nobStack.size() > 1) &&                    // we are beyond the initial nob
	      (nob.GetId() == m_nobStack.front()->GetId())  // and found the same nob again
	  )
	{
		bResult = true;  // loop found. Do not pop_back stack!
	}
	else if (nob.IsPipe())
	{
		bResult = findLoop(* static_cast<Pipe const &>(nob).GetEndKnotPtr());  // recursion
	}
	else if (nob.IsBaseKnot())
	{
		bResult = static_cast<BaseKnot const &>(nob).Apply2AllOutPipesB
		(
			[&](Pipe const & pipe) 
			{ 
				return findLoop(pipe); 
			} 
		);
	}
	else
	{
		assert(false);  // nob is neither Pipe nor baseknot
	}

	if (! bResult)
		m_nobStack.pop_back(); // no loop in this branch

	return bResult;
}

bool ModelAnalyzer::hasAnomaly(Knot const & knot)
{
	bool bFoundAnomaly { false };

	if (! knot.HasIncoming())
	{
		knot.Apply2AllOutPipes([&](Pipe const & pipe) { m_nobStack.push_back(& pipe); });
		bFoundAnomaly = true;
	}
	else if (! knot.HasOutgoing())
	{
		knot.Apply2AllInPipes([&](Pipe const & pipe) { m_nobStack.push_back(& pipe); });
		bFoundAnomaly = true;
	}

	if (bFoundAnomaly)
		m_nobStack.push_back(& knot);

	return bFoundAnomaly; 
}

NobStack ModelAnalyzer::FindAnomaly(NNetModelReaderInterface const & nmri)
{
	m_nobStack.clear();
	if (! nmri.GetUPNobsC().Apply2AllB<Knot>([&](Knot const & k) { return hasAnomaly(k); }))
		statusDisplay(L"no anomalies found");
	return m_nobStack;
}
