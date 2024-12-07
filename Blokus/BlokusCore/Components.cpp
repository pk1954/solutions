// Components.cpp
//
// BlokusCore

module BlokusCore:Components;

void Components::Initialize()
{
    // Init ´player types

	Color const COL_RED    { Color(1.0f, 0.2f, 0.2f) };
	Color const COL_GREEN  { Color(0.0f, 1.0f, 0.0f) };
	Color const COL_BLUE   { Color(0.4f, 0.4f, 1.0f) };
	Color const COL_YELLOW { Color(0.8f, 0.8f, 0.0f) };

    m_playerTypes[0] = { CoordPos(  0_COORD,   0_COORD), COL_RED,    L"RED"    };
    m_playerTypes[1] = { CoordPos(MAX_COORD,   0_COORD), COL_GREEN,  L"GREEN"  }; 
    m_playerTypes[2] = { CoordPos(MAX_COORD, MAX_COORD), COL_BLUE,   L"BLUE"   };
    m_playerTypes[3] = { CoordPos(  0_COORD, MAX_COORD), COL_YELLOW, L"YELLOW" };

	// Init shapes

	m_pieceTypes[0].SetShape
	({{
		{ true, true, true },
		{ true },
		{ true }
	}});

	m_pieceTypes[1].SetShape
	({{
		{ true, true, true, true },
		{ true }
	}});

	m_pieceTypes[2].SetShape
	({{
		{ false, true },
		{ true,  true,  true },
		{ false, true }
	}});

	m_pieceTypes[3].SetShape
	({{
		{ true },
		{ true, true, true },
		{ true }
	}});

	m_pieceTypes[4].SetShape
	({{
		{ true },
		{ true,  true },
		{ false, true, true  }
	}});

	m_pieceTypes[5].SetShape
	({{
		{ true },
		{ true,  true,  true },
		{ false, false, true }
	}});

	m_pieceTypes[6].SetShape
	({{
		{ true,  true },
		{ false, true, true, true }
	}});

	m_pieceTypes[7].SetShape
	({{
		{ true, true },
		{ true },
		{ true, true }
	}});

	m_pieceTypes[8].SetShape
	({{
		{ true },
		{ true,  true, true },
		{ false, true }
	}});

	m_pieceTypes[9].SetShape
	({{
		{ true, true,  true },
		{ true, true  }
	}});

	m_pieceTypes[10].SetShape
	({{
		{ true,  true,  true,  true },
		{ false, true }
	}});

	m_pieceTypes[11].SetShape
	({{
		{ true, true, true, true, true }
	}});

	m_pieceTypes[12].SetShape
	({{
		{ true, true, true },
		{ true }
	}});

	m_pieceTypes[13].SetShape
	({{
		{ true },
		{ true, true },
		{ true }
	}});

	m_pieceTypes[14].SetShape
	({{
		{ true, true, true, true }
	}});

	m_pieceTypes[15].SetShape
	({{
		{ true,  true },
		{ true,  true }
	}});

	m_pieceTypes[16].SetShape
	({{
		{ true,  true },
		{ false, true, true }
	}});

	m_pieceTypes[17].SetShape
	({{
		{ true },
		{ true, true }
	}});

	m_pieceTypes[18].SetShape
	({{
		{ true, true, true }
	}});

	m_pieceTypes[19].SetShape
	({{
		{ true, true }
	}});

	m_pieceTypes[20].SetShape
	({{
		{ true }
	}});

	// Init positions

	m_pieceTypes [0].SetPos(BOARD_SIZE +  1,  0);
	m_pieceTypes [1].SetPos(BOARD_SIZE +  5,  0);
	m_pieceTypes [2].SetPos(BOARD_SIZE + 10,  0);
	m_pieceTypes [3].SetPos(BOARD_SIZE +  3,  2);
	m_pieceTypes [4].SetPos(BOARD_SIZE +  7,  2);
	m_pieceTypes [5].SetPos(BOARD_SIZE +  1,  5);
	m_pieceTypes [6].SetPos(BOARD_SIZE +  5,  5);
	m_pieceTypes [7].SetPos(BOARD_SIZE + 11,  4);
	m_pieceTypes [8].SetPos(BOARD_SIZE +  1,  9);
	m_pieceTypes [9].SetPos(BOARD_SIZE +  5,  8);
	m_pieceTypes[10].SetPos(BOARD_SIZE +  9,  8);
	m_pieceTypes[11].SetPos(BOARD_SIZE +  8, 11);
	m_pieceTypes[12].SetPos(BOARD_SIZE +  1, 14);
	m_pieceTypes[13].SetPos(BOARD_SIZE +  6, 14);
	m_pieceTypes[14].SetPos(BOARD_SIZE +  9, 14);
	m_pieceTypes[15].SetPos(BOARD_SIZE +  3, 16);
	m_pieceTypes[16].SetPos(BOARD_SIZE +  9, 16);
	m_pieceTypes[17].SetPos(BOARD_SIZE +  1, 18);
	m_pieceTypes[18].SetPos(BOARD_SIZE +  5, 19);
	m_pieceTypes[19].SetPos(BOARD_SIZE +  9, 19);
	m_pieceTypes[20].SetPos(BOARD_SIZE + 12, 19);
}
