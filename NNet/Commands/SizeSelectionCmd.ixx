// SizeSelectionCmd.ixx
//
// Commands

module;

#include "NNetModelWriterInterface.h"
#include "NNetCommand.h"

export module SizeSelectionCmd;

import MoreTypes;

export class SizeSelectionCmd : public NNetCommand
{
public:
	explicit SizeSelectionCmd(float const fFactor)
		: m_fFactor(fFactor)
	{
		m_umPntCenter = m_pNMWI->GetUPNobs().CenterOfGravity
		(
			[](Nob const & nob){ return nob.IsSelected() && nob.IsBaseKnot(); }
		);
	}

	void Do  () final { sizeSelection(m_fFactor); }
	void Undo() final { sizeSelection(1.0f / m_fFactor); }

private:
	void sizeSelection(float const fFactor) const
	{
		m_pNMWI->GetUPNobs().Apply2AllSelected<BaseKnot>
			(
				[this, fFactor](BaseKnot & b) 
				{ 
					b.SetPos(m_umPntCenter + (b.GetPos() - m_umPntCenter) * fFactor);
				}
		);
	}

	MicroMeterPnt m_umPntCenter;
	float         m_fFactor;
};
