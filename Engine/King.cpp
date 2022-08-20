#include "King.h"
#include "ChessBoard.h"
#include "Cell.h"

King::King(Team t, const Vei2& loc)
	:
	Piece(t, loc)
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

std::vector<Vei2> King::GetPossibleMoves(const ChessBoard& brd) const
{
	std::vector<Vei2> ans;
	std::vector<Vei2> moves = { {pos.x - 1, pos.y}, {pos.x - 1, pos.y - 1}, {pos.x - 1, pos.y + 1}, {pos.x, pos.y - 1}, {pos.x, pos.y + 1}, {pos.x + 1, pos.y + 1}, {pos.x + 1, pos.y}, {pos.x + 1, pos.y - 1} };
	for (const auto& move : moves)
	{
		if (IsValidLoc(move))
		{
			auto c = brd.CellAt(move);
			if (c->Empty() || c->GetPiece()->GetTeam() != team)
				ans.push_back(move);
		}
	}
	return ans;
}

std::vector<Vei2> King::GetPossibleAttackMoves(const ChessBoard& brd) const
{
	return GetPossibleMoves(brd);
}


