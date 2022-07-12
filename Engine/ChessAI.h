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
			loc(Vei2{ 0,0 })
		{
			cell = nullptr;
		}
		_Move(Vei2 loc, std::shared_ptr<Cell> c)
			:
			loc(loc),
			cell(c)
		{}
		std::shared_ptr<Cell> cell;
		Vei2 loc;
	};
	Team team = Team::BLACK;

	std::mt19937 rng;
	std::vector<_Move> GenerateMoves(const ChessBoard& brd) const;
	int Score();
	ChessBoard& brd1;
	ChessBoard& brd2;
};