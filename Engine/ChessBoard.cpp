#include "ChessBoard.h"

ChessBoard::ChessBoard()
{
	for (int row = 0; row < 8; row++)
	{
		Cell::Shade startShade = row % 2 ? Cell::Shade::DARK : Cell::Shade::LIGHT;
		for (int col = 0; col < 8; col++)
		{
			Vei2 loc = { row,col };
			auto sh = Cell::Shade(((int)startShade + col) % 2);
			cells[LinearizeCoords(loc)] = std::make_unique<Cell>(sh, loc, tiles);
		}
	}
	for (int x = 0; x < 8; x++)
	{
		int i = LinearizeCoords({ x,1 });
		cells[i]->piece = std::make_unique<Pawn>(Team::BLACK, sPieces, Vei2{ x,1 });
		cells[i]->p = Pieces::BLACK_PAWN;
	}
	cells[LinearizeCoords({ 0,0 })]->piece = std::make_unique<Rook>(Team::BLACK, sPieces, Vei2{ 0,0 });
	cells[LinearizeCoords({ 1,0 })]->piece = std::make_unique<Knight>(Team::BLACK, sPieces, Vei2{ 1,0 });
	cells[LinearizeCoords({ 2,0 })]->piece = std::make_unique<Bishop>(Team::BLACK, sPieces, Vei2{ 2,0 });
	cells[LinearizeCoords({ 3,0 })]->piece = std::make_unique<Queen>(Team::BLACK, sPieces, Vei2{ 3,0 });
	cells[LinearizeCoords({ 4,0 })]->piece = std::make_unique<King>(Team::BLACK, sPieces, Vei2{4,0 });
	cells[LinearizeCoords({ 5,0 })]->piece = std::make_unique<Bishop>(Team::BLACK, sPieces, Vei2{ 5,0 });
	cells[LinearizeCoords({ 6,0 })]->piece = std::make_unique<Knight>(Team::BLACK, sPieces, Vei2{ 6,0 });
	cells[LinearizeCoords({ 7,0 })]->piece = std::make_unique<Rook>(Team::BLACK, sPieces, Vei2{ 7,0 });

	cells[LinearizeCoords({ 0,0 })]->p = Pieces::BLACK_ROOK;
	cells[LinearizeCoords({ 1,0 })]->p = Pieces::BLACK_KNIGHT;
	cells[LinearizeCoords({ 2,0 })]->p = Pieces::BLACK_BISHOP;
	cells[LinearizeCoords({ 3,0 })]->p = Pieces::BLACK_QUEEN;
	cells[LinearizeCoords({ 4,0 })]->p = Pieces::BLACK_KING;
	cells[LinearizeCoords({ 5,0 })]->p = Pieces::BLACK_BISHOP;
	cells[LinearizeCoords({ 6,0 })]->p = Pieces::BLACK_KNIGHT;
	cells[LinearizeCoords({ 7,0 })]->p = Pieces::BLACK_ROOK;

	for (int x = 0; x < 8; x++)
	{
		int i = LinearizeCoords({ x,6 });
		cells[i]->piece = std::make_unique<Pawn>(Team::WHITE, sPieces, Vei2{ x,6 });
		cells[i]->p = Pieces::WHITE_PAWN;
	}
	cells[LinearizeCoords({ 0,7 })]->piece = std::make_unique<Rook>(Team::WHITE, sPieces, Vei2{ 0,7 });
	cells[LinearizeCoords({ 1,7 })]->piece = std::make_unique<Knight>(Team::WHITE, sPieces, Vei2{ 1,7 });
	cells[LinearizeCoords({ 2,7 })]->piece = std::make_unique<Bishop>(Team::WHITE, sPieces, Vei2{ 2,7 });
	cells[LinearizeCoords({ 3,7 })]->piece = std::make_unique<Queen>(Team::WHITE, sPieces, Vei2{ 3,7 });
	cells[LinearizeCoords({ 4,7 })]->piece = std::make_unique<King>(Team::WHITE, sPieces, Vei2{ 4,7 });
	cells[LinearizeCoords({ 5,7 })]->piece = std::make_unique<Bishop>(Team::WHITE, sPieces, Vei2{ 5,77 });
	cells[LinearizeCoords({ 6,7 })]->piece = std::make_unique<Knight>(Team::WHITE, sPieces, Vei2{ 6,7 });
	cells[LinearizeCoords({ 7,7 })]->piece = std::make_unique<Rook>(Team::WHITE, sPieces, Vei2{ 7,7 });

	cells[LinearizeCoords({ 0,7 })]->p = Pieces::WHITE_ROOK;
	cells[LinearizeCoords({ 1,7 })]->p = Pieces::WHITE_KNIGHT;
	cells[LinearizeCoords({ 2,7 })]->p = Pieces::WHITE_BISHOP;
	cells[LinearizeCoords({ 3,7 })]->p = Pieces::WHITE_QUEEN;
	cells[LinearizeCoords({ 4,7 })]->p = Pieces::WHITE_KING;
	cells[LinearizeCoords({ 5,7 })]->p = Pieces::WHITE_BISHOP;
	cells[LinearizeCoords({ 6,7 })]->p = Pieces::WHITE_KNIGHT;
	cells[LinearizeCoords({ 7,7 })]->p = Pieces::WHITE_ROOK;
	
}

