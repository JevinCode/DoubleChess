#include "Pawn.h"
#include "ChessBoard.h"
#include "Cell.h"

Pawn::Pawn(Team t, const Vei2& loc)
	:
	Piece(t, loc)
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

std::vector<_Move> Pawn::GetPossibleMoves(const ChessBoard& brd) const
{
	std::vector<_Move> ans;
	if (team == Team::WHITE)
	{
		Vei2 ahead = { pos.x,pos.y + 1 };
		Vei2 aheadLeft = { pos.x - 1, pos.y + 1 };
		Vei2 aheadRight = { pos.x + 1, pos.y + 1 };
		if (brd.CellAt(ahead)->Empty())
		{
			if (pos.y == 6)
			{
				ans.push_back({ pos, ahead, MoveType::QueenPromotion });
				ans.push_back({ pos, ahead, MoveType::KnightPromotion });
			}
			else
				ans.push_back({ pos, ahead, MoveType::Normal });
		}
		if (IsValidLoc(aheadLeft) && !brd.CellAt(aheadLeft)->Empty() && brd.CellAt(aheadLeft)->GetPiece()->GetTeam() != team)
		{
			if (pos.y == 6)
			{
				ans.push_back({ pos, aheadLeft, MoveType::QueenPromotion });
				ans.push_back({ pos, aheadLeft, MoveType::KnightPromotion });
			}
			else
				ans.push_back({ pos, aheadLeft, MoveType::Normal });
		}
		if (IsValidLoc(aheadRight) && !brd.CellAt(aheadRight)->Empty() && brd.CellAt(aheadRight)->GetPiece()->GetTeam() != team)
		{
			if (pos.y == 6)
			{
				ans.push_back({ pos, aheadRight, MoveType::QueenPromotion });
				ans.push_back({ pos, aheadRight, MoveType::KnightPromotion });
			}
			else
				ans.push_back({ pos, aheadRight, MoveType::Normal });
		}
		if (IsValidLoc(aheadLeft) && brd.IsEnPassantable() && brd.GetEnPassantSquare() == aheadLeft)
			ans.push_back({ pos, aheadLeft, MoveType::EnPassant });
		if (IsValidLoc(aheadRight) && brd.IsEnPassantable() && brd.GetEnPassantSquare() == aheadRight)
			ans.push_back({ pos, aheadRight, MoveType::EnPassant });
	}
	else
	{
		Vei2 ahead = { pos.x, pos.y - 1 };
		Vei2 aheadLeft = { pos.x - 1, pos.y - 1 };
		Vei2 aheadRight = { pos.x + 1, pos.y - 1 };
		if (brd.CellAt(ahead)->Empty())
		{
			if (pos.y == 1)
			{
				ans.push_back({ pos, ahead, MoveType::QueenPromotion });
				ans.push_back({ pos, ahead, MoveType::KnightPromotion });
			}
			else
				ans.push_back({ pos, ahead, MoveType::Normal });
		}
		if (IsValidLoc(aheadLeft) && !brd.CellAt(aheadLeft)->Empty() && brd.CellAt(aheadLeft)->GetPiece()->GetTeam() != team)
		{
			if (pos.y == 1)
			{
				ans.push_back({ pos, aheadLeft, MoveType::QueenPromotion });
				ans.push_back({ pos, aheadLeft, MoveType::KnightPromotion });
			}
			else
				ans.push_back({ pos, aheadLeft, MoveType::Normal });
		}
		if (IsValidLoc(aheadRight) && !brd.CellAt(aheadRight)->Empty() && brd.CellAt(aheadRight)->GetPiece()->GetTeam() != team)
		{
			if (pos.y == 1)
			{
				ans.push_back({ pos, aheadRight, MoveType::QueenPromotion });
				ans.push_back({ pos, aheadRight, MoveType::KnightPromotion });
			}
			else
				ans.push_back({ pos, aheadRight, MoveType::Normal });
		}
		if (IsValidLoc(aheadLeft) && brd.IsEnPassantable() && brd.GetEnPassantSquare() == aheadLeft)
			ans.push_back({ pos, aheadLeft, MoveType::EnPassant });
		if (IsValidLoc(aheadRight) && brd.IsEnPassantable() && brd.GetEnPassantSquare() == aheadRight)
			ans.push_back({ pos, aheadRight, MoveType::EnPassant });
	}
	
	Vei2 ahead2 = team == Team::WHITE ? Vei2{ pos.x, pos.y + 2 } : Vei2{ pos.x, pos.y - 2 };
	if (!hasMoved && brd.CellAt(ahead2)->Empty() && brd.CellAt(ahead2)->Empty())
		ans.push_back({ pos, ahead2, MoveType::Normal });
	return ans;
}

std::vector<_Move> Pawn::GetPossibleAttackMoves(const ChessBoard& brd) const
{
	std::vector<_Move> ans;
	Vei2 ahead3 = team == Team::BLACK ? Vei2{ pos.x + 1, pos.y - 1 } : Vei2{ pos.x + 1, pos.y + 1 };
	if (IsValidLoc(ahead3))
		ans.push_back({ pos,ahead3,MoveType::Normal });

	Vei2 ahead4 = team == Team::BLACK ? Vei2{ pos.x - 1, pos.y - 1 } : Vei2{ pos.x - 1, pos.y + 1 };
	if (IsValidLoc(ahead4))
		ans.push_back({ pos,ahead4,MoveType::Normal });
	return ans;
}
