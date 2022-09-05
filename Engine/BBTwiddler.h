#pragma once
#include "BitBoard.h"
#include "UniversalTypes.h"
#include <vector>
#include <cassert>
#include <intrin.h>

class BBTwiddler
{
public:
	static int bitScanForward(BitBoard bb);
	static int bitScanReverse(BitBoard bb);
	static bool SingleElement(BitBoard bb);
	static int PopCount(BitBoard bb);
	static BitBoard NorthOne(BitBoard bb);
	static BitBoard EastOne(BitBoard bb);
	static BitBoard SouthOne(BitBoard bb);
	static BitBoard WestOne(BitBoard bb);
	static BitBoard NortheastOne(BitBoard bb);
	static BitBoard NorthwestOne(BitBoard bb);
	static BitBoard SoutheastOne(BitBoard bb);
	static BitBoard SouthwestOne(BitBoard bb);

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