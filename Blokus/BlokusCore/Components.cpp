// Components.cpp
//
// BlokusCore

module BlokusCore:Components;

void Components::Initialize()
{
	initPieceTypes();
}

void Components::initPieceTypes()
{
	m_pieceTypes[0] =
	{{
		{ true, true, true, true },
		{ true }
	}};
	m_pieceTypes[0].SetPos(24, 0);

	m_pieceTypes[1] =
	{{
		{ true,  true,  true,  true },
		{ false, true }
	}};
	m_pieceTypes[1].SetPos(28, 8);

	m_pieceTypes[2] =
	{{
		{ true, true, true, true, true }
	}};
	m_pieceTypes[2].SetPos(27, 11);

	m_pieceTypes[3] =
	{{
		{ true, true, true },
		{ true },
		{ true }
	}};
	m_pieceTypes[3].SetPos(20, 0);

	m_pieceTypes[4] =
	{{
		{ true },
		{ true,  true },
		{ false, true, true  }
	}};
	m_pieceTypes[4].SetPos(26, 2);

	m_pieceTypes[5] =
	{{
		{ false, true },
		{ true,  true,  true },
		{ false, true }
	}};
	m_pieceTypes[5].SetPos(29, 0);

	m_pieceTypes[6] =
	{{
		{ true },
		{ true,  true, true },
		{ false, true }
	}};
	m_pieceTypes[6].SetPos(20, 9);

	m_pieceTypes[7] =
	{{
		{ true,  true },
		{ false, true, true, true }
	}};
	m_pieceTypes[7].SetPos(24, 5);

	m_pieceTypes[8] =
	{{
		{ true },
		{ true, true, true },
		{ true }
	}};
	m_pieceTypes[8].SetPos(22, 2);

	m_pieceTypes[9] =
	{{
		{ true },
		{ true,  true,  true },
		{ false, false, true }
	}};
	m_pieceTypes[9].SetPos(20, 5);

	m_pieceTypes[10] =
	{{
		{ true, true },
		{ true },
		{ true, true }
	}};
	m_pieceTypes[10].SetPos(30, 4);

	m_pieceTypes[11] =
	{{
		{ true, true,  true },
		{ true, true  }
	}};
	m_pieceTypes[11].SetPos(24, 8);


	m_pieceTypes[12] =
	{{
		{ true,  true },
		{ true,  true }
	}};
	m_pieceTypes[12].SetPos(22, 16);

	m_pieceTypes[13] =
	{{
		{ true,  true },
		{ false, true, true }
	}};
	m_pieceTypes[13].SetPos(28, 16);

	m_pieceTypes[14] =
	{{
		{ true, true, true },
		{ true }
	}};
	m_pieceTypes[14].SetPos(20, 14);

	m_pieceTypes[15] =
	{{
		{ true },
		{ true, true },
		{ true }
	}};
	m_pieceTypes[15].SetPos(25, 14);

	m_pieceTypes[16] =
	{{
		{ true, true, true, true }
	}};
	m_pieceTypes[16].SetPos(28, 14);


	m_pieceTypes[17] =
	{{
		{ true },
		{ true, true }
	}};
	m_pieceTypes[17].SetPos(20, 18);

	m_pieceTypes[18] =
	{{
		{ true, true, true }
	}};
	m_pieceTypes[18].SetPos(24, 19);


	m_pieceTypes[19] =
	{{
		{ true, true }
	}};
	m_pieceTypes[19].SetPos(28, 19);


	m_pieceTypes[20] =
	{{
		{ true }
	}};
	m_pieceTypes[20].SetPos(31, 19);

}
