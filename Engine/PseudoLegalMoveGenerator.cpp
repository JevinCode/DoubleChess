#include "PseudoLegalMoveGenerator.h"

//TODO - Refactor and clean up this code! 
//1. Make enum index class so we dont need to cast to int all the time
//2. encapsulate chessboard better so we don't need to make this a friend class

std::vector<_Move> PseudoLegalMoveGenerator::GenerateKnightMoves(Team t, const ChessBoard& brd)
{
	auto pieceBBs = brd.GetPieceBBs();
	std::vector<_Move> knightMoves;
	if (t == Team::BLACK)
	{
		const auto blackKnightsBB = pieceBBs[(int)ChessBoard::BBIndex::Black] & pieceBBs[(int)ChessBoard::BBIndex::Knights];
		const auto blackKnightSquares = brd.BitBoardToSquares(blackKnightsBB);
		for (const auto& square : blackKnightSquares)
		{
			//first we will compute quiet moves
			BitBoard attacks = brd.KnightAttacks[(int)square];
			BitBoard quiets = attacks & brd.pieceBBs[(int)ChessBoard::BBIndex::Empty];
			BitBoard captures = attacks & pieceBBs[(int)ChessBoard::BBIndex::White];

			std::vector<ChessBoard::Square> quietSquares = brd.BitBoardToSquares(quiets);
			std::vector<ChessBoard::Square> captureSquares = brd.BitBoardToSquares(captures);
			for (const auto& sq : quietSquares)
				knightMoves.push_back({ (ushort)_Move::Flag::None, (ushort)square, (ushort)sq, _Move::PieceType::Knight });

			for (const auto& sq : captureSquares)
			{
				auto capturePT = brd.ParseCapture(sq);
				knightMoves.push_back({ (ushort)_Move::Flag::Capture, (ushort)square, (ushort)sq, _Move::PieceType::Knight, capturePT });
			}
		}
		return knightMoves;
	}

	const auto whiteKnightsBB = pieceBBs[(int)ChessBoard::BBIndex::White] & pieceBBs[(int)ChessBoard::BBIndex::Knights];
	const auto whiteKnightSquares = brd.BitBoardToSquares(whiteKnightsBB);
	for (const auto& square : whiteKnightSquares)
	{
		//first we will compute quiet moves
		BitBoard attacks = brd.KnightAttacks[(int)square];
		BitBoard quiets = attacks & brd.pieceBBs[(int)ChessBoard::BBIndex::Empty];
		BitBoard captures = attacks & pieceBBs[(int)ChessBoard::BBIndex::Black];

		std::vector<ChessBoard::Square> quietSquares = brd.BitBoardToSquares(quiets);
		std::vector<ChessBoard::Square> captureSquares = brd.BitBoardToSquares(captures);
		for (const auto& sq : quietSquares)
			knightMoves.push_back({ (uint)_Move::Flag::None, (uint)square, (uint)sq, _Move::PieceType::Knight });

		for (const auto& sq : captureSquares)
		{
			auto capturePT = brd.ParseCapture(sq);
			knightMoves.push_back({ (uint)_Move::Flag::Capture, (uint)square, (uint)sq, _Move::PieceType::Knight, capturePT });
		}
	}
	return knightMoves;
}
std::vector<_Move> GenerateRookMoves(Team t, const ChessBoard& brd)
{
	std::vector<_Move> rookMoves;
	Team other = (Team)(1 - (int)t);
	auto pieceBBs = brd.GetPieceBBs();
	auto RayAttacks = brd.GetRayAttacks();
	auto rooks = pieceBBs[(int)t] & pieceBBs[(int)ChessBoard::BBIndex::Rooks];
	auto rookSquares = ChessBoard::BitBoardToSquares(rooks);
	for (const auto square : rookSquares)
	{
		BitBoard attacks =
			BBTwiddler::GetPositiveRayAttack(pieceBBs[(int)ChessBoard::BBIndex::Occupied], Direction::North, (int)square, RayAttacks) |
			BBTwiddler::GetPositiveRayAttack(pieceBBs[(int)ChessBoard::BBIndex::Occupied], Direction::East, (int)square, RayAttacks) |
			BBTwiddler::GetNegativeRayAttack(pieceBBs[(int)ChessBoard::BBIndex::Occupied], Direction::South, (int)square, RayAttacks) |
			BBTwiddler::GetNegativeRayAttack(pieceBBs[(int)ChessBoard::BBIndex::Occupied], Direction::West, (int)square, RayAttacks);
		BitBoard quiets = attacks & pieceBBs[(int)ChessBoard::BBIndex::Empty];
		BitBoard captures = attacks & pieceBBs[(int)other];

		auto quietSquares = ChessBoard::BitBoardToSquares(quiets);
		auto captureSquares = ChessBoard::BitBoardToSquares(captures);

		for (const auto target : quietSquares)
			rookMoves.push_back({ (ushort)_Move::Flag::None, (ushort)square, (ushort)target, _Move::PieceType::Rook });
		for (const auto target : captureSquares)
		{
			auto capturePT = brd.ParseCapture(target);
			rookMoves.push_back({ (ushort)_Move::Flag::Capture, (ushort)square, (ushort)target, _Move::PieceType::Rook, capturePT });
		}
	}
	return rookMoves;
}
std::vector<_Move> GenerateBishopMoves(Team t, const ChessBoard& brd)
{
	std::vector<_Move> bishopMoves;
	Team other = (Team)(1 - (int)t);
	auto pieceBBs = brd.GetPieceBBs();
	auto RayAttacks = brd.GetRayAttacks();
	auto bishops = pieceBBs[(int)t] & pieceBBs[(int)ChessBoard::BBIndex::Bishops];
	auto bishopSquares = ChessBoard::BitBoardToSquares(bishops);
	for (const auto square : bishopSquares)
	{
		BitBoard attacks =
			BBTwiddler::GetPositiveRayAttack(pieceBBs[(int)ChessBoard::BBIndex::Occupied], Direction::Northeast, (int)square, RayAttacks) |
			BBTwiddler::GetNegativeRayAttack(pieceBBs[(int)ChessBoard::BBIndex::Occupied], Direction::Southeast, (int)square, RayAttacks) |
			BBTwiddler::GetNegativeRayAttack(pieceBBs[(int)ChessBoard::BBIndex::Occupied], Direction::Southwest, (int)square, RayAttacks) |
			BBTwiddler::GetPositiveRayAttack(pieceBBs[(int)ChessBoard::BBIndex::Occupied], Direction::Northwest, (int)square, RayAttacks);
		BitBoard quiets = attacks & pieceBBs[(int)ChessBoard::BBIndex::Empty];
		BitBoard captures = attacks & pieceBBs[(int)other];

		auto quietSquares = ChessBoard::BitBoardToSquares(quiets);
		auto captureSquares = ChessBoard::BitBoardToSquares(captures);

		for (const auto target : quietSquares)
			bishopMoves.push_back({ (ushort)_Move::Flag::None, (ushort)square, (ushort)target, _Move::PieceType::Bishop });
		for (const auto target : captureSquares)
		{
			auto capturePT = brd.ParseCapture(target);
			bishopMoves.push_back({ (ushort)_Move::Flag::Capture, (ushort)square, (ushort)target, _Move::PieceType::Bishop, capturePT });
		}
	}
	return bishopMoves;
}
std::vector<_Move> GenerateQueenMoves(Team t, const ChessBoard& brd)
{
	std::vector<_Move> queenMoves;
	Team other = (Team)(1 - (int)t);
	auto pieceBBs = brd.GetPieceBBs();
	auto RayAttacks = brd.GetRayAttacks();
	auto queens = pieceBBs[(int)t] & pieceBBs[(int)ChessBoard::BBIndex::Queens];
	auto queenSquares = ChessBoard::BitBoardToSquares(queens);
	for (const auto square : queenSquares)
	{
		BitBoard attacks =
			BBTwiddler::GetPositiveRayAttack(pieceBBs[(int)ChessBoard::BBIndex::Occupied], Direction::North, (int)square, RayAttacks) |
			BBTwiddler::GetPositiveRayAttack(pieceBBs[(int)ChessBoard::BBIndex::Occupied], Direction::East, (int)square, RayAttacks) |
			BBTwiddler::GetNegativeRayAttack(pieceBBs[(int)ChessBoard::BBIndex::Occupied], Direction::South, (int)square, RayAttacks) |
			BBTwiddler::GetNegativeRayAttack(pieceBBs[(int)ChessBoard::BBIndex::Occupied], Direction::West, (int)square, RayAttacks) |
			BBTwiddler::GetPositiveRayAttack(pieceBBs[(int)ChessBoard::BBIndex::Occupied], Direction::Northeast, (int)square, RayAttacks) |
			BBTwiddler::GetNegativeRayAttack(pieceBBs[(int)ChessBoard::BBIndex::Occupied], Direction::Southeast, (int)square, RayAttacks) |
			BBTwiddler::GetNegativeRayAttack(pieceBBs[(int)ChessBoard::BBIndex::Occupied], Direction::Southwest, (int)square, RayAttacks) |
			BBTwiddler::GetPositiveRayAttack(pieceBBs[(int)ChessBoard::BBIndex::Occupied], Direction::Northwest, (int)square, RayAttacks);
		BitBoard quiets = attacks & pieceBBs[(int)ChessBoard::BBIndex::Empty];
		BitBoard captures = attacks & pieceBBs[(int)other];

		auto quietSquares = ChessBoard::BitBoardToSquares(quiets);
		auto captureSquares = ChessBoard::BitBoardToSquares(captures);

		for (const auto target : quietSquares)
			queenMoves.push_back({ (ushort)_Move::Flag::None, (ushort)square, (ushort)target, _Move::PieceType::Queen });
		for (const auto target : captureSquares)
		{
			auto capturePT = brd.ParseCapture(target);
			queenMoves.push_back({ (ushort)_Move::Flag::Capture, (ushort)square, (ushort)target, _Move::PieceType::Queen, capturePT });
		}
	}
	return queenMoves;
}
std::vector<_Move> PseudoLegalMoveGenerator::GenerateSlidingMoves(Team t, const ChessBoard& brd)
{
	std::vector<_Move> slidingMoves = GenerateRookMoves(t, brd);
	std::vector<_Move> moreMoves = GenerateBishopMoves(t, brd);
	slidingMoves.insert(slidingMoves.end(), moreMoves.begin(), moreMoves.end());
	moreMoves = GenerateQueenMoves(t, brd);
	slidingMoves.insert(slidingMoves.end(), moreMoves.begin(), moreMoves.end());

	return slidingMoves;
}
std::vector<_Move> PseudoLegalMoveGenerator::GenerateMoves(Team t, const ChessBoard& brd)
{
	auto moves = GenerateKnightMoves(t, brd);
	auto moreMoves = GenerateSlidingMoves(t, brd);
	moves.insert(moves.end(), moreMoves.begin(), moreMoves.end());
	moreMoves = GeneratePawnMoves(t, brd);
	moves.insert(moves.end(), moreMoves.begin(), moreMoves.end());
	//moreMoves = GenerateKingMoves(t, brd);
	//moves.insert(moves.end(), moreMoves.begin(), moreMoves.end());
	return moves;
}
BitBoard PseudoLegalMoveGenerator::SinglePushTargetsWhite(const ChessBoard& brd)
{
	auto whitePawns = brd.pieceBBs[(int)ChessBoard::BBIndex::White] & brd.pieceBBs[(int)ChessBoard::BBIndex::Pawns];
	return BBTwiddler::NorthOne(whitePawns) & brd.pieceBBs[(int)ChessBoard::BBIndex::Empty];
}

