#include "Queen.h"

Queen::Queen(Team t, const Surface& surf, const Vei2& loc)
	:
	Piece(t, surf, loc)
{}

void Queen::Draw(Graphics& gfx, const Vei2& loc) const
{
	auto team = this->GetTeam();
	if (team == Team::BLACK)
	{
		gfx.DrawSprite(loc.x, loc.y, { 30,60,2,32 }, s, SpriteEffect::Chroma{ Colors::Red });
	}
	else
	{
		gfx.DrawSprite(loc.x, loc.y, { 30,60,27,57 }, s, SpriteEffect::Chroma{ Colors::Red });
	}
}