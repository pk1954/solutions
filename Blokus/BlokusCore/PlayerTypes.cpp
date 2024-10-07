// PlayerTypes.cpp
//
// BlokusCore

module BlokusCore:PlayerTypes;

using std::array;

array<PlayerType, NR_OF_PLAYERS> PlayerTypes::m_playerTypes;

Color const COL_RED    { Color(1.0f, 0.2f, 0.2f) };
Color const COL_GREEN  { Color(0.0f, 1.0f, 0.0f) };
Color const COL_BLUE   { Color(0.4f, 0.4f, 1.0f) };
Color const COL_YELLOW { Color(0.8f, 0.8f, 0.0f) };

void PlayerTypes::Initialize()
{
    m_playerTypes[0] = { CoordPos(  0_COORD,   0_COORD), COL_RED,    L"RED"    };
    m_playerTypes[1] = { CoordPos(MAX_COORD,   0_COORD), COL_GREEN,  L"GREEN"  }; 
    m_playerTypes[2] = { CoordPos(MAX_COORD, MAX_COORD), COL_BLUE,   L"BLUE"   };
    m_playerTypes[3] = { CoordPos(  0_COORD, MAX_COORD), COL_YELLOW, L"YELLOW" };
}

PlayerType const& PlayerTypes::GetPlayerType(PlayerId const id)
{
    return m_playerTypes.at(id.GetValue());
}
