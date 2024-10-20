// Components.cpp
//
// BlokusCore

module BlokusCore:Components;

void Components::Initialize()
{
	initShapes();
	initPositions();
}

void Components::initShapes()
{
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
}

void Components::initPositions()
{
	m_pieceTypes [0].SetPos( 0,  0);
	m_pieceTypes [1].SetPos( 4,  0);
	m_pieceTypes [2].SetPos( 9,  0);
	m_pieceTypes [3].SetPos( 2,  2);
	m_pieceTypes [4].SetPos( 6,  2);
	m_pieceTypes [5].SetPos( 0,  5);
	m_pieceTypes [6].SetPos( 4,  5);
	m_pieceTypes [7].SetPos(10,  4);
	m_pieceTypes [8].SetPos( 0,  9);
	m_pieceTypes [9].SetPos( 4,  8);
	m_pieceTypes[10].SetPos( 8,  8);
	m_pieceTypes[11].SetPos( 7, 11);
	m_pieceTypes[12].SetPos( 0, 14);
	m_pieceTypes[13].SetPos( 5, 14);
	m_pieceTypes[14].SetPos( 8, 14);
	m_pieceTypes[15].SetPos( 2, 16);
	m_pieceTypes[16].SetPos( 8, 16);
	m_pieceTypes[17].SetPos( 0, 18);
	m_pieceTypes[18].SetPos( 4, 19);
	m_pieceTypes[19].SetPos( 8, 19);
	m_pieceTypes[20].SetPos(11, 19);

	Apply2AllPieceTypes
	(
		[](PieceType& pt)
		{
			CoordPos pos = pt.GetInitialPos();
			pos.Move2Horz(COORD_BOARD_SIZE + 1);
			pt.SetPos(pos);
		}
	);
}
