// action.h
//
// EvolutionCoreInterface

#pragma once

#include <array>
#include <functional>

class Action
{
public:
	enum class Id : unsigned short
	{ 
		move, 
		clone,
		marry,
		interact, 
		eat, 
		fertilize,
		passOn,
		undefined,
		count
	};

	static const int COUNT = static_cast<int>( Id::count );

	static void Apply2AllEnabledActions( std::function<void(Id const &)> const & );

	static Id Select( std::function< Id (Id const &) > const & func )
	{
        for ( int index = 0; index < static_cast<int>( Id::count ); ++index )
		{
            Id actionRes = func( static_cast<Id>(index) );   
			if ( IsDefined( actionRes ) )                    // if lambda returns defined action
				return actionRes;                            // break out of loop
		}
		return Id::undefined;
	}

	static bool IsDefined( Id const action )
	{
		return action != Id::undefined;
	}

	static bool IsUndefined( Id const action )
	{
		return action == Id::undefined;
	}

	static wchar_t const * const GetName( Id const );
};