void ChessBoard::Draw(Graphics& gfx) const
{
	for (const auto& c : cells)
	{
		c->DrawCell(gfx, topLeft);
	}
}

int ChessBoard::LinearizeCoords(const Vei2& loc)
{
	return loc.x + 8 * loc.y;
}

Vei2 ChessBoard::Dimensify(int loc)
{
	return { loc % 8, loc / 8 };
}

Vei2 ChessBoard::GetOffset() const
{
	return topLeft;
}

void ChessBoard::ReleaseHighlights()
{
	for (auto& c : cells)
	{
		c->ReleaseHighlight();
	}
}

bool ChessBoard::IsValidLoc(const Vei2& loc) const
{
	if (loc.x >= 0 && loc.x < 8 && loc.y >= 0 && loc.y < 8)
		return true;
	return false;
}

std::vector<std::pair<Vei2,ChessBoard::Cell::HighlightType>> ChessBoard::GetValidMoves(const Vei2& loc, const Pieces p ) const
{
	std::vector<std::pair<Vei2,Cell::HighlightType>> ans;
	auto croissantSquare = cells[LinearizeCoords(loc)]->piece->GetEnCroissantSquare();
	switch (p)
	{
		case Pieces::BLACK_PAWN:
		{
			Vei2 ahead = { loc.x, loc.y + 1 };
			if (IsValidLoc(ahead) && cells[LinearizeCoords(ahead)]->Empty())
				ans.push_back({{ loc.x, loc.y + 1 },Cell::HighlightType::YELLOW});

			Vei2 ahead2 = { loc.x, 3 };
			if (loc.y == 1 && cells[LinearizeCoords(ahead2)]->Empty() && cells[LinearizeCoords(ahead)]->Empty())
				ans.push_back({ahead2,Cell::HighlightType::YELLOW});

			Vei2 ahead3 = { loc.x + 1, loc.y + 1 };
			if (IsValidLoc(ahead3))
			{
				int i = LinearizeCoords(ahead3);
				if (!cells[i]->Empty() && cells[i]->piece->GetTeam() == Team::WHITE)
					ans.push_back({ ahead3,Cell::HighlightType::RED });
				//Check for En Passant
				else if (cells[LinearizeCoords(loc)]->piece->IsCroissant() && croissantSquare == ahead3)
					ans.push_back({ ahead3,Cell::HighlightType::RED });
			}

			Vei2 ahead4 = { loc.x - 1, loc.y + 1 };
			if (IsValidLoc(ahead4))
			{
				int i = LinearizeCoords(ahead4);
				if (!cells[i]->Empty() && cells[i]->piece->GetTeam() == Team::WHITE)
					ans.push_back({ ahead4,Cell::HighlightType::RED });
				//Check for En Passant
				else if (cells[LinearizeCoords(loc)]->piece->IsCroissant() && croissantSquare == ahead4)
					ans.push_back({ ahead4,Cell::HighlightType::RED });
			}
			break;
		}
		case Pieces::WHITE_PAWN:
		{
			Vei2 ahead = { loc.x, loc.y - 1 };
			if (IsValidLoc(ahead) && cells[LinearizeCoords(ahead)]->Empty())
				ans.push_back({ahead,Cell::HighlightType::YELLOW});

			Vei2 ahead2 = { loc.x, 4 };
			if (loc.y == 6 && cells[LinearizeCoords(ahead2)]->Empty() && cells[LinearizeCoords(ahead)]->Empty())
				ans.push_back({ahead2,Cell::HighlightType::YELLOW});

			Vei2 ahead3 = { loc.x + 1, loc.y - 1 };
			if (IsValidLoc(ahead3))
			{
				int i = LinearizeCoords(ahead3);
				if (!cells[i]->Empty() && cells[i]->piece->GetTeam() == Team::BLACK)
					ans.push_back({ahead3,Cell::HighlightType::RED});
				//Check for En Passant
				else if (cells[LinearizeCoords(loc)]->piece->IsCroissant() && croissantSquare == ahead3)
					ans.push_back({ahead3,Cell::HighlightType::RED});
			}																										   
																																				   
			Vei2 ahead4 = { loc.x - 1, loc.y - 1 };
			if (IsValidLoc(ahead4))
			{
				int i = LinearizeCoords(ahead4);
				if (!cells[i]->Empty() && cells[i]->piece->GetTeam() == Team::BLACK)
					ans.push_back({ ahead4,Cell::HighlightType::RED });
				//Check for En Passant
				else if (cells[LinearizeCoords(loc)]->piece->IsCroissant() && croissantSquare == ahead4)
					ans.push_back({ ahead4,Cell::HighlightType::RED });
			}
			break;
		}
		case Pieces::BLACK_KNIGHT:
		{
			std::vector<Vei2> moves = { {loc.x - 2, loc.y - 1},{loc.x - 1, loc.y - 2}, {loc.x + 1, loc.y - 2}, {loc.x + 2, loc.y - 1}, {loc.x + 2, loc.y + 1}, {loc.x + 1, loc.y + 2}, {loc.x - 1, loc.y + 2}, {loc.x - 2, loc.y + 1} };
			for (auto l : moves)
			{
				if (IsValidLoc(l))
				{
					int i = LinearizeCoords(l);
					if (cells[i]->Empty())
						ans.push_back({ l,Cell::HighlightType::YELLOW });
					else if (cells[i]->piece->GetTeam() == Team::WHITE)
						ans.push_back({ l,Cell::HighlightType::RED });
				}
			}
			break;
		}
		case Pieces::WHITE_KNIGHT:
		{
			std::vector<Vei2> moves = { {loc.x - 2, loc.y - 1},{loc.x - 1, loc.y - 2}, {loc.x + 1, loc.y - 2}, {loc.x + 2, loc.y - 1}, {loc.x + 2, loc.y + 1}, {loc.x + 1, loc.y + 2}, {loc.x - 1, loc.y + 2}, {loc.x - 2, loc.y + 1} };
			for (auto l : moves)
			{
				if (IsValidLoc(l))
				{
					int i = LinearizeCoords(l);
					if (cells[i]->Empty())
						ans.push_back({ l,Cell::HighlightType::YELLOW });
					else if (cells[i]->piece->GetTeam() == Team::BLACK)
						ans.push_back({ l,Cell::HighlightType::RED });
				}
			}
			break;
		}
		case Pieces::BLACK_ROOK:
		{
			int x = loc.x - 1;
			while (x >= 0)
			{
				int i = LinearizeCoords({ x,loc.y });
				if (cells[i]->Empty())
				{
					ans.push_back({{ x,loc.y },Cell::HighlightType::YELLOW});
					x--;
					continue;
				}
				if (cells[i]->piece->GetTeam() == Team::BLACK)
					break;
				ans.push_back({{ x,loc.y },Cell::HighlightType::RED});
				break;
			}
			x = loc.x + 1;
			while (x < 8)
			{
				int i = LinearizeCoords({ x,loc.y });
				if (cells[i]->Empty())
				{
					ans.push_back({{ x,loc.y },Cell::HighlightType::YELLOW});
					x++;
					continue;
				}
				if (cells[i]->piece->GetTeam() == Team::BLACK)
					break;
				ans.push_back({{ x,loc.y },Cell::HighlightType::RED});
				break;
			}
			int y = loc.y - 1;
			while (y >= 0)
			{
				int i = LinearizeCoords({ loc.x,y });
				if (cells[i]->Empty())
				{
					ans.push_back({{ loc.x,y },Cell::HighlightType::YELLOW});
					y--;
					continue;
				}
				if (cells[i]->piece->GetTeam() == Team::BLACK)
					break;
				ans.push_back({{ loc.x,y },Cell::HighlightType::RED});
				break;
			}
			y = loc.y + 1;
			while (y < 8)
			{
				int i = LinearizeCoords({ loc.x,y });
				if (cells[i]->Empty())
				{
					ans.push_back({{ loc.x,y },Cell::HighlightType::YELLOW});
					y++;
					continue;
				}
				if (cells[i]->piece->GetTeam() == Team::BLACK)
					break;
				ans.push_back({{ loc.x,y },Cell::HighlightType::RED});
				break;
			}
			break;
		}
		case Pieces::WHITE_ROOK:
		{
			int x = loc.x - 1;
			while (x >= 0)
			{
				int i = LinearizeCoords({ x,loc.y });
				if (cells[i]->Empty())
				{
					ans.push_back({{ x,loc.y },Cell::HighlightType::YELLOW});
					x--;
					continue;
				}
				if (cells[i]->piece->GetTeam() == Team::WHITE)
					break;
				ans.push_back({{ x,loc.y },Cell::HighlightType::RED});
				break;
			}
			x = loc.x + 1;
			while (x < 8)
			{
				int i = LinearizeCoords({ x,loc.y });
				if (cells[i]->Empty())
				{
					ans.push_back({{ x,loc.y },Cell::HighlightType::YELLOW});
					x++;
					continue;
				}
				if (cells[i]->piece->GetTeam() == Team::WHITE)
					break;
				ans.push_back({{ x,loc.y },Cell::HighlightType::RED});
				break;
			}
			int y = loc.y - 1;
			while (y >= 0)
			{
				int i = LinearizeCoords({ loc.x,y });
				if (cells[i]->Empty())
				{
					ans.push_back({{ loc.x,y },Cell::HighlightType::YELLOW});
					y--;
					continue;
				}
				if (cells[i]->piece->GetTeam() == Team::WHITE)
					break;
				ans.push_back({{ loc.x,y },Cell::HighlightType::RED});
				break;
			}
			y = loc.y + 1;
			while (y < 8)
			{
				int i = LinearizeCoords({ loc.x,y });
				if (cells[i]->Empty())
				{
					ans.push_back({{ loc.x,y },Cell::HighlightType::YELLOW});
					y++;
					continue;
				}
				if (cells[i]->piece->GetTeam() == Team::WHITE)
					break;
				ans.push_back({{ loc.x,y },Cell::HighlightType::RED});
				break;
			}
			break;
		}
		case Pieces::BLACK_BISHOP:
		{
			Vei2 pos = { loc.x + 1, loc.y - 1 };
			while (IsValidLoc(pos))
			{
				int i = LinearizeCoords(pos);
				if (cells[i]->Empty())
				{
					ans.push_back({pos,Cell::HighlightType::YELLOW});
					pos += {1, -1};
					continue;
				}
				if (cells[i]->piece->GetTeam() == Team::BLACK)
					break;
				ans.push_back({pos,Cell::HighlightType::RED});
				break;
			}
			pos = { loc.x - 1, loc.y - 1 };
			while (IsValidLoc(pos))
			{
				int i = LinearizeCoords(pos);
				if (cells[i]->Empty())
				{
					ans.push_back({pos,Cell::HighlightType::YELLOW});
					pos += {-1, -1};
					continue;
				}
				if (cells[i]->piece->GetTeam() == Team::BLACK)
					break;
				ans.push_back({pos,Cell::HighlightType::RED});
				break;
			}
			pos = { loc.x + 1, loc.y + 1 };
			while (IsValidLoc(pos))
			{
				int i = LinearizeCoords(pos);
				if (cells[i]->Empty())
				{
					ans.push_back({pos,Cell::HighlightType::YELLOW});
					pos += {1, 1};
					continue;
				}
				if (cells[i]->piece->GetTeam() == Team::BLACK)
					break;
				ans.push_back({pos,Cell::HighlightType::RED});
				break;
			}
			pos = { loc.x - 1, loc.y + 1 };
			while (IsValidLoc(pos))
			{
				int i = LinearizeCoords(pos);
				if (cells[i]->Empty())
				{
					ans.push_back({pos,Cell::HighlightType::YELLOW});
					pos += {-1, 1};
					continue;
				}
				if (cells[i]->piece->GetTeam() == Team::BLACK)
					break;
				ans.push_back({pos,Cell::HighlightType::RED});
				break;
			}
			break;
		}
		case Pieces::WHITE_BISHOP:
		{
			Vei2 pos = { loc.x + 1, loc.y - 1 };
			while (IsValidLoc(pos))
			{
				int i = LinearizeCoords(pos);
				if (cells[i]->Empty())
				{
					ans.push_back({pos,Cell::HighlightType::YELLOW});
					pos += {1, -1};
					continue;
				}
				if (cells[i]->piece->GetTeam() == Team::WHITE)
					break;
				ans.push_back({pos,Cell::HighlightType::RED});
				break;
			}
			pos = { loc.x - 1, loc.y - 1 };
			while (IsValidLoc(pos))
			{
				int i = LinearizeCoords(pos);
				if (cells[i]->Empty())
				{
					ans.push_back({pos,Cell::HighlightType::YELLOW});
					pos += {-1, -1};
					continue;
				}
				if (cells[i]->piece->GetTeam() == Team::WHITE)
					break;
				ans.push_back({pos,Cell::HighlightType::RED});
				break;
			}
			pos = { loc.x + 1, loc.y + 1 };
			while (IsValidLoc(pos))
			{
				int i = LinearizeCoords(pos);
				if (cells[i]->Empty())
				{
					ans.push_back({pos,Cell::HighlightType::YELLOW});
					pos += {1, 1};
					continue;
				}
				if (cells[i]->piece->GetTeam() == Team::WHITE)
					break;
				ans.push_back({pos,Cell::HighlightType::RED});
				break;
			}
			pos = { loc.x - 1, loc.y + 1 };
			while (IsValidLoc(pos))
			{
				int i = LinearizeCoords(pos);
				if (cells[i]->Empty())
				{
					ans.push_back({pos,Cell::HighlightType::YELLOW});
					pos += {-1, 1};
					continue;
				}
				if (cells[i]->piece->GetTeam() == Team::WHITE)
					break;
				ans.push_back({pos,Cell::HighlightType::RED});
				break;
			}
			break;
		}
		case Pieces::BLACK_QUEEN:
		{
			int x = loc.x - 1;
			while (x >= 0)
			{
				int i = LinearizeCoords({ x,loc.y });
				if (cells[i]->Empty())
				{
					ans.push_back({{ x,loc.y },Cell::HighlightType::YELLOW});
					x--;
					continue;
				}
				if (cells[i]->piece->GetTeam() == Team::BLACK)
					break;
				ans.push_back({{ x,loc.y },Cell::HighlightType::RED});
				break;
			}
			x = loc.x + 1;
			while (x < 8)
			{
				int i = LinearizeCoords({ x,loc.y });
				if (cells[i]->Empty())
				{
					ans.push_back({{ x,loc.y },Cell::HighlightType::YELLOW});
					x++;
					continue;
				}
				if (cells[i]->piece->GetTeam() == Team::BLACK)
					break;
				ans.push_back({{ x,loc.y },Cell::HighlightType::RED});
				break;
			}
			int y = loc.y - 1;
			while (y >= 0)
			{
				int i = LinearizeCoords({ loc.x,y });
				if (cells[i]->Empty())
				{
					ans.push_back({{ loc.x,y },Cell::HighlightType::YELLOW});
					y--;
					continue;
				}
				if (cells[i]->piece->GetTeam() == Team::BLACK)
					break;
				ans.push_back({{ loc.x,y },Cell::HighlightType::RED});
				break;
			}
			y = loc.y + 1;
			while (y < 8)
			{
				int i = LinearizeCoords({ loc.x,y });
				if (cells[i]->Empty())
				{
					ans.push_back({{ loc.x,y },Cell::HighlightType::YELLOW});
					y++;
					continue;
				}
				if (cells[i]->piece->GetTeam() == Team::BLACK)
					break;
				ans.push_back({{ loc.x,y },Cell::HighlightType::RED});
				break;
			}
			Vei2 pos = { loc.x + 1, loc.y - 1 };
			while (IsValidLoc(pos))
			{
				int i = LinearizeCoords(pos);
				if (cells[i]->Empty())
				{
					ans.push_back({pos,Cell::HighlightType::YELLOW});
					pos += {1, -1};
					continue;
				}
				if (cells[i]->piece->GetTeam() == Team::BLACK)
					break;
				ans.push_back({pos,Cell::HighlightType::RED});
				break;
			}
			pos = { loc.x - 1, loc.y - 1 };
			while (IsValidLoc(pos))
			{
				int i = LinearizeCoords(pos);
				if (cells[i]->Empty())
				{
					ans.push_back({pos,Cell::HighlightType::YELLOW});
					pos += {-1, -1};
					continue;
				}
				if (cells[i]->piece->GetTeam() == Team::BLACK)
					break;
				ans.push_back({pos,Cell::HighlightType::RED});
				break;
			}
			pos = { loc.x + 1, loc.y + 1 };
			while (IsValidLoc(pos))
			{
				int i = LinearizeCoords(pos);
				if (cells[i]->Empty())
				{
					ans.push_back({pos,Cell::HighlightType::YELLOW});
					pos += {1, 1};
					continue;
				}
				if (cells[i]->piece->GetTeam() == Team::BLACK)
					break;
				ans.push_back({pos,Cell::HighlightType::RED});
				break;
			}
			pos = { loc.x - 1, loc.y + 1 };
			while (IsValidLoc(pos))
			{
				int i = LinearizeCoords(pos);
				if (cells[i]->Empty())
				{
					ans.push_back({pos,Cell::HighlightType::YELLOW});
					pos += {-1, 1};
					continue;
				}
				if (cells[i]->piece->GetTeam() == Team::BLACK)
					break;
				ans.push_back({pos,Cell::HighlightType::RED});
				break;
			}
			break;
		}
		case Pieces::WHITE_QUEEN:
		{
			int x = loc.x - 1;
			while (x >= 0)
			{
				int i = LinearizeCoords({ x,loc.y });
				if (cells[i]->Empty())
				{
					ans.push_back({{ x,loc.y },Cell::HighlightType::YELLOW});
					x--;
					continue;
				}
				if (cells[i]->piece->GetTeam() == Team::WHITE)
					break;
				ans.push_back({{ x,loc.y },Cell::HighlightType::RED});
				break;
			}
			x = loc.x + 1;
			while (x < 8)
			{
				int i = LinearizeCoords({ x,loc.y });
				if (cells[i]->Empty())
				{
					ans.push_back({{ x,loc.y },Cell::HighlightType::YELLOW});
					x++;
					continue;
				}
				if (cells[i]->piece->GetTeam() == Team::WHITE)
					break;
				ans.push_back({{ x,loc.y },Cell::HighlightType::RED});
				break;
			}
			int y = loc.y - 1;
			while (y >= 0)
			{
				int i = LinearizeCoords({ loc.x,y });
				if (cells[i]->Empty())
				{
					ans.push_back({{ loc.x,y },Cell::HighlightType::YELLOW});
					y--;
					continue;
				}
				if (cells[i]->piece->GetTeam() == Team::WHITE)
					break;
				ans.push_back({{ loc.x,y },Cell::HighlightType::RED});
				break;
			}
			y = loc.y + 1;
			while (y < 8)
			{
				int i = LinearizeCoords({ loc.x,y });
				if (cells[i]->Empty())
				{
					ans.push_back({{ loc.x,y },Cell::HighlightType::YELLOW});
					y++;
					continue;
				}
				if (cells[i]->piece->GetTeam() == Team::WHITE)
					break;
				ans.push_back({{ loc.x,y },Cell::HighlightType::RED});
				break;
			}
			Vei2 pos = { loc.x + 1, loc.y - 1 };
			while (IsValidLoc(pos))
			{
				int i = LinearizeCoords(pos);
				if (cells[i]->Empty())
				{
					ans.push_back({pos,Cell::HighlightType::YELLOW});
					pos += {1, -1};
					continue;
				}
				if (cells[i]->piece->GetTeam() == Team::WHITE)
					break;
				ans.push_back({pos,Cell::HighlightType::RED});
				break;
			}
			pos = { loc.x - 1, loc.y - 1 };
			while (IsValidLoc(pos))
			{
				int i = LinearizeCoords(pos);
				if (cells[i]->Empty())
				{
					ans.push_back({pos,Cell::HighlightType::YELLOW});
					pos += {-1, -1};
					continue;
				}
				if (cells[i]->piece->GetTeam() == Team::WHITE)
					break;
				ans.push_back({pos,Cell::HighlightType::RED});
				break;
			}
			pos = { loc.x + 1, loc.y + 1 };
			while (IsValidLoc(pos))
			{
				int i = LinearizeCoords(pos);
				if (cells[i]->Empty())
				{
					ans.push_back({pos,Cell::HighlightType::YELLOW});
					pos += {1, 1};
					continue;
				}
				if (cells[i]->piece->GetTeam() == Team::WHITE)
					break;
				ans.push_back({pos,Cell::HighlightType::RED});
				break;
			}
			pos = { loc.x - 1, loc.y + 1 };
			while (IsValidLoc(pos))
			{
				int i = LinearizeCoords(pos);
				if (cells[i]->Empty())
				{
					ans.push_back({pos,Cell::HighlightType::YELLOW});
					pos += {-1, 1};
					continue;
				}
				if (cells[i]->piece->GetTeam() == Team::WHITE)
					break;
				ans.push_back({pos,Cell::HighlightType::RED});
				break;
			}
			break;
		}
		case Pieces::BLACK_KING:
		{
			std::vector<Vei2> moves = { {loc.x - 1, loc.y}, {loc.x - 1, loc.y - 1}, {loc.x - 1, loc.y + 1}, {loc.x, loc.y - 1}, {loc.x, loc.y + 1}, {loc.x + 1, loc.y + 1}, {loc.x + 1, loc.y}, {loc.x + 1, loc.y - 1} };
			for (const auto& move : moves)
			{
				if (IsValidLoc(move))
				{
					if (cells[LinearizeCoords(move)]->Empty())
						ans.push_back({ move,Cell::HighlightType::YELLOW });
					else if (cells[LinearizeCoords(move)]->piece->GetTeam() == Team::WHITE)
						ans.push_back({ move,Cell::HighlightType::RED });
				}
			}
			break;
		}
		case Pieces::WHITE_KING:
		{
			std::vector<Vei2> moves = { {loc.x - 1, loc.y}, {loc.x - 1, loc.y - 1}, {loc.x - 1, loc.y + 1}, {loc.x, loc.y - 1}, {loc.x, loc.y + 1}, {loc.x + 1, loc.y + 1}, {loc.x + 1, loc.y}, {loc.x + 1, loc.y - 1} };
			for (const auto& move : moves)
			{
				if (IsValidLoc(move))
				{
					if (cells[LinearizeCoords(move)]->Empty())
						ans.push_back({ move,Cell::HighlightType::YELLOW });
					else if (cells[LinearizeCoords(move)]->piece->GetTeam() == Team::BLACK)
						ans.push_back({ move,Cell::HighlightType::RED });
				}
			}
			break;
		}
	}
	return ans;
}

