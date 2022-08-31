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

BitBoard BBTwiddler::GetPositiveRayAttack(const BitBoard occupied, Direction dir, int square, const std::vector<std::vector<BitBoard>> RayAttacks)
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
BitBoard BBTwiddler::GetNegativeRayAttack(const BitBoard occupied, Direction dir, int square, const std::vector<std::vector<BitBoard>> RayAttacks)
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