#include "BBTwiddler.h"
#include "ChessBoard.h"

/**
 * bitScanForward
 * @author Martin Läuter (1997)
 *         Charles E. Leiserson
 *         Harald Prokop
 *         Keith H. Randall
 * "Using de Bruijn Sequences to Index a 1 in a Computer Word"
 * @param bb bitboard to scan
 * @precondition bb != 0
 * @return index (0..63) of least significant one bit
 */

int BBTwiddler::bitScanForward(BitBoard bb)
{
	const BitBoard debruijn64 = C64(0x03f79d71b4cb0a89);
	assert(bb != 0);
	return index64[((bb & -bb) * debruijn64) >> 58];
}

/**
 * bitScanReverse
 * @authors Kim Walisch, Mark Dickinson
 * @param bb bitboard to scan
 * @precondition bb != 0
 * @return index (0..63) of most significant one bit
 */
int BBTwiddler::bitScanReverse(BitBoard bb)
{
	const BitBoard debruijn64 = C64(0x03f79d71b4cb0a89);
	assert(bb != 0);
	bb |= bb >> 1;
	bb |= bb >> 2;
	bb |= bb >> 4;
	bb |= bb >> 8;
	bb |= bb >> 16;
	bb |= bb >> 32;
	return rIndex64[(bb * debruijn64) >> 58];
}
BitBoard BBTwiddler::NorthOne(BitBoard bb)
{
	return bb << 8;
}

BitBoard BBTwiddler::SouthOne(BitBoard bb)
{
	return bb >> 8;
}
BitBoard BBTwiddler::EastOne(BitBoard bb)
{
	return (bb & ChessBoard::NotHFile) << 1;
}
BitBoard BBTwiddler::WestOne(BitBoard bb)
{
	return (bb & ChessBoard::NotAFile) >> 1;
}
BitBoard BBTwiddler::NortheastOne(BitBoard bb)
{
	return (bb & ChessBoard::NotHFile) << 9;
}
BitBoard BBTwiddler::NorthwestOne(BitBoard bb)
{
	return (bb & ChessBoard::NotAFile) << 7;
}
BitBoard BBTwiddler::SoutheastOne(BitBoard bb)
{
	return (bb & ChessBoard::NotHFile) >> 7;
}
BitBoard BBTwiddler::SouthwestOne(BitBoard bb)
{
	return (bb & ChessBoard::NotAFile) >> 9;
}

BitBoard BBTwiddler::GetPositiveRayAttack(const BitBoard occupied, Direction dir, int square, const std::vector<std::vector<BitBoard>>& RayAttacks)
{
	BitBoard attacks = RayAttacks[square][(int)dir];
	BitBoard blocker = attacks & occupied;
	if (blocker)
	{
		square = BBTwiddler::bitScanForward(blocker);
		attacks ^= RayAttacks[(int)square][(int)dir];
	}
	return attacks;
}
BitBoard BBTwiddler::GetNegativeRayAttack(const BitBoard occupied, Direction dir, int square, const std::vector<std::vector<BitBoard>>& RayAttacks)
{
	BitBoard attacks = RayAttacks[(int)square][(int)dir];
	BitBoard blocker = attacks & occupied;
	if (blocker)
	{
		square = BBTwiddler::bitScanReverse(blocker);
		attacks ^= RayAttacks[(int)square][(int)dir];
	}
	return attacks;
}

BitBoard BBTwiddler::GetRookAttackBBSingle(const BitBoard occupied, int square, const std::vector<std::vector<BitBoard>>& RayAttacks)
{
		return 
		GetPositiveRayAttack(occupied, Direction::North, square, RayAttacks) |
		GetPositiveRayAttack(occupied, Direction::East, square, RayAttacks) |
		GetNegativeRayAttack(occupied, Direction::South, square, RayAttacks) |
		GetNegativeRayAttack(occupied, Direction::West, square, RayAttacks);
}

