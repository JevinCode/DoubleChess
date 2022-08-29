#include "BBTwiddler.h"


/**
 * bitScanForward
 * @author Martin L�uter (1997)
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