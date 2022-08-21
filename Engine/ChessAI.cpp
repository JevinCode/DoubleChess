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
{
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
	}
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
	auto b2moves = GenerateMoves(brd2);
	if (brd1.blackInCheck)
	{
		std::uniform_int_distribution<int> num(0, b1moves.size() - 1);
		auto move = b1moves[num(rng)];
		brd1.Move(move);
		//handle promotion if applicable
		brd1.whiteInCheck = brd1.IsWhiteInCheck();
	}
	else if (brd2.blackInCheck)
	{
		std::uniform_int_distribution<int> num(0, b2moves.size() - 1);
		auto move = b2moves[num(rng)];
		brd2.Move(move);
		//handle promotion if applicable
		brd2.whiteInCheck = brd2.IsWhiteInCheck();
	}
	else
	{
		int choice = rand() % 2;
		if (choice)
		{
			std::uniform_int_distribution<int> num(0, b1moves.size() - 1);
			auto move = b1moves[num(rng)];
			brd1.Move(move);
			//handle promotion if applicable
			brd1.whiteInCheck = brd1.IsWhiteInCheck();
		}
		else
		{
			std::uniform_int_distribution<int> num(0, b2moves.size() - 1);
			auto move = b2moves[num(rng)];
			brd2.Move(move);
			//handle promotion if applicable
			brd2.whiteInCheck = brd2.IsWhiteInCheck();
		}
	}
}

void ChessAI::Move(const OpeningEngine::OpeningMove& mv)
{
	if (opener->OutOfMoves())
	{
		midGame = true;
		Move();
	}

	auto book = opener->SelectBook(rng, mv);
	bookName = book.name;
	if (opener->OutOfMoves())
	{
		midGame = true;
		Move();
		return;
	}
	if (mv.isBoard1)
		brd1.Move(book.moves[0].move);
	else
		brd2.Move(book.moves[0].move);
}

std::vector<_Move> ChessAI::GenerateMoves(ChessBoard& brd)
{
	std::vector<_Move> AImoves;
	for (int x = 0; x < 8; x++)
	{
		for (int y = 0; y < 8; y++)
		{
			auto cell = brd.CellAt({ x,y });
			if (!cell->Empty() && cell->GetPiece()->GetTeam() == team) 
			{
				Vei2 src = { x,y };
				auto moves = brd.GetValidMoves(src);
				for (const auto& move : moves)
				{
					AImoves.push_back(move);
				}
			}
		}
	}
	return AImoves;
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
