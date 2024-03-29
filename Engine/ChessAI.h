#pragma once
#include "ChessBoard.h"
#include "Cell.h"
#include <random>
#include <stack>
#include "OpeningEngine.h"
#include "UniversalTypes.h"
class ChessAI
{
public:
	ChessAI(Team t, ChessBoard& brd1, ChessBoard& brd2);
	~ChessAI()
	{
		delete opener;
	}
	void HandleMoveEvent(bool isBrd1);
	bool MidGame() const;
	std::string GetBookName() const;
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
	struct _AIMove
	{
		_AIMove(CellData src, CellData dest)
			:
			dest(dest),
			src(src)
		{}
		CellData src;
		CellData dest;
	};

	void Move();
	void Move(const OpeningEngine::OpeningMove& mv);
	OpeningEngine* opener = new OpeningEngine();
	Team team = Team::BLACK;
	std::mt19937 rng;
	ChessBoard& brd1;
	ChessBoard& brd2;
	//std::stack<_Move> moveTree;
	bool midGame = false;
	OpeningEngine::OpeningMove oppMove;
	std::string bookName = "";
	//Methods
	std::vector<_Move> GenerateMoves(ChessBoard& brd); //not const as it makes a call to getValidMoves, which runs a simulation that mutates the board state.
	int Score(const ChessBoard& brd) const;
	//_Move CalculateMove();
};