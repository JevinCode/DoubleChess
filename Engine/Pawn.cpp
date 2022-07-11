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

std::vector<Vei2> Pawn::GetPossibleMoves(const ChessBoard& brd) const
{
	std::vector<Vei2> ans;
	Vei2 ahead = team == Team::WHITE ? Vei2{ pos.x, pos.y - 1 } : Vei2{ pos.x, pos.y + 1 };
	if (IsValidLoc(ahead) && brd.CellAt(ahead)->Empty())
		ans.push_back( ahead);
	
	Vei2 ahead2 = team == Team::WHITE ? Vei2{ pos.x, pos.y - 2 } : Vei2{ pos.x, pos.y + 2 };
	if (numMoves == 0 && brd.CellAt(ahead2)->Empty() && brd.CellAt(ahead)->Empty())
		ans.push_back(ahead2);

	Vei2 ahead3 = team == Team::WHITE ? Vei2{ pos.x + 1, pos.y - 1 } : Vei2{ pos.x + 1, pos.y + 1 };
	if (IsValidLoc(ahead3))
	{
		auto c = brd.CellAt(ahead3);
		if (!c->Empty() && c->GetPiece()->GetTeam() != team)
			ans.push_back(ahead3);
		//Check for En Passant
		else if (brd.IsEnPassantable() && brd.GetPassantTeam() != team && brd.GetEnPassantSquare() == ahead3)
			ans.push_back(ahead3);
	}

	Vei2 ahead4 = team == Team::WHITE ? Vei2{ pos.x - 1, pos.y - 1 } : Vei2{ pos.x - 1, pos.y + 1 };
	if (IsValidLoc(ahead4))
	{
		if (!brd.CellAt(ahead4)->Empty() && brd.CellAt(ahead4)->GetPiece()->GetTeam() != team)
			ans.push_back(ahead4);
		//Check for En Passant
		else if (brd.IsEnPassantable() && brd.GetPassantTeam() != team && brd.GetEnPassantSquare() == ahead4)
			ans.push_back(ahead4);
	}
	return ans;
}

std::vector<Vei2> Pawn::GetPossibleAttackMoves(const ChessBoard& brd) const
{
	std::vector<Vei2> ans;
	Vei2 ahead3 = team == Team::WHITE ? Vei2{ pos.x + 1, pos.y - 1 } : Vei2{ pos.x + 1, pos.y + 1 };
	if (IsValidLoc(ahead3))
	{
		auto c = brd.CellAt(ahead3);
		if (!c->Empty() && c->GetPiece()->GetTeam() != team)
			ans.push_back(ahead3);
		//Check for En Passant
		else if (brd.IsEnPassantable() && brd.GetPassantTeam() != team && brd.GetEnPassantSquare() == ahead3)
			ans.push_back(ahead3);
	}

	Vei2 ahead4 = team == Team::WHITE ? Vei2{ pos.x - 1, pos.y - 1 } : Vei2{ pos.x - 1, pos.y + 1 };
	if (IsValidLoc(ahead4))
	{
		auto c = brd.CellAt(ahead4);
		if (!c->Empty() && c->GetPiece()->GetTeam() != team)
			ans.push_back(ahead4);
		//Check for En Passant
		else if (brd.IsEnPassantable() && brd.GetPassantTeam() != team && brd.GetEnPassantSquare() == ahead4)
			ans.push_back(ahead4);
	}
	return ans;
}
