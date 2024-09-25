// Components.cpp
//
// BlokusCore

module BlokusCore:Components;

void Components::Initialize()
{
	initPieceTypes();
	Apply2AllPieceTypes
	(
		[](PieceType& pt)
		{
			BlokusCoordPos pos = pt.GetInitialPos();
			pos.Move2Horz(COORD_BOARD_SIZE + 1);
			pt.SetPos(pos);
		}
	);
}

void Components::initPieceTypes()
{
	m_pieceTypes[0] =
	{{
		{ true, true, true },
		{ true },
		{ true }
	}};
	m_pieceTypes[0].SetPos(0, 0);

	m_pieceTypes[1] =
	{{
		{ true, true, true, true },
		{ true }
	}};
	m_pieceTypes[1].SetPos(4, 0);

	m_pieceTypes[2] =
	{{
		{ false, true },
		{ true,  true,  true },
		{ false, true }
	}};
	m_pieceTypes[2].SetPos(9, 0);

	m_pieceTypes[3] =
	{{
		{ true },
		{ true, true, true },
		{ true }
	}};
	m_pieceTypes[3].SetPos(2, 2);

	m_pieceTypes[4] =
	{{
		{ true },
		{ true,  true },
		{ false, true, true  }
	}};
	m_pieceTypes[4].SetPos(6, 2);

	m_pieceTypes[5] =
	{{
		{ true },
		{ true,  true,  true },
		{ false, false, true }
	}};
	m_pieceTypes[5].SetPos(0, 5);

	m_pieceTypes[6] =
	{{
		{ true,  true },
		{ false, true, true, true }
	}};
	m_pieceTypes[6].SetPos(4, 5);

	m_pieceTypes[7] =
	{{
		{ true, true },
		{ true },
		{ true, true }
	}};
	m_pieceTypes[7].SetPos(10, 4);

	m_pieceTypes[8] =
	{{
		{ true },
		{ true,  true, true },
		{ false, true }
	}};
	m_pieceTypes[8].SetPos(0, 9);

	m_pieceTypes[9] =
	{{
		{ true, true,  true },
		{ true, true  }
	}};
	m_pieceTypes[9].SetPos(4, 8);

	m_pieceTypes[10] =
	{{
		{ true,  true,  true,  true },
		{ false, true }
	}};
	m_pieceTypes[10].SetPos(8, 8);

	m_pieceTypes[11] =
	{{
		{ true, true, true, true, true }
	}};
	m_pieceTypes[11].SetPos(7, 11);


	m_pieceTypes[12] =
	{{
		{ true, true, true },
		{ true }
	}};
	m_pieceTypes[12].SetPos(0, 14);

	m_pieceTypes[13] =
	{{
		{ true },
		{ true, true },
		{ true }
	}};
	m_pieceTypes[13].SetPos(5, 14);

	m_pieceTypes[14] =
	{{
		{ true, true, true, true }
	}};
	m_pieceTypes[14].SetPos(8, 14);

	m_pieceTypes[15] =
	{{
		{ true,  true },
		{ true,  true }
	}};
	m_pieceTypes[15].SetPos(2, 16);

	m_pieceTypes[16] =
	{{
		{ true,  true },
		{ false, true, true }
	}};
	m_pieceTypes[16].SetPos(8, 16);


	m_pieceTypes[17] =
	{{
		{ true },
		{ true, true }
	}};
	m_pieceTypes[17].SetPos(0, 18);

	m_pieceTypes[18] =
	{{
		{ true, true, true }
	}};
	m_pieceTypes[18].SetPos(4, 19);


	m_pieceTypes[19] =
	{{
		{ true, true }
	}};
	m_pieceTypes[19].SetPos(8, 19);


	m_pieceTypes[20] =
	{{
		{ true }
	}};
	m_pieceTypes[20].SetPos(11, 19);

}
