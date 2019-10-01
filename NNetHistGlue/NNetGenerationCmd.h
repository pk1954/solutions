// NNetGenerationCmd.h
//
// NNetHistGlue

#pragma once

#include "GenerationCmd.h"

class NNetGenerationCmd : public GenerationCmd
{
public:

	enum class Id : int8_t // enumeration starts after last GenerationCmd::Id value
	{
		highlight = static_cast<int8_t>(GenerationCmd::Id::FIRST_APP_CMD),  // placeholder
		editLast,                                                           // placeholder
		FIRST_EDIT_CMD = highlight,
		LAST_APP_CMD   = editLast
	};

	static_assert( (int)Id::LAST_APP_CMD < GenerationCmd::MAX_APP_CMD, "Too many NNetGenerationCmd::Id values" );

	virtual bool IsEditorCommand( )
	{
		Id const cmd = static_cast<Id>( GetCommand() );

		return ( Id::FIRST_EDIT_CMD <= cmd ) && ( cmd <= Id::LAST_APP_CMD );
	}
};

wchar_t const * const GetNNetCommandName     ( NNetGenerationCmd::Id const );
wchar_t const * const GetNNetCommandNameShort( NNetGenerationCmd::Id const );

std::wostream & operator << ( std::wostream &, GenerationCmd const & );
