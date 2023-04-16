// NNetModelCommands.cpp
//
// Commands

module;

#include <cassert>
#include <iostream>
#include <memory>
#include <source_location>

module NNetModelCommands;

import Commands;
import NNetModel;
import NNetModelIO;
import Uniform2D;
import NNetCommands;

using std::wstring;
using std::wcout;
using std::endl;
using std::move;
using std::make_unique;
using std::unique_ptr;
using std::source_location;

void NNetModelCommands::Initialize
(
	NNetModelIO      * const pModelIO,
	NNetCommandStack * const pCmdStack
) 
{ 
	m_pModelIO  = pModelIO;
	m_pCmdStack = pCmdStack;
}

void NNetModelCommands::AddModel()
{
	if (m_bTrace)
		TraceStream() << source_location::current().function_name() 
		              << L" \"" << m_pModelIO->GetModelFileName() << L"\" " << endl;
	unique_ptr<Model> upImportedModel { m_pModelIO->GetImportedModel() };
	m_pCmdStack->PushCommand(make_unique<AddNobsCommand>(upImportedModel->MoveUPNobs()));
}
