#pragma once
#include "BitBoard.h"
#include "UniversalTypes.h"
#include <vector>
#include <cassert>

class BBTwiddler
{
public:
	static int bitScanForward(BitBoard bb);
	static int bitScanReverse(BitBoard bb);
	static BitBoard NorthOne(BitBoard bb);
	static BitBoard EastOne(BitBoard bb);
	static BitBoard SouthOne(BitBoard bb);
	static BitBoard WestOne(BitBoard bb);
	static BitBoard NortheastOne(BitBoard bb);
	static BitBoard NorthwestOne(BitBoard bb);
	static BitBoard SoutheastOne(BitBoard bb);
	static BitBoard SouthwestOne(BitBoard bb);
	static BitBoard GetPositiveRayAttack(const BitBoard occupied, Direction dir, int square, const std::vector<std::vector<BitBoard>>& RayAttacks);
	static BitBoard GetNegativeRayAttack(const BitBoard occupied, Direction dir, int square, const std::vector<std::vector<BitBoard>>& RayAttacks);

	static BitBoard GetRookAttackBBSingle(const BitBoard occupied, int square, const std::vector<std::vector<BitBoard>>& RayAttacks);
	static BitBoard GetRookAttackBB(const BitBoard occupied, const BitBoard rooks, const std::vector<std::vector<BitBoard>>& RayAttacks);
	static BitBoard GetBishopAttackBBSingle(const BitBoard occupied, int square, const std::vector<std::vector<BitBoard>>& RayAttacks);
	static BitBoard GetBishopAttackBB(const BitBoard occupied, const BitBoard bishops, const std::vector<std::vector<BitBoard>>& RayAttacks);
	static std::vector<int> BitBoardToSquares(BitBoard bb)
	{
		std::vector<int> squares;
		for (int i = 0; i < 64; i++)
		{
			BitBoard mask = (BitBoard)0x1 << i;
			if (bb & mask)
			{
				squares.push_back(i);
			}
		}
		return squares;
	}
	static BitBoard GetQueenAttackBBSingle(const BitBoard occupied, int square, const std::vector<std::vector<BitBoard>>& RayAttacks);
	static BitBoard GetQueenAttackBB(const BitBoard occupied, const BitBoard queens, const std::vector<std::vector<BitBoard>>& RayAttacks);

	static BitBoard KingAttacks(BitBoard kingSet);
	static BitBoard WhitePawnEastAttacks(BitBoard wpawns);
	static BitBoard WhitePawnWestAttacks(BitBoard wpawns);
	static BitBoard BlackPawnEastAttacks(BitBoard bpawns);
	static BitBoard BlackPawnWestAttacks(BitBoard bpawns);

	static BitBoard WhitePawnAttacks(BitBoard wpawns);
	static BitBoard BlackPawnAttacks(BitBoard bPawns);

private:
	static constexpr int index64[64] = {
	0, 47,  1, 56, 48, 27,  2, 60,
   57, 49, 41, 37, 28, 16,  3, 61,
   54, 58, 35, 52, 50, 42, 21, 44,
   38, 32, 29, 23, 17, 11,  4, 62,
   46, 55, 26, 59, 40, 36, 15, 53,
   34, 51, 20, 43, 31, 22, 10, 45,
   25, 39, 14, 33, 19, 30,  9, 24,
   13, 18,  8, 12,  7,  6,  5, 63
};

	static constexpr int rIndex64[64] = {
	0, 47,  1, 56, 48, 27,  2, 60,
   57, 49, 41, 37, 28, 16,  3, 61,
   54, 58, 35, 52, 50, 42, 21, 44,
   38, 32, 29, 23, 17, 11,  4, 62,
   46, 55, 26, 59, 40, 36, 15, 53,
   34, 51, 20, 43, 31, 22, 10, 45,
   25, 39, 14, 33, 19, 30,  9, 24,
   13, 18,  8, 12,  7,  6,  5, 63
	};
};