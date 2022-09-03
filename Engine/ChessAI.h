#pragma once
#include "ChessBoard.h"
#include "Cell.h"
#include <random>
#include <stack>
#include "UniversalTypes.h"
class ChessAI
{
public:
	ChessAI(Team t, ChessBoard& brd1, ChessBoard& brd2);
	void HandleMoveEvent(bool isBrd1);
	bool MidGame() const;
	std::string GetBookName() const;
	Team GetTeam() const;
private:

	void Move();
	Team team = Team::BLACK;
	std::mt19937 rng;
	ChessBoard& brd1;
	ChessBoard& brd2;
	//std::stack<_Move> moveTree;
	bool midGame = false;
	std::string bookName = "";
	//Methods
	int Score(const ChessBoard& brd) const;
	//_Move CalculateMove();
};