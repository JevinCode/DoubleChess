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

void ChessAI::HandleMoveEvent()
{
	brd1.GenerateMoves(GetTeam());
	brd2.GenerateMoves(GetTeam());
	Move();
}

bool ChessAI::MidGame() const
{
	return midGame;
}

std::string ChessAI::GetBookName() const
{
	return bookName;
}
void ChessAI::Move()
{
	auto b1moves = brd1.userPossibleMoves;
	auto b2moves = brd2.userPossibleMoves;
	if (brd1.IsInCheck(team))
	{
		assert(b1moves.size() > 0);
		std::uniform_int_distribution<int> num(0, b1moves.size() - 1);
		auto move = b1moves[num(rng)];
		brd1.ApplyMove(move);
		//handle promotion if applicable
	}
	else if (brd2.IsInCheck(team))
	{
		assert(b2moves.size() > 0);
		std::uniform_int_distribution<int> num(0, b2moves.size() - 1);
		auto move = b2moves[num(rng)];
		brd2.ApplyMove(move);
		//handle promotion if applicable
	}
	else
	{
		int choice = rand() % 2;
		if (choice)
		{
			std::uniform_int_distribution<int> num(0, b1moves.size() - 1);
			auto move = b1moves[num(rng)];
			brd1.ApplyMove(move);
			//handle promotion if applicable
		}
		else
		{
			std::uniform_int_distribution<int> num(0, b2moves.size() - 1);
			auto move = b2moves[num(rng)];
			brd2.ApplyMove(move);
			//handle promotion if applicable
		}
	}
}

Team ChessAI::GetTeam() const
{
	return team;
}


int ChessAI::Score(const ChessBoard& brd) const
{
	int score = 0;
	return score;
}
