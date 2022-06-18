#include "Pawn.h"


Pawn::Pawn(Team t, const Surface& surf, const Vei2& loc)
	:
	Piece(t, surf, loc)
{}

void Pawn::Draw(Graphics& gfx, const Vei2& loc) const
{
	auto team = this->GetTeam();
	if (team == Team::BLACK)
	{
		gfx.DrawSprite(loc.x, loc.y, { 147,177,3,33 }, s, SpriteEffect::Chroma{ Colors::Red });
	}
	else
	{
		gfx.DrawSprite(loc.x, loc.y, { 147,177,27,57 }, s, SpriteEffect::Chroma{ Colors::Red });
	}
}

void Pawn::Update(const Vei2& loc)
{
	SetPosition(loc);
	numMoves++;
	if (numMoves == 1 && (pos.y == 4 || pos.y == 3))
	{
		isEnCroissantable = true;
		EnCroissantSquare = this->GetTeam() == Team::WHITE ? loc + Vei2{ 0, 1 } : loc + Vei2{ 0,-1 };
		EnCroissantPawn = loc;
	}
	else
		isEnCroissantable = false;
}