// PlayerId.ixx
//
// BlokusCore

export module BlokusCore:PlayerId;

import std;
import Types;

using std::numeric_limits;

export using PlayerId = NamedType<char, struct PlayerId_Parameter>;

export inline unsigned int const NR_OF_PLAYERS { 4 };

export inline PlayerId const NO_PLAYER    { 0 };
export inline PlayerId const FIRST_PLAYER { 1 };
export inline PlayerId const LAST_PLAYER  { NR_OF_PLAYERS };

export void Apply2AllPlayerIds(auto const& func)
{
    for (PlayerId id = FIRST_PLAYER; id <= LAST_PLAYER; ++id)
        func(id);
}
