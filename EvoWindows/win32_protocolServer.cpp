// Win32_protocolServer.cpp 
//
// Win32_utilities

#include "stdafx.h"
#include <sstream>
#include <iostream>
#include "EvolutionCore.h"
#include "HistorySystem.h"
#include "win32_protocolServer.h"

ProtocolServer::ProtocolServer( )
  : m_pCore( nullptr ),
	m_pHistorySystem( nullptr )
{
}

ProtocolServer::~ProtocolServer( )
{
	m_pCore = nullptr;
	m_pHistorySystem = nullptr;
}

void ProtocolServer::Notify( bool const bImmediate )
{
	std::wcout << L"HistGen " << m_pHistorySystem->GetCurrentGeneration().GetLong() << std::endl;
	std::wcout << m_pCore->GetProtocolData( )->str();
	m_pCore->ClearProtocolData( );
}