BitBoard PseudoLegalMoveGenerator::DoublePushTargetsWhite(const ChessBoard& brd)
{
	auto singlePushTargets = SinglePushTargetsWhite(brd);
	return BBTwiddler::NorthOne(singlePushTargets) & brd.pieceBBs[(int)ChessBoard::BBIndex::Empty] & ChessBoard::Rank4;
}

BitBoard PseudoLegalMoveGenerator::SinglePushTargetsBlack(const ChessBoard& brd)
{
	auto blackPawns = brd.pieceBBs[(int)ChessBoard::BBIndex::Black] & brd.pieceBBs[(int)ChessBoard::BBIndex::Pawns];
	return BBTwiddler::SouthOne(blackPawns) & brd.pieceBBs[(int)ChessBoard::BBIndex::Empty];
}

BitBoard PseudoLegalMoveGenerator::DoublePushTargetsBlack(const ChessBoard& brd)
{
	auto singlePushTargets = SinglePushTargetsBlack(brd);
	return BBTwiddler::SouthOne(singlePushTargets) & brd.pieceBBs[(int)ChessBoard::BBIndex::Empty] & ChessBoard::Rank5;
}

std::vector<_Move> PseudoLegalMoveGenerator::GeneratePawnMoves(Team t, const ChessBoard& brd)
{
	std::vector<_Move> moves;
	if (t == Team::WHITE)
	{
		BitBoard singlePushTargets = SinglePushTargetsWhite(brd);
		BitBoard singlePushPromotions = singlePushTargets & ChessBoard::Rank8;
		BitBoard singlePushQuiets = singlePushTargets ^ singlePushPromotions;
		BitBoard doublePushTargets = DoublePushTargetsWhite(brd); 
		BitBoard wPawns = brd.pieceBBs[(int)ChessBoard::BBIndex::Pawns] & brd.pieceBBs[(int)ChessBoard::BBIndex::White];
		BitBoard wPawnEastAttacks = BBTwiddler::WhitePawnEastAttacks(wPawns) & brd.pieceBBs[(int)ChessBoard::BBIndex::Black];
		BitBoard wPawnEastPromotions = wPawnEastAttacks & ChessBoard::Rank8;
		wPawnEastAttacks ^= wPawnEastPromotions;
		BitBoard wPawnWestAttacks = BBTwiddler::WhitePawnWestAttacks(wPawns) & brd.pieceBBs[(int)ChessBoard::BBIndex::Black];
		BitBoard wPawnWestPromotions = wPawnWestAttacks & ChessBoard::Rank8;
		wPawnWestAttacks ^= wPawnWestPromotions;

		auto singlePushPromotionSquares = ChessBoard::BitBoardToSquares(singlePushPromotions);
		auto singlePushQuietSquares = ChessBoard::BitBoardToSquares(singlePushQuiets);
		auto doublePushSquares = ChessBoard::BitBoardToSquares(doublePushTargets);
		auto attackSquaresEast = ChessBoard::BitBoardToSquares(wPawnEastAttacks);
		auto promotionSquaresEast = ChessBoard::BitBoardToSquares(wPawnEastPromotions);
		auto attackSquaresWest = ChessBoard::BitBoardToSquares(wPawnWestAttacks);
		auto promotionSquaresWest = ChessBoard::BitBoardToSquares(wPawnWestPromotions);

		for (const auto square : singlePushPromotionSquares)
		{
			moves.push_back({ (uint)_Move::Flag::KnightPromotion, (uint)square - 8, (uint)square, _Move::PieceType::Pawn });
			moves.push_back({ (uint)_Move::Flag::QueenPromotion, (uint)square - 8, (uint)square, _Move::PieceType::Pawn });
		}		
		for (const auto square : singlePushQuietSquares)
		{
			moves.push_back({ (uint)_Move::Flag::None, (uint)square - 8, (uint)square, _Move::PieceType::Pawn });
		}
		for(const auto square : doublePushSquares)
			moves.push_back({ (uint)_Move::Flag::PawnDoublePush, (uint)square - 16, (uint)square, _Move::PieceType::Pawn });
		for (const auto square : attackSquaresEast)
		{
			auto capturePT = brd.ParseCapture(square);
			moves.push_back({ (uint)_Move::Flag::Capture, (uint)square - 9, (uint)square, _Move::PieceType::Pawn, capturePT });
		}
		for (const auto square : promotionSquaresEast)
		{
			auto capturePT = brd.ParseCapture(square);
			moves.push_back({ (uint)_Move::Flag::KnightPromotionCapture, (uint)square - 9, (uint)square, _Move::PieceType::Pawn, capturePT });
			moves.push_back({ (uint)_Move::Flag::QueenPromotionCapture, (uint)square - 9, (uint)square, _Move::PieceType::Pawn, capturePT });
		}
		for (const auto square : attackSquaresWest)
		{
			auto capturePT = brd.ParseCapture(square);
			moves.push_back({ (uint)_Move::Flag::Capture, (uint)square - 7, (uint)square, _Move::PieceType::Pawn, capturePT });
		}
		for (const auto square : promotionSquaresWest)
		{
			auto capturePT = brd.ParseCapture(square);
			moves.push_back({ (uint)_Move::Flag::KnightPromotionCapture, (uint)square - 7, (uint)square, _Move::PieceType::Pawn, capturePT });
			moves.push_back({ (uint)_Move::Flag::QueenPromotionCapture, (uint)square - 7, (uint)square, _Move::PieceType::Pawn, capturePT });
		}
		return moves;
	}

	BitBoard singlePushTargets = SinglePushTargetsBlack(brd);
	BitBoard singlePushPromotions = singlePushTargets & ChessBoard::Rank1;
	BitBoard singlePushQuiets = singlePushTargets ^ singlePushPromotions;
	BitBoard doublePushTargets = DoublePushTargetsBlack(brd);
	BitBoard bPawns = brd.pieceBBs[(int)ChessBoard::BBIndex::Pawns] & brd.pieceBBs[(int)ChessBoard::BBIndex::Black];
	BitBoard bPawnEastAttacks = BBTwiddler::BlackPawnEastAttacks(bPawns) & brd.pieceBBs[(int)ChessBoard::BBIndex::White];
	BitBoard bPawnEastPromotions = bPawnEastAttacks & ChessBoard::Rank1;
	bPawnEastAttacks ^= bPawnEastPromotions;
	BitBoard bPawnWestAttacks = BBTwiddler::BlackPawnWestAttacks(bPawns) & brd.pieceBBs[(int)ChessBoard::BBIndex::White];
	BitBoard bPawnWestPromotions = bPawnWestAttacks & ChessBoard::Rank1;
	bPawnWestAttacks ^= bPawnWestPromotions;

	auto singlePushPromotionSquares = ChessBoard::BitBoardToSquares(singlePushPromotions);
	auto singlePushQuietSquares = ChessBoard::BitBoardToSquares(singlePushQuiets);
	auto doublePushSquares = ChessBoard::BitBoardToSquares(doublePushTargets);
	auto attackSquaresEast = ChessBoard::BitBoardToSquares(bPawnEastAttacks);
	auto promotionSquaresEast = ChessBoard::BitBoardToSquares(bPawnEastPromotions);
	auto attackSquaresWest = ChessBoard::BitBoardToSquares(bPawnWestAttacks);
	auto promotionSquaresWest = ChessBoard::BitBoardToSquares(bPawnWestPromotions);

	for (const auto square : singlePushPromotionSquares)
	{
		moves.push_back({ (uint)_Move::Flag::KnightPromotion, (uint)square + 8, (uint)square, _Move::PieceType::Pawn });
		moves.push_back({ (uint)_Move::Flag::QueenPromotion, (uint)square + 8, (uint)square, _Move::PieceType::Pawn });
	}
	for (const auto square : singlePushQuietSquares)
		moves.push_back({ (uint)_Move::Flag::None, (uint)square + 8, (uint)square, _Move::PieceType::Pawn });
	for (const auto square : doublePushSquares)
		moves.push_back({ (uint)_Move::Flag::PawnDoublePush, (uint)square + 16, (uint)square, _Move::PieceType::Pawn });
	for (const auto square : attackSquaresEast)
	{
		auto capturePT = brd.ParseCapture(square);
		moves.push_back({ (uint)_Move::Flag::Capture, (uint)square + 7, (uint)square, _Move::PieceType::Pawn, capturePT });
	}		
	for (const auto square : promotionSquaresEast)
	{
		auto capturePT = brd.ParseCapture(square);
		moves.push_back({ (uint)_Move::Flag::KnightPromotionCapture, (uint)square + 7, (uint)square, _Move::PieceType::Pawn, capturePT });
		moves.push_back({ (uint)_Move::Flag::QueenPromotionCapture, (uint)square + 7, (uint)square, _Move::PieceType::Pawn, capturePT });
	}
	for (const auto square : attackSquaresWest)
	{
		auto capturePT = brd.ParseCapture(square);
		moves.push_back({ (uint)_Move::Flag::Capture, (uint)square + 9, (uint)square, _Move::PieceType::Pawn, capturePT });
	}
	for (const auto square : promotionSquaresWest)
	{
		auto capturePT = brd.ParseCapture(square);
		moves.push_back({ (uint)_Move::Flag::KnightPromotionCapture, (uint)square + 9, (uint)square, _Move::PieceType::Pawn, capturePT });
		moves.push_back({ (uint)_Move::Flag::QueenPromotionCapture, (uint)square + 9, (uint)square, _Move::PieceType::Pawn, capturePT });
	}
	return moves;
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
//		moves.push_back({ (uint)_Move::Flag::Capture, (uint)kingSquare[0], (uint)square, _Move::PieceType::King, capturePT });
//	}
//	for (const auto square : quietSquares)
//	{
//		moves.push_back({ (uint)_Move::Flag::None, (uint)kingSquare[0], (uint)square, _Move::PieceType::King });
//	}
//	return moves;
//}