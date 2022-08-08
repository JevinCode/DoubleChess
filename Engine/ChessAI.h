#pragma once
#include "ChessBoard.h"
#include "Cell.h"
#include <random>
#include <stack>

class ChessAI
{
public:
	ChessAI(Team t, ChessBoard& brd1, ChessBoard& brd2);
	void Move();
private:
	struct CellData
	{
		CellData(const Vei2& l, std::shared_ptr<Piece> p)
			:
			loc(l),
			piece(p)
		{}
		Vei2 loc = {0,0};
		std::shared_ptr<Piece> piece = nullptr;
	};
	struct _Move
	{
		_Move(CellData src, CellData dest)
			:
			dest(dest),
			src(src)
		{}
		CellData src;
		CellData dest;
	};

	Team team = Team::BLACK;
	std::mt19937 rng;
	ChessBoard& brd1;
	ChessBoard& brd2;
	std::stack<_Move> moveTree;

	//Methods
	std::vector<_Move> GenerateMoves(const ChessBoard& brd) const;
	int Score(const ChessBoard& brd) const;
	_Move CalculateMove();
};