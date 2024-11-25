// EvoGenerationCmd.ixx
//
// EvoHistGlue

export module EvoHistGlueLib:EvoGenerationCmd;

import std;
import HistoryLib;

using std::wostream;
using std::int8_t;

export class EvoGenerationCmd : public GenerationCmd
{
public:

	enum class Id : int8_t // enumeration starts after last GenerationCmd::Id value
	{
		editSetBrushMode = static_cast<int8_t>(GenerationCmd::Id::FIRST_APP_CMD),
		editSetBrushShape,
		editSetBrushManipulator,
		editSetBrushRadius,
		editSetBrushIntensity,
		editSetPOI,
		editDoEdit,
		FIRST_EDIT_CMD = editSetBrushMode,
		LAST_APP_CMD = editDoEdit
	};

	bool IsEditorCommand()
	{
		Id const cmd = static_cast<Id>(GetCommand());

		static_assert((int)Id::LAST_APP_CMD < GenerationCmd::MAX_APP_CMD, "Too many EvoGenerationCmd::Id values");

		return (Id::FIRST_EDIT_CMD <= cmd) && (cmd <= Id::LAST_APP_CMD);
	}
};

wchar_t const* const GetEvoCommandName(EvoGenerationCmd::Id const);
wchar_t const* const GetEvoCommandNameShort(EvoGenerationCmd::Id const);

wostream& operator << (wostream&, GenerationCmd const&);
