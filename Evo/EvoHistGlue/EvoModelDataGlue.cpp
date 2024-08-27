// EvoModelDataGlue.cpp
//
// EvoHistGlue

module EvoHistGlueLib:EvoModelDataGlue;

import EvoCoreLib;
import :EvoGenerationCmd;

//Stopwatch EvoModelDataGlue::stopwatch;

void EvoModelDataGlue::OnAppCommand(GenerationCmd const cmd)
{
	EvoGenerationCmd::Id const evoCmd = static_cast<EvoGenerationCmd::Id>(cmd.GetCommand());
	Int24   const param  = cmd.GetParam();
	switch (evoCmd)
	{

	case EvoGenerationCmd::Id::editDoEdit:
		m_pEvolutionCore->ModelDoEdit(GridPoint24::Unpack(param));
		break;

	case EvoGenerationCmd::Id::editSetPOI:
		GridPOI::SetPoi(GridPoint24::Unpack(param));
		break;

	case EvoGenerationCmd::Id::editSetBrushMode:
        m_pEvolutionCore->SetBrushMode(static_cast<tBrushMode>(param.GetValue()));
        break;

    case EvoGenerationCmd::Id::editSetBrushShape:
        m_pEvolutionCore->SetBrushShape(static_cast<tShape>(param.GetValue()));
		break;

    case EvoGenerationCmd::Id::editSetBrushManipulator:
        m_pEvolutionCore->SetBrushManipulator(static_cast<tManipulator>(param.GetValue()));
		break;

    case EvoGenerationCmd::Id::editSetBrushRadius:
        m_pEvolutionCore->SetBrushRadius(static_cast<GridCoord>(param.GetValue()));
		break;

    case EvoGenerationCmd::Id::editSetBrushIntensity:
        m_pEvolutionCore->SetBrushIntensity(static_cast<PERCENT>(param.GetValue()));
		break;

	default:
		Assert(false);
	}
}
