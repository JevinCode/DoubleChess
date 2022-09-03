#include "PseudoLegalMoveGenerator.h"

//TODO - Refactor and clean up this code! 
//1. Make enum index class so we dont need to cast to int all the time
//2. encapsulate chessboard better so we don't need to make this a friend class

std::vector<_Move> PseudoLegalMoveGenerator::GenerateKnightMoves(Team t, const ChessBoard& brd)
{
	Team other = (Team)(1 - (int)t);
	auto pieceBBs = brd.GetPieceBBs();
	auto pinCorridors = brd.GetCorridors();
	auto pins = brd.GetPins();
	std::vector<_Move> knightMoves;

	//handle pins first
	const auto pinnedKnights = pieceBBs[ChessBoard::BBIndex::Knights] & pieceBBs[t] & pins;
	const auto pinnedSquares = ChessBoard::BitBoardToSquares(pinnedKnights);
	for (const auto& square : pinnedSquares)
	{
		auto attacks = brd.KnightAttacks[square] & pinCorridors[square];
		auto quiets = attacks & brd.GetEmptyBB();
		auto captures = attacks & pieceBBs[other];

		auto quietSquares = ChessBoard::BitBoardToSquares(quiets);
		auto captureSquares = ChessBoard::BitBoardToSquares(captures);
		for (const auto& sq : quietSquares)
			knightMoves.push_back({ (ushort)_Move::Flag::None, (ushort)square, (ushort)sq, PieceType::Knight });
		for(const auto& sq : captureSquares)
		{
			auto capturePT = brd.ParseCapture(sq);
			knightMoves.push_back({ (ushort)_Move::Flag::Capture, (ushort)square, (ushort)sq, PieceType::Knight, capturePT });
		}
	}

	//handle non-pinned knights
	const auto freeKnights = pieceBBs[ChessBoard::BBIndex::Knights] & pieceBBs[t] & (ChessBoard::Universe ^ pins);
	const auto freeSquares = ChessBoard::BitBoardToSquares(freeKnights);
	for (const auto& square : freeSquares)
	{
		auto attacks = brd.KnightAttacks[square];
		auto quiets = attacks & brd.GetEmptyBB();
		auto captures = attacks & pieceBBs[other];

		auto quietSquares = ChessBoard::BitBoardToSquares(quiets);
		auto captureSquares = ChessBoard::BitBoardToSquares(captures);
		for (const auto& sq : quietSquares)
			knightMoves.push_back({ (ushort)_Move::Flag::None, (ushort)square, (ushort)sq, PieceType::Knight });
		for (const auto& sq : captureSquares)
		{
			auto capturePT = brd.ParseCapture(sq);
			knightMoves.push_back({ (ushort)_Move::Flag::Capture, (ushort)square, (ushort)sq, PieceType::Knight, capturePT });
		}
	}
	return knightMoves;
}
//std::vector<_Move> GenerateRookMoves(Team t, const ChessBoard& brd)
//{
//	std::vector<_Move> rookMoves;
//	Team other = (Team)(1 - (int)t);
//	auto pieceBBs = brd.GetPieceBBs();
//	auto RayAttacks = brd.GetRayAttacks();
//	auto rooks = pieceBBs[t] & pieceBBs[ChessBoard::BBIndex::Rooks];
//	auto rookSquares = ChessBoard::BitBoardToSquares(rooks);
//	for (const auto square : rookSquares)
//	{
//		BitBoard attacks =
//			BBTwiddler::GetPositiveRayAttack(pieceBBs[ChessBoard::BBIndex::Occupied], Direction::North, (int)square, RayAttacks) |
//			BBTwiddler::GetPositiveRayAttack(pieceBBs[ChessBoard::BBIndex::Occupied], Direction::East, (int)square, RayAttacks) |
//			BBTwiddler::GetNegativeRayAttack(pieceBBs[ChessBoard::BBIndex::Occupied], Direction::South, (int)square, RayAttacks) |
//			BBTwiddler::GetNegativeRayAttack(pieceBBs[ChessBoard::BBIndex::Occupied], Direction::West, (int)square, RayAttacks);
//		BitBoard quiets = attacks & pieceBBs[ChessBoard::BBIndex::Empty];
//		BitBoard captures = attacks & pieceBBs[other];
//
//		auto quietSquares = ChessBoard::BitBoardToSquares(quiets);
//		auto captureSquares = ChessBoard::BitBoardToSquares(captures);
//
//		for (const auto target : quietSquares)
//			rookMoves.push_back({ (ushort)_Move::Flag::None, (ushort)square, (ushort)target, PieceType::Rook });
//		for (const auto target : captureSquares)
//		{
//			auto capturePT = brd.ParseCapture(target);
//			rookMoves.push_back({ (ushort)_Move::Flag::Capture, (ushort)square, (ushort)target, PieceType::Rook, capturePT });
//		}
//	}
//	return rookMoves;
//}
//std::vector<_Move> GenerateBishopMoves(Team t, const ChessBoard& brd)
//{
//	std::vector<_Move> bishopMoves;
//	Team other = (Team)(1 - (int)t);
//	auto pieceBBs = brd.GetPieceBBs();
//	auto RayAttacks = brd.GetRayAttacks();
//	auto bishops = pieceBBs[t] & pieceBBs[ChessBoard::BBIndex::Bishops];
//	auto bishopSquares = ChessBoard::BitBoardToSquares(bishops);
//	for (const auto square : bishopSquares)
//	{
//		BitBoard attacks =
//			BBTwiddler::GetPositiveRayAttack(pieceBBs[ChessBoard::BBIndex::Occupied], Direction::Northeast, (int)square, RayAttacks) |
//			BBTwiddler::GetNegativeRayAttack(pieceBBs[ChessBoard::BBIndex::Occupied], Direction::Southeast, (int)square, RayAttacks) |
//			BBTwiddler::GetNegativeRayAttack(pieceBBs[ChessBoard::BBIndex::Occupied], Direction::Southwest, (int)square, RayAttacks) |
//			BBTwiddler::GetPositiveRayAttack(pieceBBs[ChessBoard::BBIndex::Occupied], Direction::Northwest, (int)square, RayAttacks);
//		BitBoard quiets = attacks & pieceBBs[ChessBoard::BBIndex::Empty];
//		BitBoard captures = attacks & pieceBBs[other];
//
//		auto quietSquares = ChessBoard::BitBoardToSquares(quiets);
//		auto captureSquares = ChessBoard::BitBoardToSquares(captures);
//
//		for (const auto target : quietSquares)
//			bishopMoves.push_back({ (ushort)_Move::Flag::None, (ushort)square, (ushort)target, PieceType::Bishop });
//		for (const auto target : captureSquares)
//		{
//			auto capturePT = brd.ParseCapture(target);
//			bishopMoves.push_back({ (ushort)_Move::Flag::Capture, (ushort)square, (ushort)target, PieceType::Bishop, capturePT });
//		}
//	}
//	return bishopMoves;
//}
//std::vector<_Move> GenerateQueenMoves(Team t, const ChessBoard& brd)
//{
//	std::vector<_Move> queenMoves;
//	Team other = (Team)(1 - (int)t);
//	auto pieceBBs = brd.GetPieceBBs();
//	auto RayAttacks = brd.GetRayAttacks();
//	auto queens = pieceBBs[(int)t] & pieceBBs[(int)ChessBoard::BBIndex::Queens];
//	auto queenSquares = ChessBoard::BitBoardToSquares(queens);
//	for (const auto square : queenSquares)
//	{
//		BitBoard attacks =
//			BBTwiddler::GetPositiveRayAttack(pieceBBs[ChessBoard::BBIndex::Occupied], Direction::North, (int)square, RayAttacks) |
//			BBTwiddler::GetPositiveRayAttack(pieceBBs[ChessBoard::BBIndex::Occupied], Direction::East, (int)square, RayAttacks) |
//			BBTwiddler::GetNegativeRayAttack(pieceBBs[ChessBoard::BBIndex::Occupied], Direction::South, (int)square, RayAttacks) |
//			BBTwiddler::GetNegativeRayAttack(pieceBBs[ChessBoard::BBIndex::Occupied], Direction::West, (int)square, RayAttacks) |
//			BBTwiddler::GetPositiveRayAttack(pieceBBs[ChessBoard::BBIndex::Occupied], Direction::Northeast, (int)square, RayAttacks) |
//			BBTwiddler::GetNegativeRayAttack(pieceBBs[ChessBoard::BBIndex::Occupied], Direction::Southeast, (int)square, RayAttacks) |
//			BBTwiddler::GetNegativeRayAttack(pieceBBs[ChessBoard::BBIndex::Occupied], Direction::Southwest, (int)square, RayAttacks) |
//			BBTwiddler::GetPositiveRayAttack(pieceBBs[ChessBoard::BBIndex::Occupied], Direction::Northwest, (int)square, RayAttacks);
//		BitBoard quiets = attacks & pieceBBs[ChessBoard::BBIndex::Empty];
//		BitBoard captures = attacks & pieceBBs[other];
//
//		auto quietSquares = ChessBoard::BitBoardToSquares(quiets);
//		auto captureSquares = ChessBoard::BitBoardToSquares(captures);
//
//		for (const auto target : quietSquares)
//			queenMoves.push_back({ (ushort)_Move::Flag::None, (ushort)square, (ushort)target, PieceType::Queen });
//		for (const auto target : captureSquares)
//		{
//			auto capturePT = brd.ParseCapture(target);
//			queenMoves.push_back({ (ushort)_Move::Flag::Capture, (ushort)square, (ushort)target, PieceType::Queen, capturePT });
//		}
//	}
//	return queenMoves;
//}
//std::vector<_Move> PseudoLegalMoveGenerator::GenerateSlidingMoves(Team t, const ChessBoard& brd)
//{
//	std::vector<_Move> slidingMoves = GenerateRookMoves(t, brd);
//	std::vector<_Move> moreMoves = GenerateBishopMoves(t, brd);
//	slidingMoves.insert(slidingMoves.end(), moreMoves.begin(), moreMoves.end());
//	moreMoves = GenerateQueenMoves(t, brd);
//	slidingMoves.insert(slidingMoves.end(), moreMoves.begin(), moreMoves.end());
//
//	return slidingMoves;
//}
//std::vector<_Move> PseudoLegalMoveGenerator::GenerateMoves(Team t, const ChessBoard& brd)
//{
//	auto moves = GenerateKnightMoves(t, brd);
//	auto moreMoves = GenerateSlidingMoves(t, brd);
//	moves.insert(moves.end(), moreMoves.begin(), moreMoves.end());
//	moreMoves = GeneratePawnMoves(t, brd);
//	moves.insert(moves.end(), moreMoves.begin(), moreMoves.end());
//	//moreMoves = GenerateKingMoves(t, brd);
//	//moves.insert(moves.end(), moreMoves.begin(), moreMoves.end());
//	return moves;
//}
BitBoard PseudoLegalMoveGenerator::SinglePushTargetsWhite(BitBoard wPawns, BitBoard empty)
{
	return BBTwiddler::NorthOne(wPawns) & empty;
}

