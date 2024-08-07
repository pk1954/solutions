// WrapDescription.ixx
//
// ModelIO

module;

#include <string>
#include <iostream>

export module WrapDescription;

import IoUtil;
import IoConstants;
import Commands;
import RunTime;
import NNetModelIO;
import NNetModel;

using std::wstring;
using std::wostream;
using std::endl;

export class WrapDescription : public Wrapper
{
public:
	using Wrapper::Wrapper;

	void operator() (Script& script) const final
	{
		wstring const wstrDescription{ script.ScrReadString() };
		NNetModelIO::GetImportNMWI().AddDescriptionLine(wstrDescription);
	}

	void Write(wostream & out) const final
	{
		wstring const& wstrCmdName{ GetName() };
		wstring wstrLine;
		int iLineNr = 0;
		while (NNetModelIO::GetExportNMRI().GetDescriptionLine(iLineNr++, wstrLine))
		{
			WriteCmdName(out);
			out << Quoted(wstrLine) << endl;
		}
	};
};
