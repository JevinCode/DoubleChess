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

void ChessAI::HandleMoveEvent(bool isBrd1)
{/*
	if (midGame)
	{
		Move();
	}
	else
	{
		if (isBrd1)
		{
			Move({ isBrd1, brd1.moveMade });
		}
		else
		{
			Move({ isBrd1, brd2.moveMade });
		}
	}*/
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
	auto b1moves = GenerateMoves(brd1);
//	auto b2moves = GenerateMoves(brd2);
//	if (brd1.blackInCheck)
//	{
//		std::uniform_int_distribution<int> num(0, b1moves.size() - 1);
//		auto move = b1moves[num(rng)];
//		brd1.Move(move);
//		//handle promotion if applicable
//		brd1.whiteInCheck = brd1.IsWhiteInCheck();
//	}
//	else if (brd2.blackInCheck)
//	{
//		std::uniform_int_distribution<int> num(0, b2moves.size() - 1);
//		auto move = b2moves[num(rng)];
//		brd2.Move(move);
//		//handle promotion if applicable
//		brd2.whiteInCheck = brd2.IsWhiteInCheck();
//	}
//	else
//	{
//		int choice = rand() % 2;
//		if (choice)
//		{
//			std::uniform_int_distribution<int> num(0, b1moves.size() - 1);
//			auto move = b1moves[num(rng)];
//			brd1.Move(move);
//			//handle promotion if applicable
//			brd1.whiteInCheck = brd1.IsWhiteInCheck();
//		}
//		else
//		{
//			std::uniform_int_distribution<int> num(0, b2moves.size() - 1);
//			auto move = b2moves[num(rng)];
//			brd2.Move(move);
//			//handle promotion if applicable
//			brd2.whiteInCheck = brd2.IsWhiteInCheck();
//		}
//	}
}

std::vector<_Move> ChessAI::GenerateMoves(ChessBoard& brd)
{
	return {};
}

int ChessAI::Score(const ChessBoard& brd) const
{
	int score = 0;
	return score;
}
