// NobType.cpp
//
// NNetModel

#include "stdafx.h"
#include <ostream>
#include <unordered_map>
#include "NobType.h"

using std::unordered_map;
using std::wstring;

wstring NobType::GetName(NobType::Value const val)
{
	static unordered_map < NobType::Value, wstring const > mapNob =
	{
		{ NobType::Value::undefined,       L"undefined"       },
		{ NobType::Value::inputConnector,  L"inputConnector"  },
		{ NobType::Value::outputConnector, L"outputConnector" },
		{ NobType::Value::inputNeuron,     L"inputNeuron"     },
		{ NobType::Value::outputNeuron,    L"outputNeuron"    },
		{ NobType::Value::neuron,          L"neuron"          },
		{ NobType::Value::pipe,            L"pipe"            },
		{ NobType::Value::knot,            L"knot"            },
		{ NobType::Value::synapse,         L"synapse"         },
		{ NobType::Value::branch,          L"branch"          },
		{ NobType::Value::bend,            L"bend"            }
	};				  
	return mapNob.at(val);
}

NobType::Value NobType::GetTypeFromName(wstring const & name)
{
	static unordered_map < wstring, NobType::Value > mapNob =
	{
		{ L"undefined",       NobType::Value::undefined       },
		{ L"inputConnector",  NobType::Value::inputConnector  },
		{ L"outputConnector", NobType::Value::outputConnector },
		{ L"inputNeuron",     NobType::Value::inputNeuron     },
		{ L"outputNeuron",    NobType::Value::outputNeuron    },
		{ L"neuron",          NobType::Value::neuron	      },
		{ L"pipe",            NobType::Value::pipe	          },
		{ L"knot",            NobType::Value::knot            },
		{ L"synapse",         NobType::Value::synapse         },
		{ L"branch",          NobType::Value::branch          },
		{ L"bend",            NobType::Value::bend            }
	};				  
	return mapNob.at(name);
}

wostream & operator<< (wostream & out, NobType const & nobType)
{
	wstring const wstrName { NobType::GetName(nobType.GetValue()) };
	out << wstrName;
	return out;
}
