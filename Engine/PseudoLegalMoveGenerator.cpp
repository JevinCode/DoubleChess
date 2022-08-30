#include "PseudoLegalMoveGenerator.h"
BitBoard PseudoLegalMoveGenerator::GetPositiveRayAttack(BitBoard occupied, Direction dir, ChessBoard::Square square, const ChessBoard& brd)
{
	BitBoard attacks = brd.RayAttacks[(int)square][(int)dir];
	BitBoard blocker = attacks & occupied;
	if (blocker)
	{
		square = (ChessBoard::Square)BBTwiddler::bitScanForward(blocker);
		attacks ^= brd.RayAttacks[(int)square][(int)dir];
	}
	return attacks;
}	
BitBoard PseudoLegalMoveGenerator::GetNegativeRayAttack(BitBoard occupied, Direction dir, ChessBoard::Square square, const ChessBoard& brd)
{
	BitBoard attacks = brd.RayAttacks[(int)square][(int)dir];
	BitBoard blocker = attacks & occupied;
	if (blocker)
	{
		square = (ChessBoard::Square)BBTwiddler::bitScanReverse(blocker);
		attacks ^= brd.RayAttacks[(int)square][(int)dir];
	}
	return attacks;
}

std::vector<_Move> PseudoLegalMoveGenerator::GenerateKnightMoves(Team t, const ChessBoard& brd)
{
	auto pieceBBs = brd.GetPieceBBs();
	std::vector<_Move> knightMoves;
	if (t == Team::BLACK)
	{
		const auto blackKnightsBB = pieceBBs[(int)ChessBoard::Pieces::Black] & pieceBBs[(int)ChessBoard::Pieces::Knights];
		const auto blackKnightSquares = brd.BitBoardToSquares(blackKnightsBB);
		for (const auto& square : blackKnightSquares)
		{
			//first we will compute quiet moves
			BitBoard attacks = brd.KnightAttacks[(int)square];
			BitBoard quiets = attacks & brd.empty;
			BitBoard captures = attacks & pieceBBs[(int)ChessBoard::Pieces::White];

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

	const auto whiteKnightsBB = pieceBBs[(int)ChessBoard::Pieces::White] & pieceBBs[(int)ChessBoard::Pieces::Knights];
	const auto whiteKnightSquares = brd.BitBoardToSquares(whiteKnightsBB);
	for (const auto& square : whiteKnightSquares)
	{
		//first we will compute quiet moves
		BitBoard attacks = brd.KnightAttacks[(int)square];
		BitBoard quiets = attacks & brd.empty;
		BitBoard captures = attacks & pieceBBs[(int)ChessBoard::Pieces::Black];

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
std::vector<_Move> PseudoLegalMoveGenerator::GenerateSlidingMoves(Team t, const ChessBoard& brd)
{
	std::vector<_Move> slidingMoves;
	if (t == Team::BLACK)
	{
		auto blackRooks = brd.pieceBBs[(int)ChessBoard::Pieces::Black] & brd.pieceBBs[(int)ChessBoard::Pieces::Rooks];
		auto blackRookSquares = ChessBoard::BitBoardToSquares(blackRooks);
		for (const auto square : blackRookSquares)
		{
			BitBoard attacks = 
				GetPositiveRayAttack(brd.occupied, Direction::North, square, brd) |
				GetPositiveRayAttack(brd.occupied, Direction::East, square, brd) |
				GetNegativeRayAttack(brd.occupied, Direction::South, square, brd) |
				GetNegativeRayAttack(brd.occupied, Direction::West, square, brd);
			BitBoard quiets = attacks & brd.empty;
			BitBoard captures = (attacks ^ quiets) & brd.pieceBBs[(int)ChessBoard::Pieces::White];

			auto quietSquares = ChessBoard::BitBoardToSquares(quiets);
			auto captureSquares = ChessBoard::BitBoardToSquares(captures);

			for (const auto target : quietSquares)
				slidingMoves.push_back({ (ushort)_Move::Flag::None, (ushort)square, (ushort)target, _Move::PieceType::Rook });
			for (const auto target : captureSquares)
			{
				auto capturePT = brd.ParseCapture(target);
				slidingMoves.push_back({ (ushort)_Move::Flag::Capture, (ushort)square, (ushort)target, _Move::PieceType::Rook, capturePT });
			}
		}

		auto blackBishops = brd.pieceBBs[(int)ChessBoard::Pieces::Black] & brd.pieceBBs[(int)ChessBoard::Pieces::Bishops];
		auto blackBishopSquares = ChessBoard::BitBoardToSquares(blackBishops);
		for (const auto square : blackBishopSquares)
		{
			BitBoard attacks =
				GetPositiveRayAttack(brd.occupied, Direction::Northeast, square, brd) |
				GetNegativeRayAttack(brd.occupied, Direction::Southeast, square, brd) |
				GetNegativeRayAttack(brd.occupied, Direction::Southwest, square, brd) |
				GetPositiveRayAttack(brd.occupied, Direction::Northwest, square, brd);
			BitBoard quiets = attacks & brd.empty;
			BitBoard captures = (attacks ^ quiets) & brd.pieceBBs[(int)ChessBoard::Pieces::White];

			auto quietSquares = ChessBoard::BitBoardToSquares(quiets);
			auto captureSquares = ChessBoard::BitBoardToSquares(captures);

			for (const auto target : quietSquares)
				slidingMoves.push_back({ (ushort)_Move::Flag::None, (ushort)square, (ushort)target, _Move::PieceType::Bishop });
			for (const auto target : captureSquares)
			{
				auto capturePT = brd.ParseCapture(target);
				slidingMoves.push_back({ (ushort)_Move::Flag::Capture, (ushort)square, (ushort)target, _Move::PieceType::Bishop, capturePT });
			}
		}

		auto blackQueens = brd.pieceBBs[(int)ChessBoard::Pieces::Black] & brd.pieceBBs[(int)ChessBoard::Pieces::Queens];
		auto blackQueenSquares = ChessBoard::BitBoardToSquares(blackQueens);
		for (const auto square : blackQueenSquares)
		{
			BitBoard attacks =
				GetPositiveRayAttack(brd.occupied, Direction::North, square, brd) |
				GetPositiveRayAttack(brd.occupied, Direction::East, square, brd) |
				GetNegativeRayAttack(brd.occupied, Direction::South, square, brd) |
				GetNegativeRayAttack(brd.occupied, Direction::West, square, brd) |
				GetPositiveRayAttack(brd.occupied, Direction::Northeast, square, brd) |
				GetNegativeRayAttack(brd.occupied, Direction::Southeast, square, brd) |
				GetNegativeRayAttack(brd.occupied, Direction::Southwest, square, brd) |
				GetPositiveRayAttack(brd.occupied, Direction::Northwest, square, brd);
			BitBoard quiets = attacks & brd.empty;
			BitBoard captures = (attacks ^ quiets) & brd.pieceBBs[(int)ChessBoard::Pieces::White];

			auto quietSquares = ChessBoard::BitBoardToSquares(quiets);
			auto captureSquares = ChessBoard::BitBoardToSquares(captures);

			for (const auto target : quietSquares)
				slidingMoves.push_back({ (ushort)_Move::Flag::None, (ushort)square, (ushort)target, _Move::PieceType::Queen });
			for (const auto target : captureSquares)
			{
				auto capturePT = brd.ParseCapture(target);
				slidingMoves.push_back({ (ushort)_Move::Flag::Capture, (ushort)square, (ushort)target, _Move::PieceType::Queen, capturePT });
			}
		}
		return slidingMoves;
	}

	auto whiteRooks = brd.pieceBBs[(int)ChessBoard::Pieces::White] & brd.pieceBBs[(int)ChessBoard::Pieces::Rooks];
	auto whiteRookSquares = ChessBoard::BitBoardToSquares(whiteRooks);
	for (const auto square : whiteRookSquares)
	{
		BitBoard attacks =
			GetPositiveRayAttack(brd.occupied, Direction::North, square, brd) |
			GetPositiveRayAttack(brd.occupied, Direction::East, square, brd) |
			GetNegativeRayAttack(brd.occupied, Direction::South, square, brd) |
			GetNegativeRayAttack(brd.occupied, Direction::West, square, brd);
		BitBoard quiets = attacks & brd.empty;
		BitBoard captures = (attacks ^ quiets) & brd.pieceBBs[(int)ChessBoard::Pieces::Black];

		auto quietSquares = ChessBoard::BitBoardToSquares(quiets);
		auto captureSquares = ChessBoard::BitBoardToSquares(captures);

		for (const auto target : quietSquares)
			slidingMoves.push_back({ (ushort)_Move::Flag::None, (ushort)square, (ushort)target, _Move::PieceType::Rook });
		for (const auto target : captureSquares)
		{
			auto capturePT = brd.ParseCapture(target);
			slidingMoves.push_back({ (ushort)_Move::Flag::Capture, (ushort)square, (ushort)target, _Move::PieceType::Rook, capturePT });
		}
	}

	auto whiteBishops = brd.pieceBBs[(int)ChessBoard::Pieces::White] & brd.pieceBBs[(int)ChessBoard::Pieces::Bishops];
	auto whiteBishopSquares = ChessBoard::BitBoardToSquares(whiteBishops);
	for (const auto square : whiteBishopSquares)
	{
		BitBoard attacks =
			GetPositiveRayAttack(brd.occupied, Direction::Northeast, square, brd) |
			GetNegativeRayAttack(brd.occupied, Direction::Southeast, square, brd) |
			GetNegativeRayAttack(brd.occupied, Direction::Southwest, square, brd) |
			GetPositiveRayAttack(brd.occupied, Direction::Northwest, square, brd);
		BitBoard quiets = attacks & brd.empty;
		BitBoard captures = (attacks ^ quiets) & brd.pieceBBs[(int)ChessBoard::Pieces::Black];

		auto quietSquares = ChessBoard::BitBoardToSquares(quiets);
		auto captureSquares = ChessBoard::BitBoardToSquares(captures);

		for (const auto target : quietSquares)
			slidingMoves.push_back({ (ushort)_Move::Flag::None, (ushort)square, (ushort)target, _Move::PieceType::Bishop });
		for (const auto target : captureSquares)
		{
			auto capturePT = brd.ParseCapture(target);
			slidingMoves.push_back({ (ushort)_Move::Flag::Capture, (ushort)square, (ushort)target, _Move::PieceType::Bishop, capturePT });
		}
	}

	auto whiteQueens = brd.pieceBBs[(int)ChessBoard::Pieces::White] & brd.pieceBBs[(int)ChessBoard::Pieces::Queens];
	auto whiteQueenSquares = ChessBoard::BitBoardToSquares(whiteQueens);
	for (const auto square : whiteQueenSquares)
	{
		BitBoard attacks =
			GetPositiveRayAttack(brd.occupied, Direction::North, square, brd) |
			GetPositiveRayAttack(brd.occupied, Direction::East, square, brd) |
			GetNegativeRayAttack(brd.occupied, Direction::South, square, brd) |
			GetNegativeRayAttack(brd.occupied, Direction::West, square, brd) |
			GetPositiveRayAttack(brd.occupied, Direction::Northeast, square, brd) |
			GetNegativeRayAttack(brd.occupied, Direction::Southeast, square, brd) |
			GetNegativeRayAttack(brd.occupied, Direction::Southwest, square, brd) |
			GetPositiveRayAttack(brd.occupied, Direction::Northwest, square, brd);
		BitBoard quiets = attacks & brd.empty;
		BitBoard captures = (attacks ^ quiets) & brd.pieceBBs[(int)ChessBoard::Pieces::Black];

		auto quietSquares = ChessBoard::BitBoardToSquares(quiets);
		auto captureSquares = ChessBoard::BitBoardToSquares(captures);

		for (const auto target : quietSquares)
			slidingMoves.push_back({ (ushort)_Move::Flag::None, (ushort)square, (ushort)target, _Move::PieceType::Queen });
		for (const auto target : captureSquares)
		{
			auto capturePT = brd.ParseCapture(target);
			slidingMoves.push_back({ (ushort)_Move::Flag::Capture, (ushort)square, (ushort)target, _Move::PieceType::Queen, capturePT });
		}
	}
	return slidingMoves;
}
std::vector<_Move> PseudoLegalMoveGenerator::GenerateMoves(Team t, const ChessBoard& brd)
{
	auto moves = GenerateKnightMoves(t, brd);
	auto moreMoves = GenerateSlidingMoves(t, brd);
	moves.insert(moves.end(), moreMoves.begin(), moreMoves.end());
	moreMoves = GeneratePawnMoves(t, brd);
	moves.insert(moves.end(), moreMoves.begin(), moreMoves.end());
	return moves;
}
BitBoard PseudoLegalMoveGenerator::SinglePushTargetsWhite(const ChessBoard& brd)
{
	auto whitePawns = brd.pieceBBs[(int)ChessBoard::Pieces::White] & brd.pieceBBs[(int)ChessBoard::Pieces::Pawns];
	return BBTwiddler::RankNorth(whitePawns) & brd.empty;
}

BitBoard PseudoLegalMoveGenerator::DoublePushTargetsWhite(const ChessBoard& brd)
{
	auto singlePushTargets = SinglePushTargetsWhite(brd);
	return BBTwiddler::RankNorth(singlePushTargets) & brd.empty & ChessBoard::Rank4;
}

BitBoard PseudoLegalMoveGenerator::SinglePushTargetsBlack(const ChessBoard& brd)
{
	auto blackPawns = brd.pieceBBs[(int)ChessBoard::Pieces::Black] & brd.pieceBBs[(int)ChessBoard::Pieces::Pawns];
	return BBTwiddler::RankSouth(blackPawns) & brd.empty;
}

BitBoard PseudoLegalMoveGenerator::DoublePushTargetsBlack(const ChessBoard& brd)
{
	auto singlePushTargets = SinglePushTargetsBlack(brd);
	return BBTwiddler::RankSouth(singlePushTargets) & brd.empty & ChessBoard::Rank5;
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
		BitBoard wPawns = brd.pieceBBs[(int)ChessBoard::Pieces::Pawns] & brd.pieceBBs[(int)ChessBoard::Pieces::White];
		BitBoard wPawnEastAttacks = WhitePawnEastAttacks(wPawns) & brd.pieceBBs[(int)ChessBoard::Pieces::Black];
		BitBoard wPawnEastPromotions = wPawnEastAttacks & ChessBoard::Rank8;
		wPawnEastAttacks ^= wPawnEastPromotions;
		BitBoard wPawnWestAttacks = WhitePawnWestAttacks(wPawns) & brd.pieceBBs[(int)ChessBoard::Pieces::Black];
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
	BitBoard bPawns = brd.pieceBBs[(int)ChessBoard::Pieces::Pawns] & brd.pieceBBs[(int)ChessBoard::Pieces::Black];
	BitBoard bPawnEastAttacks = BlackPawnEastAttacks(bPawns) & brd.pieceBBs[(int)ChessBoard::Pieces::White];
	BitBoard bPawnEastPromotions = bPawnEastAttacks & ChessBoard::Rank1;
	bPawnEastAttacks ^= bPawnEastPromotions;
	BitBoard bPawnWestAttacks = BlackPawnWestAttacks(bPawns) & brd.pieceBBs[(int)ChessBoard::Pieces::White];
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

BitBoard PseudoLegalMoveGenerator::WhitePawnEastAttacks(BitBoard wPawns)
{
	return BBTwiddler::NortheastOne(wPawns);
}
BitBoard PseudoLegalMoveGenerator::WhitePawnWestAttacks(BitBoard wPawns)
{
	return BBTwiddler::NorthwestOne(wPawns);
}
BitBoard PseudoLegalMoveGenerator::BlackPawnEastAttacks(BitBoard bPawns)
{
	return BBTwiddler::SoutheastOne(bPawns);
}
BitBoard PseudoLegalMoveGenerator::BlackPawnWestAttacks(BitBoard bPawns)
{
	return BBTwiddler::SouthwestOne(bPawns);
}

BitBoard PseudoLegalMoveGenerator::WhitePawnAttacks(BitBoard wPawns)
{
	return WhitePawnEastAttacks(wPawns) | WhitePawnWestAttacks(wPawns);
}

BitBoard PseudoLegalMoveGenerator::BlackPawnAttacks(BitBoard bPawns)
{
	return BlackPawnEastAttacks(bPawns) | BlackPawnWestAttacks(bPawns);
}