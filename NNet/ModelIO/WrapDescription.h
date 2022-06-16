// WrapDescription.h 
//
// ModelIO

#pragma once

#include "NNetModelWriterInterface.h"
#include "NNetModelIO.h"
#include "NNetWrapperBase.h"

class WrapDescription : public NNetWrapperBase
{
public:
	using NNetWrapperBase::NNetWrapperBase;

	void operator() (Script & script) const final
	{
		wstring const wstrDescription { script.ScrReadString() };
		m_modelIO.GetImportNMWI().AddDescriptionLine(wstrDescription);
	}

	void Write(wostream & out) const final 
	{
		wstring const & wstrCmdName { SymbolTable::GetSymbolName(Symbol(this)) };
		wstring wstrLine;
		int iLineNr = 0;
		while (m_modelIO.GetExportNMRI().GetDescriptionLine(iLineNr++, wstrLine))
		{
			out << wstrCmdName << L" \"" << wstrLine << "\"" << endl;
		}
	};
};
