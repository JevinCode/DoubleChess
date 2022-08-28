#include "Cell.h"

Cell::Cell(Cell::Shade s, const Vei2& loc)
	:
	shade(s),
	loc(loc)
{}

void Cell::Highlight(const HighlightType h)
{
	highlight = h;
}

Cell::HighlightType Cell::GetHighlight() const
{
	return highlight;
}

void Cell::ClearHighlight()
{
	highlight = HighlightType::NONE;
}


void Cell::Draw(Graphics& gfx, const Vei2& offset) const
{
	int xDest = loc.x * dimension + offset.x;
	int yDest = (7 - loc.y) * dimension + offset.y;
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
		if (shade == Shade::DARK)
			gfx.DrawSprite(xDest, yDest, { 0,30,0,30 }, s, SpriteEffect::Chroma{ Colors::Magenta });
		else
			gfx.DrawSprite(xDest, yDest, { 30,60,0,30 }, s, SpriteEffect::Chroma{ Colors::Magenta });
		break;
	}
}

Vei2 Cell::GetLoc() const
{
	return loc;
}