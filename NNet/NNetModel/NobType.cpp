// NobType.cpp
//
// NNetModel

module NNetModel:NobType;

import std;

using std::unordered_map;
using std::wostream;
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
		{ synapse,         L"synapse"         },
		{ fork,            L"fork"            },
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
		{ L"inputLine",       inputLine       },
		{ L"outputLine",      outputLine      },
		{ L"neuron",          neuron	      },
		{ L"pipe",            pipe	          },
		{ L"synapse",         synapse         },
		{ L"fork",            fork            },
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
