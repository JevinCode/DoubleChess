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

std::vector<Vei2> Rook::GetPossibleMoves(const ChessBoard& brd) const
{
	std::vector<Vei2> ans;
	int x = pos.x - 1;
	while (x >= 0)
	{
		Vei2 i = { x,pos.y };
		if (brd.CellAt(i)->Empty())
		{
			ans.push_back(i);
			x--;
			continue;
		}
		if (brd.CellAt(i)->GetPiece()->GetTeam() != team)
			ans.push_back(i);
		break;
	}
	x = pos.x + 1;
	while (x < 8)
	{
		Vei2 i = { x,pos.y };
		if (brd.CellAt(i)->Empty())
		{
			ans.push_back(i);
			x++;
			continue;
		}
		if (brd.CellAt(i)->GetPiece()->GetTeam() != team)
			ans.push_back(i);
		break;
	}
	int y = pos.y - 1;
	while (y >= 0)
	{
		Vei2 i = { pos.x,y };
		if (brd.CellAt(i)->Empty())
		{
			ans.push_back(i);
			y--;
			continue;
		}
		if (brd.CellAt(i)->GetPiece()->GetTeam() != team)
			ans.push_back(i);
		break;
	}
	y = pos.y + 1;
	while (y < 8)
	{
		Vei2 i = { pos.x,y};
		if (brd.CellAt(i)->Empty())
		{
			ans.push_back(i);
			y++;
			continue;
		}
		if (brd.CellAt(i)->GetPiece()->GetTeam() != team)
			ans.push_back(i);
		break;
	}
	return ans;
}

std::vector<Vei2> Rook::GetPossibleAttackMoves(const ChessBoard& brd) const
{
	std::vector<Vei2> ans;
	int x = pos.x - 1;
	while (x >= 0)
	{
		Vei2 i = { x,pos.y };
		if (brd.CellAt(i)->Empty())
		{
			x--;
			continue;
		}
		if (brd.CellAt(i)->GetPiece()->GetTeam() != team)
			ans.push_back(i);
		break;
	}
	x = pos.x + 1;
	while (x < 8)
	{
		Vei2 i = { x,pos.y };
		if (brd.CellAt(i)->Empty())
		{
			x++;
			continue;
		}
		if (brd.CellAt(i)->GetPiece()->GetTeam() != team)
			ans.push_back(i);
		break;
	}
	int y = pos.y - 1;
	while (y >= 0)
	{
		Vei2 i = { pos.x,y };
		if (brd.CellAt(i)->Empty())
		{
			y--;
			continue;
		}
		if (brd.CellAt(i)->GetPiece()->GetTeam() != team)
			ans.push_back(i);
		break;
	}
	y = pos.y + 1;
	while (y < 8)
	{
		Vei2 i = { pos.x,y };
		if (brd.CellAt(i)->Empty())
		{
			y++;
			continue;
		}
		if (brd.CellAt(i)->GetPiece()->GetTeam() != team)
			ans.push_back(i);
		break;
	}
	return ans;
}
