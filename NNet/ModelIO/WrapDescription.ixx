// WrapDescription.ixx
//
// ModelIO

module;

#include <string>
#include <iostream>

export module WrapDescription;

import IoUtil;
import IoConstants;
import WrapBase;
import Script;
import NNetModelIO;
import NNetModel;

using std::wstring;
using std::wostream;
using std::endl;

export class WrapDescription : public WrapBase
{
public:
	using WrapBase::WrapBase;

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
			out << DOUBLE_QUOTE << wstrLine << DOUBLE_QUOTE << endl;
		}
	};
};
