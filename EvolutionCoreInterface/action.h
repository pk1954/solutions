// action.h
//
// EvolutionCoreInterface

#pragma once

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

	static void Apply2All( std::function<void(Id const &)> const & func )
	{
        for ( int index = 0; index < COUNT; ++index )
		{
            func( static_cast<Id>(index) );   
		}
	}

	static Id Select( std::function< Id (Id const &) > const & func )
	{
        for ( int index = 0; index < COUNT; ++index )
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

template <typename T>
class ActionArray
{
public:
    T & operator[] ( Action::Id action ) 
	{ 
		return m_tArray.at( static_cast<unsigned short>( action ) ); 
	}

	void fill(T const val)
	{
		m_tArray.fill( val );
	}

private:
    std::array < T, Action::COUNT > m_tArray;
};
