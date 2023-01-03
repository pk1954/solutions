// DeselectModuleCmd.ixx
//
// Commands

export module DeselectModuleCmd;

import SelectionCommand;
import NNetModel;

export class DeselectModuleCmd : public SelectionCommand
{
public:
	DeselectModuleCmd()
	{ }

	void Do() final 
	{ 
		SelectionCommand::Do();
		m_pNMWI->GetUPNobs().SelectAllNobs(false); 
	}
};
