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

const Vei2& Piece::GetEnCroissantSquare() const
{
		return EnCroissantSquare;
}

const Vei2& Piece::GetEnCroissantPawnLoc() const
{
	return EnCroissantPawn;
}

void Pawn::Update(const Vei2& loc)
{
	SetPosition(loc);
	numMoves++;
	if (numMoves == 1 && (pos.y == 4 || pos.y == 3))
	{
		isEnCroissantable = true;
		EnCroissantSquare = this->GetTeam() == Team::WHITE ? loc + Vei2{0, 1} : loc + Vei2{ 0,-1 };
		EnCroissantPawn = loc;
	}
	else
		isEnCroissantable = false;
}

Bishop::Bishop(Team t, const Surface& surf, const Vei2& loc)
	:
	Piece(t, surf, loc)
{}

void Bishop::Draw(Graphics& gfx, const Vei2& loc) const
{
	auto team = this->GetTeam();
	if (team == Team::BLACK)
	{
		gfx.DrawSprite(loc.x, loc.y, { 89,119,2,32 }, s, SpriteEffect::Chroma{ Colors::Red });
	}
	else
	{
		gfx.DrawSprite(loc.x, loc.y, { 90,120,27,57 }, s, SpriteEffect::Chroma{ Colors::Red });
	}
}

Knight::Knight(Team t, const Surface& surf, const Vei2& loc)
	:
	Piece(t, surf, loc)
{}

void Knight::Draw(Graphics& gfx, const Vei2& loc) const
{
	auto team = this->GetTeam();
	if (team == Team::BLACK)
	{
		gfx.DrawSprite(loc.x, loc.y, { 120,150,2,32 }, s, SpriteEffect::Chroma{ Colors::Red });
	}
	else
	{
		gfx.DrawSprite(loc.x, loc.y, { 120,150,27,57 }, s, SpriteEffect::Chroma{ Colors::Red });
	}
}

Rook::Rook(Team t, const Surface& surf, const Vei2& loc)
	:
	Piece(t, surf, loc)
{}

void Rook::Draw(Graphics& gfx, const Vei2& loc) const
{
	auto team = this->GetTeam();
	if (team == Team::BLACK)
	{
		gfx.DrawSprite(loc.x, loc.y, { 60,90,2,32 }, s, SpriteEffect::Chroma{ Colors::Red });
	}
	else
	{
		gfx.DrawSprite(loc.x, loc.y, { 60,90,27,57 }, s, SpriteEffect::Chroma{ Colors::Red });
	}
}

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

