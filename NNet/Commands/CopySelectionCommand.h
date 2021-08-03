// CopySelectionCommand.h
//
// Commands

#pragma once

#include <vector>
#include "Nob.h"
#include "NamedType.h"
#include "NNetParameters.h"
#include "NobIdList.h"
#include "Command.h"

class NNetModelWriterInterface;
class BaseKnot;

using std::vector;
using std::unique_ptr;
using std::make_unique;

class CopySelectionCommand : public Command
{
public:
	CopySelectionCommand(NNetModelWriterInterface &);
	virtual ~CopySelectionCommand(){};

	virtual void Do  (NNetModelWriterInterface &);
	virtual void Undo(NNetModelWriterInterface &);

private:

	void addMissingKnot(BaseKnot *, Nob2NobFunc const &);

	using SelNobsIndex  = NamedType <size_t, struct SelNobsIndex_Parameter>;
	using SSIndexVector = vector<SelNobsIndex>;

	size_t        m_sizeOfSelection { 0 };
	SSIndexVector m_indexList       {};     // indices into m_copies
	vector<UPNob> m_copies          {};
	NobIdList     m_selectedNobIds  {};
};

