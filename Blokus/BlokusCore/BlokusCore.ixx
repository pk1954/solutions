// BlokusCore.ixx
//
// BlokusCore

export module BlokusCore;

export import :BlokusMove;
export import :BlokusPreferences;
export import :BlokusUtilities;
export import :Components;
export import :Match;
export import :Shape;
//export import :Strategy;
//export import :StrategyTakeFirst;
//export import :StrategyRandom;
export import :Tournament;

import :PlayerTypes;
import :Components;
import :BlokusPreferences;

export void InitializeBlokusCore()
{
	PlayerTypes::Initialize();
	Components::Initialize();
	BlokusPreferences::Initialize();
}