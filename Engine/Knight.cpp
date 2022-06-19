#include "Knight.h"
#include "Cell.h"
#include "ChessBoard.h"

Knight::Knight(Team t, const Vei2& loc)
	:
	Piece(t, loc)
{
}

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

std::vector<Vei2> Knight::GetPossibleMoves(const ChessBoard& brd) const
{
	std::vector<Vei2> Moves;
	for (const auto& offset : moveOffsets)
	{
		auto loc = pos + offset;
		if (IsValidLoc(loc))
		{
			auto c = brd.CellAt(loc);
			if(c->Empty() || c->GetPiece()->GetTeam() != team)
				Moves.push_back(loc);
		}
	}
	return Moves;
}

std::vector<Vei2> Knight::GetPossibleAttackMoves(const ChessBoard& brd) const
{
	std::vector<Vei2> Moves;
	for (const auto& offset : moveOffsets)
	{
		auto loc = pos + offset;
		if (IsValidLoc(loc))
		{
			auto c = brd.CellAt(loc);
			if (!c->Empty() && c->GetPiece()->GetTeam() != team)
				Moves.push_back(loc);
		}
	}
	return Moves;
}
