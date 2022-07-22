#include "Piece.h"
#include "Chessboard.h"
#include "Cell.h"

Piece::Piece(Team t, const Vei2& pos)
	:
team(t),
pos(pos)
{}

void Piece::Update(const Vei2& loc)
{
	SetPosition(loc);
	numMoves++;
	hasMoved = true;
}

const Vei2& Piece::GetPosition() const
{
	return pos;
}

void Piece::SetPosition(const Vei2& loc)
{
	this->pos = loc;
}

bool Piece::HasMoved() const
{
	return hasMoved;
}

Team Piece::GetTeam() const
{
	return team;
}

int Piece::GetNumMoves()
{
	return numMoves;
}

int Piece::GetNumAdjDefenders(const ChessBoard& brd) const
{
	int nPieces = 0;
	Vei2 locs[8] = {
	{ pos.x - 1, pos.y - 1 },
	{ pos.x - 1, pos.y },
	{ pos.x - 1, pos.y + 1 },
	{ pos.x, pos.y - 1 },
	{ pos.x, pos.y + 1 },
	{ pos.x + 1, pos.y - 1 },
	{ pos.x + 1, pos.y },
	{ pos.x + 1, pos.y + 1 } };

	for (const auto& loc : locs)
		if (IsValidLoc(loc) && !brd.CellAt(loc)->Empty() && brd.CellAt(loc)->GetPiece()->GetTeam() == team)
			nPieces++;

	return nPieces;
}

bool Piece::IsValidLoc(const Vei2& loc) const
{
	return { loc.x >= 0 && loc.y >= 0 && loc.x < 8 && loc.y < 8 };
}


