// NobType.cpp
//
// NNetModel

#include "stdafx.h"
#include <ostream>
#include <unordered_map>
#include "NobType.h"

using std::unordered_map;
using std::wstring;

wstring const NobType::GetName( NobType::Value const val )
{
	static unordered_map < NobType::Value, wstring const > mapNob =
	{
		{ NobType::Value::undefined,       L"undefined"       },
		{ NobType::Value::inputConnector,  L"inputConnector"  },
		{ NobType::Value::outputConnector, L"outputConnector" },
		{ NobType::Value::closedConnector, L"closedConnector" },
		{ NobType::Value::inputNeuron,     L"inputNeuron"     },
		{ NobType::Value::outputNeuron,    L"outputNeuron"    },
		{ NobType::Value::neuron,          L"neuron"          },
		{ NobType::Value::pipe,            L"pipe"            },
		{ NobType::Value::knot,            L"knot"            }
	};				  
	return mapNob.at( val );
}

NobType::Value const NobType::GetTypeFromName( wstring const & name )
{
	static unordered_map < wstring, NobType::Value > mapNob =
	{
		{ L"undefined",       NobType::Value::undefined       },
		{ L"inputConnector",  NobType::Value::inputConnector  },
		{ L"outputConnector", NobType::Value::outputConnector },
		{ L"closedConnector", NobType::Value::closedConnector },
		{ L"inputNeuron",     NobType::Value::inputNeuron     },
		{ L"outputNeuron",    NobType::Value::outputNeuron    },
		{ L"neuron",          NobType::Value::neuron	      },
	    { L"pipe",            NobType::Value::pipe	          },
	    { L"knot",            NobType::Value::knot            }
	};				  
	return mapNob.at( name );
}

wostream & operator<< ( wostream & out, NobType const & nobType )
{
	wstring const wstrName { NobType::GetName( nobType.GetValue() ) };
	out << wstrName;
	return out;
}
