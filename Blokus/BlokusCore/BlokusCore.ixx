// BlokusCore.ixx
//
// BlokusCore

export module BlokusCore;

export import :BlokusMove;
export import :BlokusPreferences;
export import :BlokusUtilities;
export import :Components;
export import :ListOfMoves;
export import :Match;
export import :MatchReaderInterface;
export import :MatchWriterInterface;
export import :PieceMotion;
export import :PlayerId;
export import :Shape;
export import :Strategy;
export import :StrategyHuman;
export import :StrategyTakeFirst;
export import :StrategyRandom;
export import :StrategyBigFirstRandom;
export import :Tournament;

export void InitializeBlokusCore()
{
	Components::Initialize();
	Shape::Initialize();
}