#include "ChessAI.h"
#include <typeinfo>
ChessAI::ChessAI(Team t, ChessBoard& brd1, ChessBoard& brd2)
	:
	team(t),
	brd1(brd1),
	brd2(brd2)
{
	std::random_device rd;
	rng = std::mt19937(rd());
}

void ChessAI::Move()
{
	auto b1moves = GenerateMoves(brd1);
	auto b2moves = GenerateMoves(brd2);
	int choice = rand() % 2;
	if (choice)
	{
		std::uniform_int_distribution<int> num(0, b1moves.size()-1);
		auto move = b1moves[num(rng)];
		brd1.Move(move.src, move.dest);
	}
	else
	{
		std::uniform_int_distribution<int> num(0, b2moves.size()-1);
		auto move = b2moves[num(rng)];
		brd2.Move(move.src, move.dest);
	}
}

std::vector<ChessAI::_Move> ChessAI::GenerateMoves(const ChessBoard& brd) const
{
	std::vector<ChessAI::_Move> moves;
	for (int x = 0; x < 8; x++)
	{
		for (int y = 0; y < 8; y++)
		{
			auto cell = brd.CellAt({ x,y });
			if (!cell->Empty() && cell->GetPiece()->GetTeam() == team) 
			{
				auto locs = brd.CellAt({ x,y })->GetPiece()->GetPossibleMoves(brd);
				for (auto loc : locs)
				{
					moves.emplace_back(ChessAI::_Move{ loc, brd.CellAt({x,y}) });
				}
			}
		}
	}
	return moves;
}

int ChessAI::Score(const ChessBoard& brd) const
{
	int score = 0;
	for (const auto& cell : brd.cells)
	{
		if (!cell->Empty())
		{
			auto p = cell->GetPiece();
			auto t = p->GetTeam();
			auto& tid = typeid(*p);
			if (tid == typeid(Pawn))
				score += t == team ? 2 : -2;
			else if (tid == typeid(Knight))
				score += t == team ? 6 : -6;
			else if (tid == typeid(Bishop))
				score += t == team ? 6 : -6;
			else if (tid == typeid(Rook))
				score += t == team ? 10 : -10;
			else if (tid == typeid(Queen))
				score += t == team ? 18 : -18;
			else if (tid == typeid(King) && t == team)
			{
				score += p->GetNumAdjDefenders(brd);
			}
		}
	}
	return score;
}
