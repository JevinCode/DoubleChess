#include "Piece.h"

Piece::Piece(Team t, const Surface& surf, const Vei2& pos)
	:
team(t),
s(surf),
pos(pos)
{}

bool Piece::IsCroissant() const
{
	return isEnCroissantable;
}

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

bool Piece::IsValidLoc(const Vei2& loc)
{
	return { loc.x >= 0 && loc.y >= 0 && loc.x < 8 && loc.y < 8 };
}

const Vei2& Piece::GetEnCroissantSquare() const
{
		return EnCroissantSquare;
}

const Vei2& Piece::GetEnCroissantPawnLoc() const
{
	return EnCroissantPawn;
}


