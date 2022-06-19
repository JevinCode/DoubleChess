#include "Piece.h"

Piece::Piece(Team t, const Vei2& pos)
	:
team(t),
pos(pos)
{}

void Piece::Update(const Vei2& loc)
{
	SetPosition(loc);
	isEnCroissantable = false;
}

const Vei2& Piece::GetPosition() const
{
	return pos;
}

void Piece::SetPosition(const Vei2& loc)
{
	this->pos = loc;
}

Team Piece::GetTeam() const
{
	return team;
}

bool Piece::IsValidLoc(const Vei2& loc) const
{
	return { loc.x >= 0 && loc.y >= 0 && loc.x < 8 && loc.y < 8 };
}


