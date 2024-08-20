// FatalError.ixx
//
// Toolbox\BasicUtils

export module FatalError;

import std;
import StackTrace;

using std::wstring;
using std::wostringstream;
using std::wcout;
using std::endl;

export class FatalError
{
public:
	static void Happened(long const errnr, wstring const &errText)
	{
		wcout << L"+++ Fatal error" << endl;
		wcout << L"+++ Error number:  " << errnr   << endl;
		wcout << L"+++ " << errText << endl;
		wcout << L"Stack trace:" << endl;
		wcout << StacktraceToWString();
		wcout << L"+++ forced application exit" << endl;
		exit(3);
	}

	static void Provoke()
	{
		long long x = 5;
		int * pi = (int *)(x); //-V566
		* pi = 23;
	}
};