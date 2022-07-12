#include "ChessAI.h"

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
		brd1.Move(move.cell, move.loc);
	}
	else
	{
		std::uniform_int_distribution<int> num(0, b2moves.size()-1);
		auto move = b2moves[num(rng)];
		brd2.Move(move.cell, move.loc);
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

int ChessAI::Score()
{
	return 0;
}
