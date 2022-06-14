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


ChessBoard::Cell::Cell(Cell::Shade s, const Vei2& loc, const Surface& surf)
	:
	shade(s),
	loc(loc),
	s(surf)
{
}



void ChessBoard::Cell::DrawCell(Graphics& gfx, const Vei2& offset)
{
	int xDest = loc.x * dimension + offset.x;
	int yDest = loc.y * dimension + offset.y;
	if (shade == Shade::DARK)
	{
		gfx.DrawSprite(xDest, yDest, { 0,30,0,30 }, s, SpriteEffect::Chroma{ Colors::Magenta });
	}
	else
	{
		gfx.DrawSprite(xDest, yDest, { 30,60,0,30 }, s, SpriteEffect::Chroma{ Colors::Magenta });
	}
}
