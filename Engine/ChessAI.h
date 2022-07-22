#pragma once
#include "ChessBoard.h"
#include "Cell.h"
#include <random>

class ChessAI
{
public:
	ChessAI(Team t, ChessBoard& brd1, ChessBoard& brd2);
	void Move();
private:
	struct _Move
	{
		_Move()
			:
			dest(Vei2{ 0,0 })
		{
			src = nullptr;
		}
		_Move(Vei2 dest, std::shared_ptr<Cell> c)
			:
			dest(dest),
			src(c)
		{}
		std::shared_ptr<Cell> src;
		Vei2 dest;
	};
	Team team = Team::BLACK;

	std::mt19937 rng;
	std::vector<_Move> GenerateMoves(const ChessBoard& brd) const;
	int Score(const ChessBoard& brd) const;
	ChessBoard& brd1;
	ChessBoard& brd2;
};