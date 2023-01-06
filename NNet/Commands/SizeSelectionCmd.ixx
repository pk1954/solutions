// SizeSelectionCmd.ixx
//
// Commands

export module SizeSelectionCmd;

import Types;
import NNetCommand;
import NNetModel;

export class SizeSelectionCmd : public NNetCommand
{
public:
	explicit SizeSelectionCmd(float const fFactor)
		: m_fFactor(fFactor)
	{
		m_umPntCenter = m_pNMWI->GetUPNobs().CenterOfGravity
		(
			[](Nob const & nob){ return nob.IsSelected() && nob.IsPosNob(); }
		);
	}

	void Do  () final { sizeSelection(m_fFactor); }
	void Undo() final { sizeSelection(1.0f / m_fFactor); }

private:
	void sizeSelection(float const fFactor) const
	{
		m_pNMWI->GetUPNobs().Apply2AllSelected<PosNob>
		(
			[this, fFactor](PosNob & b) 
			{ 
				b.SetPos(m_umPntCenter + (b.GetPos() - m_umPntCenter) * fFactor);
			}
		);
	}

	MicroMeterPnt m_umPntCenter;
	float         m_fFactor;
};