void ChessBoard::Move(std::unique_ptr<Piece> piece, const Vei2& loc)
{
	int i = LinearizeCoords(loc);
	cells[i]->GivePiece(std::move(piece));
	if (cells[i]->piece->IsCroissant() && loc == cells[i]->piece->GetEnCroissantSquare() && typeid(*cells[i]->piece) == typeid(Pawn))
	{
		auto croissantloc = cells[i]->piece->GetEnCroissantPawnLoc();
		cells[LinearizeCoords(croissantloc)]->Clear();
	}
	cells[LinearizeCoords(loc)]->piece->Update(loc);
}
void ChessBoard::OnClick(const Vei2& loc)
{
	int coord = LinearizeCoords(loc);
	if (cells[coord]->highlight == Cell::HighlightType::YELLOW || cells[coord]->highlight == Cell::HighlightType::RED)
	{
		int i = LinearizeCoords(cellPreviouslyHighlighted);
		Move(std::move(cells[i]->piece), loc);
		//if(pace == Pieces::WHITE_PAWN)
		cells[i]->Clear();
		ReleaseHighlights();
		PlayerTurn = (Team)(((int)PlayerTurn + 1) % 2);
		return;
	}
	ReleaseHighlights();
	auto pieceType = cells[coord]->OnClick(PlayerTurn);
	if (pieceType != ChessBoard::Pieces::NONE)
	{
		cellPreviouslyHighlighted = loc;
		auto moveTargets = GetValidMoves(loc, pieceType);
		for (const auto& loc : moveTargets)
		{
			int i = LinearizeCoords(loc.first);
			cells[i]->Highlight(loc.second);
		}
	}
}

