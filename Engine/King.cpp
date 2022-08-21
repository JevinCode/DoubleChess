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

std::vector<_Move> King::GetPossibleMoves(const ChessBoard& brd) const
{
	std::vector<_Move> ans;
	std::vector<Vei2> moves = { {pos.x - 1, pos.y}, {pos.x - 1, pos.y - 1}, {pos.x - 1, pos.y + 1}, {pos.x, pos.y - 1}, {pos.x, pos.y + 1}, {pos.x + 1, pos.y + 1}, {pos.x + 1, pos.y}, {pos.x + 1, pos.y - 1} };
	for (const auto& move : moves)
	{
		if (IsValidLoc(move))
		{
			auto c = brd.CellAt(move);
			if (c->Empty() || c->GetPiece()->GetTeam() != team)
				ans.push_back({ pos,move,MoveType::Normal });
		}
	}
	if (brd.CanCastleKingside(team))
	{
		if (team == Team::WHITE)
			ans.push_back({ pos,{6,0},MoveType::KingsideCastle });
		else
			ans.push_back({ pos,{6,7},MoveType::KingsideCastle });
	}
	if (brd.CanCastleQueenside(team))
	{
		if (team == Team::WHITE)
			ans.push_back({ pos,{2,0},MoveType::QueensideCastle });
		else
			ans.push_back({ pos,{2,7},MoveType::QueensideCastle });
	}
	return ans;
}

std::vector<_Move> King::GetPossibleAttackMoves(const ChessBoard& brd) const
{
	std::vector<_Move> ans;
	std::vector<Vei2> moves = { {pos.x - 1, pos.y}, {pos.x - 1, pos.y - 1}, {pos.x - 1, pos.y + 1}, {pos.x, pos.y - 1}, {pos.x, pos.y + 1}, {pos.x + 1, pos.y + 1}, {pos.x + 1, pos.y}, {pos.x + 1, pos.y - 1} };
	for (const auto& move : moves)
	{
		if (IsValidLoc(move))
		{
			auto c = brd.CellAt(move);
			if (c->Empty() || c->GetPiece()->GetTeam() != team)
				ans.push_back({ pos,move,MoveType::Normal });
		}
	}
	return ans;
}


