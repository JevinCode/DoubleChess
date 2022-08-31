#pragma once
#include "Bencher.h"
#include "ChessBoard.h"

class PseudoLegalMoveGenerator
{
public:
	static std::vector<_Move> GenerateMoves(Team t, const ChessBoard& brd);
	static std::vector<_Move> GenerateKnightMoves(Team t, const ChessBoard& brd);
	static std::vector<_Move> GenerateSlidingMoves(Team t, const ChessBoard& brd); 
	static std::vector<_Move> GeneratePawnMoves(Team t, const ChessBoard& brd);
	//static std::vector<_Move> GenerateKingMoves(Team t, const ChessBoard& brd);

private:
	static BitBoard SinglePushTargetsWhite(const ChessBoard& brd);
	static BitBoard DoublePushTargetsWhite(const ChessBoard& brd);
	static BitBoard SinglePushTargetsBlack(const ChessBoard& brd);
	static BitBoard DoublePushTargetsBlack(const ChessBoard& brd); 


};