BitBoard PseudoLegalMoveGenerator::DoublePushTargetsWhite(BitBoard wPawns, BitBoard empty)
{
	auto singlePushTargets = SinglePushTargetsWhite(wPawns, empty);
	return BBTwiddler::NorthOne(singlePushTargets) & empty & ChessBoard::Rank4;
}

BitBoard PseudoLegalMoveGenerator::SinglePushTargetsBlack(BitBoard bPawns, BitBoard empty)
{
	return BBTwiddler::SouthOne(bPawns) & empty;
}

BitBoard PseudoLegalMoveGenerator::DoublePushTargetsBlack(BitBoard bPawns, BitBoard empty)
{
	auto singlePushTargets = SinglePushTargetsBlack(bPawns, empty);
	return BBTwiddler::SouthOne(singlePushTargets) & empty & ChessBoard::Rank5;
}


//std::vector<_Move> PseudoLegalMoveGenerator::GenerateKingMoves(Team t, const ChessBoard& brd)
//{
//	std::vector<_Move> moves;
//	auto kingPos = brd.pieceBBs[(int)ChessBoard::BBIndex::Kings] & brd.pieceBBs[(int)t];
//	auto kingSquare = ChessBoard::BitBoardToSquares(kingPos);
//	auto attackBB = brd.KingAttacks[(int)kingSquare[0]];
//	auto attackSquares = ChessBoard::BitBoardToSquares(attackBB & brd.pieceBBs[1 - (int)t]);
//	auto quietSquares = ChessBoard::BitBoardToSquares(attackBB & brd.pieceBBs[(int)ChessBoard::BBIndex::Empty]);
//	for (const auto square : attackSquares)
//	{
//		auto capturePT = brd.ParseCapture(square);
//		moves.push_back({ (uint)_Move::Flag::Capture, (uint)kingSquare[0], (uint)square, PieceType::King, capturePT });
//	}
//	for (const auto square : quietSquares)
//	{
//		moves.push_back({ (uint)_Move::Flag::None, (uint)kingSquare[0], (uint)square, PieceType::King });
//	}
//	return moves;
//}