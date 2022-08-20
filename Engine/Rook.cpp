#include "Rook.h"
#include "ChessBoard.h"
#include "Cell.h"

Rook::Rook(Team t, const Vei2& loc)
	:
	Piece(t, loc)
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

std::vector<_Move> Rook::GetPossibleMoves(const ChessBoard& brd) const
{
	std::vector<_Move> ans;
	int x = pos.x - 1;
	while (x >= 0)
	{
		Vei2 i = { x,pos.y };
		if (brd.CellAt(i)->Empty())
		{
			ans.push_back({ pos,i,MoveType::Normal });
			x--;
			continue;
		}
		if (brd.CellAt(i)->GetPiece()->GetTeam() != team)
			ans.push_back({ pos,i,MoveType::Normal });
		break;
	}
	x = pos.x + 1;
	while (x < 8)
	{
		Vei2 i = { x,pos.y };
		if (brd.CellAt(i)->Empty())
		{
			ans.push_back({ pos,i,MoveType::Normal });
			x++;
			continue;
		}
		if (brd.CellAt(i)->GetPiece()->GetTeam() != team)
			ans.push_back({ pos,i,MoveType::Normal });
		break;
	}
	int y = pos.y - 1;
	while (y >= 0)
	{
		Vei2 i = { pos.x,y };
		if (brd.CellAt(i)->Empty())
		{
			ans.push_back({ pos,i,MoveType::Normal });
			y--;
			continue;
		}
		if (brd.CellAt(i)->GetPiece()->GetTeam() != team)
			ans.push_back({ pos,i,MoveType::Normal });
		break;
	}
	y = pos.y + 1;
	while (y < 8)
	{
		Vei2 i = { pos.x,y};
		if (brd.CellAt(i)->Empty())
		{
			ans.push_back({ pos,i,MoveType::Normal });
			y++;
			continue;
		}
		if (brd.CellAt(i)->GetPiece()->GetTeam() != team)
			ans.push_back({ pos,i,MoveType::Normal });
		break;
	}
	return ans;
}

std::vector<_Move> Rook::GetPossibleAttackMoves(const ChessBoard& brd) const
{
	return GetPossibleMoves(brd);
}
