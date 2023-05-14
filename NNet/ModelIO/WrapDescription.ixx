// WrapDescription.ixx
//
// ModelIO

module;

#include <string>
#include <iostream>

export module WrapDescription;

import IoUtil;
import NNetWrapperBase;
import Script;
import NNetModelIO;
import NNetModel;

using std::wstring;
using std::wostream;
using std::endl;

export class WrapDescription : public NNetWrapperBase
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
		wstring const& wstrCmdName{ GetName() };
		wstring wstrLine;
		int iLineNr = 0;
		while (m_modelIO.GetExportNMRI().GetDescriptionLine(iLineNr++, wstrLine))
		{
			WriteCmdName(out);
			out << DOUBLE_QUOTE << wstrLine << DOUBLE_QUOTE << endl;
		}
	};
};