void ChessBoard::Cell::Highlight(const HighlightType h)
{
	highlight = h;
}

 ChessBoard::Pieces ChessBoard::Cell::OnClick(Team PlayerTurn)
{
	 if (!Empty() && PlayerTurn == this->piece->GetTeam())
	 {
		 Highlight(HighlightType::BLUE);
		 return this->p;
	 }
	 return Pieces::NONE;
}

void ChessBoard::Cell::ReleaseHighlight()
{
	highlight = HighlightType::NONE;
}

void ChessBoard::Cell::Clear()
{
	piece = nullptr;
	p = ChessBoard::Pieces::NONE;
}

ChessBoard::Cell::Cell(Cell::Shade s, const Vei2& loc, const Surface& surf)
	:
	shade(s),
	loc(loc),
	s(surf)
{}


bool ChessBoard::Cell::Empty() const
{
	return piece == nullptr;
}


void ChessBoard::Cell::DrawCell(Graphics& gfx, const Vei2& offset)
{
	int xDest = loc.x * dimension + offset.x;
	int yDest = loc.y * dimension + offset.y;
	switch (highlight)
	{
	case HighlightType::BLUE:
		gfx.DrawSprite(xDest, yDest, { 0,30,0,30 }, s, SpriteEffect::Substitution{ Colors::Magenta, Colors::Blue });
		break;
	case HighlightType::YELLOW:
		gfx.DrawSprite(xDest, yDest, { 0,30,0,30 }, s, SpriteEffect::SolidAlpha{ Colors::Yellow, Colors::Magenta, 0.7f });
		break;
	case HighlightType::RED:
		gfx.DrawSprite(xDest, yDest, { 0,30,0,30 }, s, SpriteEffect::Substitution{ Colors::Magenta, Colors::Red });
		break;
	default:
	{
		if (shade == Shade::DARK)
		{
			gfx.DrawSprite(xDest, yDest, { 0,30,0,30 }, s, SpriteEffect::Chroma{ Colors::Magenta });
		}
		else
		{
			gfx.DrawSprite(xDest, yDest, { 30,60,0,30 }, s, SpriteEffect::Chroma{ Colors::Magenta });
		}
		break;
	}
	}
	if (!Empty())
	{
		piece->Draw(gfx, { xDest, yDest });
	}
}

