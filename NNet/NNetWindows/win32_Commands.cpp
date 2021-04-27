// win32_Commands.cpp
//
// NNetWindows

#include "stdafx.h"
#include "AlignDirectionCommand.h"
#include "AlignShapesCommand.h"
#include "PackShapesCommand.h"
#include "CreateConnectorCommand.h"
#include "NNetModelWriterInterface.h"
#include "CommandStack.h"
#include "win32_Commands.h"

using std::wcout;
using std::endl;
using std::move;
using std::make_unique;
using std::unique_ptr;

void WinCommands::Initialize
( 
	CommandStack             * const pCmdStack,
	NNetModelCommands        * const pCmds,
	NNetModelWriterInterface * const pNMWI
) 
{ 
	m_pCmdStack = pCmdStack;
	m_pCommands = pCmds;
	m_pNMWI     = pNMWI;
}

void WinCommands::Update(ConnAnimationCommand * const pCAC)
{
	if (pCAC)
		m_pNMWI->SetConnNeurons(pCAC->GetActual(), pCAC->GetAnimatedShapes());
}

unique_ptr<ShapePtrList<ConnNeuron>> WinCommands::CreateShapeList()
{
	unique_ptr<ShapePtrList<ConnNeuron>> upShapesAnimated;
	ShapeType          const shapeType { m_pNMWI->GetUPShapes().DetermineShapeType() };
	if ( ! shapeType.IsUndefinedType() )
		return make_unique<ShapePtrList<ConnNeuron>>(m_pNMWI->GetUPShapes().GetAllSelected<ConnNeuron>(shapeType));
	return nullptr;
}

void WinCommands::AlignDirection(function<void(ConnAnimationCommand const *)> const & func)
{ 
	if ( IsTraceOn() )
		TraceStream() << __func__ << endl;
	m_pCmdStack->PushCommand( make_unique<AlignDirectionCommand>(move(CreateShapeList()), func) );
}

void WinCommands::AlignShapes(function<void(ConnAnimationCommand const *)> const & func)
{ 
	if ( IsTraceOn() )
		TraceStream() << __func__ << endl;
	m_pCmdStack->PushCommand( make_unique<AlignShapesCommand>(move(CreateShapeList()), func) );
}

void WinCommands::PackShapes(function<void(ConnAnimationCommand const *)> const & func)
{
	if ( IsTraceOn() )
		TraceStream() << __func__ << endl;
	m_pCmdStack->PushCommand( make_unique<PackShapesCommand>(move(CreateShapeList()), func) );
}

void WinCommands::CreateConnector(function<void(bool const)> const & func)
{
	if ( IsTraceOn() )
		TraceStream() << __func__ << endl;
	m_pCmdStack->PushCommand( make_unique<CreateConnectorCommand>(move(CreateShapeList()), func) );
}
