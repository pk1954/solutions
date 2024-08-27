// ProtocolServer.cpp 
//
// EvoWindows

module ProtocolServer;

import std;
import EvoCoreLib;
import HistoryLib;

ProtocolServer::ProtocolServer()
  : m_pHistorySystem(nullptr)
{
//	EvolutionCore::RegisterObserver(this);
}

ProtocolServer::~ProtocolServer()
{
	m_pHistorySystem = nullptr;
}

void ProtocolServer::Notify(bool const bImmediate)
{
	auto data = EvolutionCore::GetProtocolData()->str();
	if (! data.empty())
	{
		std::wcout << L"HistGen " << m_pHistorySystem->GetCurrentGeneration().GetLong() << std::endl;
		std::wcout << data;
		EvolutionCore::ClearProtocolData();
	}
}