void ChessBoard::Cell::GivePiece( std::unique_ptr<Piece> peace)
{
	piece = std::move(peace);
	if (typeid(*piece) == typeid(Pawn))
	{
		p = piece->GetTeam() == Team::WHITE ? Pieces::WHITE_PAWN : Pieces::BLACK_PAWN;
	}
	if (typeid(*piece) == typeid(Knight))
	{
		p = piece->GetTeam() == Team::WHITE ? Pieces::WHITE_KNIGHT : Pieces::BLACK_KNIGHT;
	}
	if (typeid(*piece) == typeid(Bishop))
	{
		p = piece->GetTeam() == Team::WHITE ? Pieces::WHITE_BISHOP : Pieces::BLACK_BISHOP;
	}
	if (typeid(*piece) == typeid(Rook))
	{
		p = piece->GetTeam() == Team::WHITE ? Pieces::WHITE_ROOK : Pieces::BLACK_ROOK;
	}
	if (typeid(*piece) == typeid(Queen))
	{
		p = piece->GetTeam() == Team::WHITE ? Pieces::WHITE_QUEEN : Pieces::BLACK_QUEEN;
	}
	if (typeid(*piece) == typeid(King))
	{
		p = piece->GetTeam() == Team::WHITE ? Pieces::WHITE_KING : Pieces::BLACK_KING;
	}
}

