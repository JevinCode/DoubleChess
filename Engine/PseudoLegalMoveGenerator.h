#pragma once

#include "ChessBoard.h"
#include "UniversalTypes.h"

class PseudoLegalMoveGenerator
{
public:
	static std::vector<_Move> GenerateMoves(Team t, const ChessBoard& brd);
	static std::vector<_Move> GenerateKnightMoves(Team t, const ChessBoard& brd);
	static std::vector<_Move> GenerateSlidingMoves(Team t, const ChessBoard& brd);
private:
	static BitBoard GetPositiveRayAttack(BitBoard occupied, Direction dir, ChessBoard::Square square, const ChessBoard& brd);

	static BitBoard GetNegativeRayAttack(BitBoard occupied, Direction dir, ChessBoard::Square square, const ChessBoard& brd);
};