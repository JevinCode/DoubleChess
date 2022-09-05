#include "BBTwiddler.h"
#include "ChessBoard.h"

/**
 * bitScanForward
 * @author Kim Walisch (2012)
 * @param bb bitboard to scan
 * @precondition bb != 0
 * @return index (0..63) of least significant one bit
 */
int BBTwiddler::bitScanForward(BitBoard bb) {
	const BitBoard debruijn64 = BitBoard(0x03f79d71b4cb0a89);
	assert(bb != 0);
	return index64[((bb ^ (bb - 1)) * debruijn64) >> 58];
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
bool BBTwiddler::SingleElement(BitBoard bb)
{
	return bb != 0 && (bb & (bb - 1)) == 0;
}

int BBTwiddler::PopCount(BitBoard bb)
{
	return _mm_popcnt_u64(bb);
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