BitBoard BBTwiddler::GetRookAttackBB(const BitBoard occupied, BitBoard rooks, const std::vector<std::vector<BitBoard>>& RayAttacks)
{
	if (!rooks)
		return 0;
	//we assume there are at most two rooks
	int squareIdx1 = bitScanForward(rooks);
	int squareIdx2 = bitScanReverse(rooks);

	BitBoard attacks1 = GetRookAttackBBSingle(occupied, squareIdx1, RayAttacks);

	if (squareIdx1 == squareIdx2)
		return attacks1;

	BitBoard attacks2 = GetRookAttackBBSingle(occupied, squareIdx2, RayAttacks);

	return attacks1 | attacks2;
}

BitBoard BBTwiddler::GetBishopAttackBBSingle(const BitBoard occupied, int square, const std::vector<std::vector<BitBoard>>& RayAttacks)
{
	return
		GetPositiveRayAttack(occupied, Direction::Northeast, square, RayAttacks) |
		GetPositiveRayAttack(occupied, Direction::Northwest, square, RayAttacks) |
		GetNegativeRayAttack(occupied, Direction::Southeast, square, RayAttacks) |
		GetNegativeRayAttack(occupied, Direction::Southwest, square, RayAttacks);
}

BitBoard BBTwiddler::GetBishopAttackBB(const BitBoard occupied, const BitBoard bishops, const std::vector<std::vector<BitBoard>>& RayAttacks)
{
	if (!bishops)
		return 0;
	//we assume there are at most two rooks
	int squareIdx1 = bitScanForward(bishops);
	int squareIdx2 = bitScanReverse(bishops);

	BitBoard attacks1 = GetBishopAttackBBSingle(occupied, squareIdx1, RayAttacks);

	if (squareIdx1 == squareIdx2)
		return attacks1;

	BitBoard attacks2 = GetBishopAttackBBSingle(occupied, squareIdx2, RayAttacks);

	return attacks1 | attacks2;
}

BitBoard BBTwiddler::GetQueenAttackBBSingle(const BitBoard occupied, int square, const std::vector<std::vector<BitBoard>>& RayAttacks)
{
	return GetRookAttackBBSingle(occupied, square, RayAttacks) | GetBishopAttackBBSingle(occupied, square, RayAttacks);
}

BitBoard BBTwiddler::GetQueenAttackBB(const BitBoard occupied, const BitBoard queens, const std::vector<std::vector<BitBoard>>& RayAttacks)
{
	auto squares = BitBoardToSquares(queens);
	BitBoard result = 0;
	for (int square : squares)
	{
		result |= GetQueenAttackBBSingle(occupied, square, RayAttacks);
	}
	return result;
}

BitBoard BBTwiddler::GetKnightAttackBB(BitBoard knights, std::vector<BitBoard>& knightAttacks)
{
	auto knightSquares = BitBoardToSquares(knights);
	BitBoard result = 0;
	for (int square : knightSquares)
	{
		result |= knightAttacks[square];
	}
	return result;
}
BitBoard BBTwiddler::WhitePawnEastAttacks(BitBoard wPawns)
{
	return BBTwiddler::NortheastOne(wPawns);
}
BitBoard BBTwiddler::WhitePawnWestAttacks(BitBoard wPawns)
{
	return BBTwiddler::NorthwestOne(wPawns);
}
BitBoard BBTwiddler::BlackPawnEastAttacks(BitBoard bPawns)
{
	return BBTwiddler::SoutheastOne(bPawns);
}
BitBoard BBTwiddler::BlackPawnWestAttacks(BitBoard bPawns)
{
	return BBTwiddler::SouthwestOne(bPawns);
}

BitBoard BBTwiddler::WhitePawnAttacks(BitBoard wPawns)
{
	return WhitePawnEastAttacks(wPawns) | WhitePawnWestAttacks(wPawns);
}

BitBoard BBTwiddler::BlackPawnAttacks(BitBoard bPawns)
{
	return BlackPawnEastAttacks(bPawns) | BlackPawnWestAttacks(bPawns);
}

BitBoard BBTwiddler::KingAttacks(BitBoard kingSet)
{
	auto originalSet = kingSet;
	BitBoard attacks = BBTwiddler::EastOne(kingSet) | BBTwiddler::WestOne(kingSet);
	kingSet |= attacks;
	attacks |= BBTwiddler::NorthOne(kingSet) | BBTwiddler::SouthOne(kingSet);
	attacks ^= originalSet;
	return attacks;
}