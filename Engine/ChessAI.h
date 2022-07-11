#pragma once
#include "ChessBoard.h"

class ChessAI
{
public:
	ChessAI(Team t, ChessBoard& brd);
private:
	Team team = Team::BLACK;
	int Score();
	ChessBoard& brd;
};