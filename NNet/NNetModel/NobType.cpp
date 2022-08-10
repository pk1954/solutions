// NobType.cpp
//
// NNetModel

#include <ostream>
#include <unordered_map>
#include "NobType.h"

using std::unordered_map;
using std::wstring;

wstring NobType::GetName(NobType::Value const val)
{
	using enum NobType::Value;
	static unordered_map < NobType::Value, wstring const > mapNob =
	{
		{ undefined,       L"undefined"       },
		{ inputConnector,  L"inputConnector"  },
		{ outputConnector, L"outputConnector" },
		{ inputLine,       L"inputLine"       },
		{ outputLine,      L"outputLine"      },
		{ neuron,          L"neuron"          },
		{ pipe,            L"pipe"            },
		{ knot,            L"knot"            }
	};				  
	return mapNob.at(val);
}

NobType::Value NobType::GetTypeFromName(wstring const & name)
{
	using enum NobType::Value;
	static unordered_map < wstring, NobType::Value > mapNob =
	{
		{ L"undefined",       undefined       },
		{ L"inputConnector",  inputConnector  },
		{ L"outputConnector", outputConnector },
		{ L"inputNeuron",     inputLine       },  // Legacy
		{ L"inputLine",       inputLine       },
		{ L"outputNeuron",    outputLine      },  // Legacy
		{ L"outputLine",      outputLine      },
		{ L"neuron",          neuron	      },
		{ L"pipe",            pipe	          },
		{ L"knot",            knot            }
	};				  
	return mapNob.at(name);
}

wostream & operator<< (wostream & out, NobType const & nobType)
{
	wstring const wstrName { NobType::GetName(nobType.GetValue()) };
	out << wstrName;
	return out;
}
