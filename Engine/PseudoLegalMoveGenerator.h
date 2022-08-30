#pragma once

#include "ChessBoard.h"
#include "UniversalTypes.h"

class PseudoLegalMoveGenerator
{
public:
	static std::vector<_Move> GenerateMoves(Team t, const ChessBoard& brd);
	static std::vector<_Move> GenerateKnightMoves(Team t, const ChessBoard& brd);
	static std::vector<_Move> GenerateSlidingMoves(Team t, const ChessBoard& brd); 
	static std::vector<_Move> GeneratePawnMoves(Team t, const ChessBoard& brd);
	static std::vector<_Move> GenerateKingMoves(Team t, const ChessBoard& brd);
	static BitBoard KingAttacks(BitBoard kingSet);

private:
	static BitBoard GetPositiveRayAttack(BitBoard occupied, Direction dir, ChessBoard::Square square, const ChessBoard& brd);
	static BitBoard GetNegativeRayAttack(BitBoard occupied, Direction dir, ChessBoard::Square square, const ChessBoard& brd);
	static BitBoard SinglePushTargetsWhite(const ChessBoard& brd);
	static BitBoard DoublePushTargetsWhite(const ChessBoard& brd);
	static BitBoard SinglePushTargetsBlack(const ChessBoard& brd);
	static BitBoard DoublePushTargetsBlack(const ChessBoard& brd); 

	static BitBoard WhitePawnEastAttacks(BitBoard wpawns);
	static BitBoard WhitePawnWestAttacks(BitBoard wpawns);
	static BitBoard BlackPawnEastAttacks(BitBoard bpawns);
	static BitBoard BlackPawnWestAttacks(BitBoard bpawns);

	static BitBoard WhitePawnAttacks(BitBoard wpawns);
	static BitBoard BlackPawnAttacks(BitBoard bPawns);

};