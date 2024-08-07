// NNetEvent.cpp
//
// NNetModel

module NNetModel:NNetEvent;

import std;

using std::unordered_map;
using std::wostream;
using std::wstring;

wstring NNetEvent::GetName(EventType const type)
{
	using enum EventType;
	static unordered_map <EventType, wstring const> mapEvent =
	{
		{ stimulus,  L"stimulus"  },
		{ startScan, L"startScan" },
		{ stopScan,  L"stopScan"  }
	};				  
	return mapEvent.at(type);
}

EventType NNetEvent::GetTypeFromName(wstring const &name)
{
	using enum EventType;
	static unordered_map <wstring, EventType > mapEvent =
	{
		{ L"stimulus",  stimulus  },
		{ L"startScan", startScan },
		{ L"stopScan",  stopScan  }
	};				  
	return mapEvent.at(name);
}

wostream & operator<< (wostream & out, EventType const & type)
{
	wstring const wstrName { NNetEvent::GetName(type) };
	out << wstrName;
	return out;
}
