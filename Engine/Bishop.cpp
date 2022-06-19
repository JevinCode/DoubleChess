#include "Bishop.h"
#include "ChessBoard.h"
#include "Cell.h"

Bishop::Bishop(Team t, const Vei2& loc)
	:
	Piece(t, loc)
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

std::vector<Vei2> Bishop::GetPossibleMoves(const ChessBoard& brd) const
{
	std::vector<Vei2> ans;

	Vei2 i = { pos.x + 1, pos.y - 1 };
	while (IsValidLoc(i))
	{
		if (brd.CellAt(i)->Empty())
		{
			ans.push_back(i);
			i += {1, -1};
			continue;
		}
		if (brd.CellAt(i)->GetPiece()->GetTeam() != team)
			ans.push_back(i);
		break;
	}
	i = { pos.x + 1, pos.y + 1 };
	while (IsValidLoc(i))
	{
		if (brd.CellAt(i)->Empty())
		{
			ans.push_back(i);
			i += {1, 1};
			continue;
		}
		if (brd.CellAt(i)->GetPiece()->GetTeam() != team)
			ans.push_back(i);
		break;
	}
	i = { pos.x - 1, pos.y - 1 };
	while (IsValidLoc(i))
	{
		if (brd.CellAt(i)->Empty())
		{
			ans.push_back(i);
			i += {-1, -1};
			continue;
		}
		if (brd.CellAt(i)->GetPiece()->GetTeam() != team)
			ans.push_back(i);
		break;
	}
	i = { pos.x - 1, pos.y + 1 };
	while (IsValidLoc(i))
	{
		if (brd.CellAt(i)->Empty())
		{
			ans.push_back(i);
			i += {-1, 1};
			continue;
		}
		if (brd.CellAt(i)->GetPiece()->GetTeam() != team)
			ans.push_back(i);
		break;
	}
	return ans;
}

std::vector<Vei2> Bishop::GetPossibleAttackMoves(const ChessBoard& brd) const
{
	std::vector<Vei2> ans;

	Vei2 i = { pos.x + 1, pos.y - 1 };
	while (IsValidLoc(i))
	{
		if (brd.CellAt(i)->Empty())
		{
			i += {1, -1};
			continue;
		}
		if (brd.CellAt(i)->GetPiece()->GetTeam() != team)
			ans.push_back(i);
		break;
	}
	i = { pos.x + 1, pos.y + 1 };
	while (IsValidLoc(i))
	{
		if (brd.CellAt(i)->Empty())
		{
			i += {1, 1};
			continue;
		}
		if (brd.CellAt(i)->GetPiece()->GetTeam() != team)
			ans.push_back(i);
		break;
	}
	i = { pos.x - 1, pos.y - 1 };
	while (IsValidLoc(i))
	{
		if (brd.CellAt(i)->Empty())
		{
			i += {-1, -1};
			continue;
		}
		if (brd.CellAt(i)->GetPiece()->GetTeam() != team)
			ans.push_back(i);
		break;
	}
	i = { pos.x - 1, pos.y + 1 };
	while (IsValidLoc(i))
	{
		if (brd.CellAt(i)->Empty())
		{
			i += {-1, 1};
			continue;
		}
		if (brd.CellAt(i)->GetPiece()->GetTeam() != team)
			ans.push_back(i);
		break;
	}
	return ans;
}
