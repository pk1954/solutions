// WrapDescription.ixx
//
// ModelIO

module;

#include <string>
#include <iostream>
#include "NNetModelWriterInterface.h"
#include "NNetModelIO.h"

export module WrapDescription;

import Symtab;
import NNetWrapperBase;
import Script;

using std::wstring;
using std::wostream;
using std::endl;

class WrapDescription : public NNetWrapperBase
{
public:
	using NNetWrapperBase::NNetWrapperBase;

	void operator() (Script& script) const final
	{
		wstring const wstrDescription{ script.ScrReadString() };
		m_modelIO.GetImportNMWI().AddDescriptionLine(wstrDescription);
	}

	void Write(wostream & out) const final
	{
		wstring const& wstrCmdName{ SymbolTable::GetSymbolName(Symbol(this)) };
		wstring wstrLine;
		int iLineNr = 0;
		while (m_modelIO.GetExportNMRI().GetDescriptionLine(iLineNr++, wstrLine))
		{
			out << wstrCmdName << L" \"" << wstrLine << "\"" << endl;
		}
	};
};
