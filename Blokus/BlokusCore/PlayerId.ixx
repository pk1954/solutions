// PlayerId.ixx
//
// BlokusCore

export module BlokusCore:PlayerId;

import std;
import Types;

using std::numeric_limits;

export using PlayerId = NamedType<int, struct PlayerId_Parameter>;

export inline PlayerId const NO_PLAYER { -1 };

export inline int const NR_OF_PLAYERS { 4 };

export inline PlayerId UndefinedPlayerId()
{
	static PlayerId constexpr res { (numeric_limits<int>::min)() };
	return res;
}
