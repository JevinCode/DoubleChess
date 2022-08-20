#include "Cell.h"

Cell::Cell(Cell::Shade s, const Vei2& loc)
	:
	shade(s),
	loc(loc)
{}

Cell::Cell(Cell::Shade s, const Vei2 & loc, std::shared_ptr<Piece> p)
	:
	shade(s),
	loc(loc),
	piece(p)
{}

void Cell::Highlight(const HighlightType h)
{
	highlight = h;
}

Cell::HighlightType Cell::GetHighlight() const
{
	return highlight;
}

std::shared_ptr<Piece> Cell::OnClick(Team PlayerTurn)
{
	if (!Empty() && PlayerTurn == piece->GetTeam())
	{
		Highlight(HighlightType::BLUE);
		return piece;
	}
	return nullptr;
}

void Cell::ReleaseHighlight()
{
	highlight = HighlightType::NONE;
}

void Cell::Clear()
{
	piece = nullptr;
}

std::shared_ptr<Piece> Cell::GetPiece() const
{
	if(!Empty())
		return piece;
	return nullptr;
}

bool Cell::Empty() const
{
	return piece == nullptr;
}

void Cell::DrawCell(Graphics& gfx, const Vei2& offset)
{
	int xDest = loc.x * dimension + offset.x;
	int yDest = offset.y - ((1 + loc.y) * dimension);
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

void Cell::GivePiece(std::shared_ptr<Piece> peace)
{
	piece = peace;
}

Vei2 Cell::GetLoc() const
{
	return loc;
}
