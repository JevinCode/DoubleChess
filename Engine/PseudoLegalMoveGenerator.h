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

	static BitBoard SinglePushTargetsWhite(BitBoard wPawns, BitBoard empty);
	static BitBoard DoublePushTargetsWhite(BitBoard wPawns, BitBoard empty);
	static BitBoard SinglePushTargetsBlack(BitBoard bPawns, BitBoard empty);
	static BitBoard DoublePushTargetsBlack(BitBoard bPawns, BitBoard empty); 


};