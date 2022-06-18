#include "King.h"

King::King(Team t, const Surface& surf, const Vei2& loc)
	:
	Piece(t, surf, loc)
{}

void King::Draw(Graphics& gfx, const Vei2& loc) const
{
	auto team = this->GetTeam();
	if (team == Team::BLACK)
	{
		gfx.DrawSprite(loc.x, loc.y, { 0,30,2,32 }, s, SpriteEffect::Chroma{ Colors::Red });
	}
	else
	{
		gfx.DrawSprite(loc.x, loc.y, { 0,30,27,57 }, s, SpriteEffect::Chroma{ Colors::Red });
	}
}