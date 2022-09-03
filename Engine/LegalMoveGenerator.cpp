#include "LegalMoveGenerator.h"
#include "PseudoLegalMoveGenerator.h"
std::vector<_Move> LegalMoveGenerator::GenerateKingMoves(Team t, const ChessBoard& brd)
{
	std::vector<_Move> moves;
	auto pieceBBs = brd.GetPieceBBs();
	auto kingPos = pieceBBs[(int)ChessBoard::BBIndex::Kings] & pieceBBs[(int)t];
	auto attackBB = BBTwiddler::KingAttacks(kingPos);
	auto dangers = brd.GetKingDangerSquares(t);
	auto safes = ChessBoard::Universe ^ dangers;
	auto attackSquares = ChessBoard::BitBoardToSquares(attackBB & pieceBBs[1 - (int)t] & safes);
	auto quietSquares = ChessBoard::BitBoardToSquares(attackBB & brd.GetEmptyBB() & safes);

	auto kingSquare = ChessBoard::BitBoardToSquare(kingPos);

	//handle castling
	if (brd.CanCastleKingside(t))
	{
		if (t == Team::WHITE)
			moves.push_back({ (uint)_Move::Flag::KingsideCastle, (uint)kingSquare, (uint)ChessBoard::Square::g1, PieceType::King, t });
		else
			moves.push_back({ (uint)_Move::Flag::KingsideCastle, (uint)kingSquare, (uint)ChessBoard::Square::g8, PieceType::King, t });
	}
	if (brd.CanCastleQueenside(t))
	{
		if (t == Team::WHITE)
			moves.push_back({ (uint)_Move::Flag::QueensideCastle, (uint)kingSquare, (uint)ChessBoard::Square::c1, PieceType::King, t });
		else
			moves.push_back({ (uint)_Move::Flag::KingsideCastle, (uint)kingSquare, (uint)ChessBoard::Square::c8, PieceType::King, t });
	}

	//handle normal moves
	for (const auto square : attackSquares)
	{
		auto capturePT = brd.ParseCapture(square);
		moves.push_back({ (uint)_Move::Flag::Capture, (uint)kingSquare, (uint)square, PieceType::King, capturePT, t });
	}
	for (const auto square : quietSquares)
	{
		moves.push_back({ (uint)_Move::Flag::None, (uint)kingSquare, (uint)square, PieceType::King, t });
	}
	return moves;
}

std::vector<_Move> GenerateKingMovesCheck(Team t, const ChessBoard& brd)
{
	std::vector<_Move> moves;
	auto pieceBBs = brd.GetPieceBBs();
	auto kingPos = pieceBBs[(int)ChessBoard::BBIndex::Kings] & pieceBBs[(int)t];
	auto attackBB = BBTwiddler::KingAttacks(kingPos);
	auto dangers = brd.GetKingDangerSquares(t);
	auto safes = ChessBoard::Universe ^ dangers;
	auto attackSquares = ChessBoard::BitBoardToSquares(attackBB & pieceBBs[1 - (int)t] & safes);
	auto quietSquares = ChessBoard::BitBoardToSquares(attackBB & brd.GetEmptyBB() & safes);
	auto kingSquare = ChessBoard::BitBoardToSquare(kingPos);

	for (const auto square : attackSquares)
	{
		auto capturePT = brd.ParseCapture(square);
		moves.push_back({ (uint)_Move::Flag::Capture, (uint)kingSquare, (uint)square, PieceType::King, capturePT, t });
	}
	for (const auto square : quietSquares)
	{
		moves.push_back({ (uint)_Move::Flag::None, (uint)kingSquare, (uint)square, PieceType::King, t });
	}
	return moves;
}

std::vector<_Move> LegalMoveGenerator::GenerateKnightMoves(Team t, const ChessBoard& brd)
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
			knightMoves.push_back({ (ushort)_Move::Flag::None, (ushort)square, (ushort)sq, PieceType::Knight, t });
		for (const auto& sq : captureSquares)
		{
			auto capturePT = brd.ParseCapture(sq);
			knightMoves.push_back({ (ushort)_Move::Flag::Capture, (ushort)square, (ushort)sq, PieceType::Knight, capturePT, t });
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
			knightMoves.push_back({ (ushort)_Move::Flag::None, (ushort)square, (ushort)sq, PieceType::Knight, t });
		for (const auto& sq : captureSquares)
		{
			auto capturePT = brd.ParseCapture(sq);
			knightMoves.push_back({ (ushort)_Move::Flag::Capture, (ushort)square, (ushort)sq, PieceType::Knight, capturePT, t });
		}
	}
	return knightMoves;
}

std::vector<_Move> LegalMoveGenerator::GenerateKnightMovesCheck(Team t, const ChessBoard& brd)
{
	Team other = (Team)(1 - (int)t);
	auto pieceBBs = brd.GetPieceBBs();
	auto pinCorridors = brd.GetCorridors();
	auto pins = brd.GetPins();
	auto checkCorridor = brd.GetCheckCorridor();

	std::vector<_Move> knightMoves;
	const auto freeKnights = pieceBBs[ChessBoard::BBIndex::Knights] & pieceBBs[t] & (ChessBoard::Universe ^ pins);
	const auto freeSquares = ChessBoard::BitBoardToSquares(freeKnights);
	for (const auto& square : freeSquares)
	{
		auto attacks = brd.KnightAttacks[square] & checkCorridor;
		auto quiets = attacks & brd.GetEmptyBB();
		auto captures = attacks & pieceBBs[other];

		auto quietSquares = ChessBoard::BitBoardToSquares(quiets);
		auto captureSquares = ChessBoard::BitBoardToSquares(captures);
		for (const auto& sq : quietSquares)
			knightMoves.push_back({ (ushort)_Move::Flag::None, (ushort)square, (ushort)sq, PieceType::Knight, t });
		for (const auto& sq : captureSquares)
		{
			auto capturePT = brd.ParseCapture(sq);
			knightMoves.push_back({ (ushort)_Move::Flag::Capture, (ushort)square, (ushort)sq, PieceType::Knight, capturePT, t });
		}
	}
	return knightMoves;
}

std::vector<_Move> GenerateRookMovesCheck(Team t, const ChessBoard& brd)
{
	std::vector<_Move> rookMoves;
	Team other = (Team)(1 - (int)t);
	auto pieceBBs = brd.GetPieceBBs();
	auto RayAttacks = brd.GetRayAttacks();
	auto occupied = pieceBBs[ChessBoard::BBIndex::Occupied];
	auto rooks = pieceBBs[t] & pieceBBs[ChessBoard::BBIndex::Rooks];
	auto pins = brd.GetPins();
	auto checkCorridor = brd.GetCheckCorridor();

	//handle non-pinned rooks
	auto freeRooks = rooks ^ (pins & rooks);
	auto freeRookSquares = ChessBoard::BitBoardToSquares(freeRooks);
	for (const auto square : freeRookSquares)
	{
		BitBoard attacks = BBTwiddler::GetRookAttackBBSingle(occupied, (int)square, RayAttacks) & checkCorridor;
		BitBoard quiets = attacks & brd.GetEmptyBB();
		BitBoard captures = attacks & pieceBBs[other];

		auto quietSquares = ChessBoard::BitBoardToSquares(quiets);
		auto captureSquares = ChessBoard::BitBoardToSquares(captures);

		for (const auto target : quietSquares)
			rookMoves.push_back({ (ushort)_Move::Flag::None, (ushort)square, (ushort)target, PieceType::Rook, t });
		for (const auto target : captureSquares)
		{
			auto capturePT = brd.ParseCapture(target);
			rookMoves.push_back({ (ushort)_Move::Flag::Capture, (ushort)square, (ushort)target, PieceType::Rook, capturePT, t });
		}
	}
	return rookMoves;
}

std::vector<_Move> GenerateBishopMovesCheck(Team t, const ChessBoard& brd)
{
	std::vector<_Move> bishopMoves;
	Team other = (Team)(1 - (int)t);
	auto pieceBBs = brd.GetPieceBBs();
	auto occupied = pieceBBs[ChessBoard::BBIndex::Occupied];
	auto RayAttacks = brd.GetRayAttacks();
	auto bishops = pieceBBs[t] & pieceBBs[ChessBoard::BBIndex::Bishops];
	auto pins = brd.GetPins();
	auto checkCorridor = brd.GetCheckCorridor();

	//handle non-pinned bishops
	auto freeBishops = bishops ^ (bishops & pins);
	auto freeBishopSquares = ChessBoard::BitBoardToSquares(freeBishops);
	for (const auto square : freeBishopSquares)
	{
		BitBoard attacks = BBTwiddler::GetBishopAttackBBSingle(occupied, (int)square, RayAttacks) & checkCorridor;
		BitBoard quiets = attacks & brd.GetEmptyBB();
		BitBoard captures = attacks & pieceBBs[other];

		auto quietSquares = ChessBoard::BitBoardToSquares(quiets);
		auto captureSquares = ChessBoard::BitBoardToSquares(captures);

		for (const auto target : quietSquares)
			bishopMoves.push_back({ (ushort)_Move::Flag::None, (ushort)square, (ushort)target, PieceType::Bishop, t });
		for (const auto target : captureSquares)
		{
			auto capturePT = brd.ParseCapture(target);
			bishopMoves.push_back({ (ushort)_Move::Flag::Capture, (ushort)square, (ushort)target, PieceType::Bishop, capturePT, t });
		}
	}
	return bishopMoves;
}

std::vector<_Move> GenerateQueenMovesCheck(Team t, const ChessBoard& brd)
{
	std::vector<_Move> queenMoves;
	Team other = (Team)(1 - (int)t);
	auto pieceBBs = brd.GetPieceBBs();
	auto occupied = pieceBBs[ChessBoard::BBIndex::Occupied];
	auto RayAttacks = brd.GetRayAttacks();
	auto queens = pieceBBs[t] & pieceBBs[ChessBoard::BBIndex::Queens];
	auto pins = brd.GetPins();
	auto checkCorridor = brd.GetCheckCorridor();

	//handle non-pinned queens
	auto freeQueens = queens ^ (queens & pins);
	auto freeQueenSquares = ChessBoard::BitBoardToSquares(freeQueens);
	for (const auto square : freeQueenSquares)
	{
		BitBoard attacks = BBTwiddler::GetQueenAttackBBSingle(occupied, (int)square, RayAttacks) & checkCorridor;
		BitBoard quiets = attacks & brd.GetEmptyBB();
		BitBoard captures = attacks & pieceBBs[other];

		auto quietSquares = ChessBoard::BitBoardToSquares(quiets);
		auto captureSquares = ChessBoard::BitBoardToSquares(captures);

		for (const auto target : quietSquares)
			queenMoves.push_back({ (ushort)_Move::Flag::None, (ushort)square, (ushort)target, PieceType::Queen, t });
		for (const auto target : captureSquares)
		{
			auto capturePT = brd.ParseCapture(target);
			queenMoves.push_back({ (ushort)_Move::Flag::Capture, (ushort)square, (ushort)target, PieceType::Queen, capturePT, t });
		}
	}
	return queenMoves;
}

std::vector<_Move> LegalMoveGenerator::GenerateSlidingMovesCheck(Team t, const ChessBoard& brd)
{
	auto moves = GenerateRookMovesCheck(t, brd);
	auto moreMoves = GenerateBishopMovesCheck(t, brd);
	moves.insert(moves.end(), moreMoves.begin(), moreMoves.end());
	moreMoves = GenerateQueenMovesCheck(t, brd);
	moves.insert(moves.end(), moreMoves.begin(), moreMoves.end());
	return moves;
}

std::vector<_Move> GeneratePawnMovesCheck(Team t, const ChessBoard& brd)
{
	std::vector<_Move> moves;
	auto pieceBBs = brd.GetPieceBBs();
	auto pins = brd.GetPins();
	auto empty = brd.GetEmptyBB();
	auto checkCorridor = brd.GetCheckCorridor();
	if (t == Team::WHITE)
	{

		BitBoard wPawns = pieceBBs[ChessBoard::BBIndex::Pawns] & pieceBBs[ChessBoard::BBIndex::White];
		auto freePawns = wPawns ^ (wPawns & pins);
		BitBoard singlePushTargetsNonPinned = PseudoLegalMoveGenerator::SinglePushTargetsWhite(freePawns, empty) & checkCorridor;
		BitBoard singlePushPromotionsNonPinned = singlePushTargetsNonPinned & ChessBoard::Rank8 & checkCorridor;
		singlePushTargetsNonPinned &= ChessBoard::NotRank8;
		BitBoard doublePushNonPinned = PseudoLegalMoveGenerator::DoublePushTargetsWhite(freePawns, empty) & checkCorridor;
		BitBoard wPawnEastAttacksNonPinned = BBTwiddler::WhitePawnEastAttacks(freePawns) & pieceBBs[ChessBoard::BBIndex::Black] & checkCorridor;
		BitBoard wPawnEastPromotionsNonPinned = wPawnEastAttacksNonPinned & ChessBoard::Rank8 & checkCorridor;
		wPawnEastAttacksNonPinned ^= wPawnEastPromotionsNonPinned;
		BitBoard wPawnWestAttacksNonPinned = BBTwiddler::WhitePawnWestAttacks(freePawns) & pieceBBs[ChessBoard::BBIndex::Black] & checkCorridor;
		BitBoard wPawnWestPromotionsNonPinned = wPawnWestAttacksNonPinned & ChessBoard::Rank8 & checkCorridor;
		wPawnWestAttacksNonPinned ^= wPawnWestPromotionsNonPinned;

		auto singlePushPromotionSquares = ChessBoard::BitBoardToSquares(singlePushPromotionsNonPinned);
		auto singlePushQuietSquares = ChessBoard::BitBoardToSquares(singlePushTargetsNonPinned);
		auto doublePushSquares = ChessBoard::BitBoardToSquares(doublePushNonPinned);
		auto attackSquaresEast = ChessBoard::BitBoardToSquares(wPawnEastAttacksNonPinned);
		auto promotionSquaresEast = ChessBoard::BitBoardToSquares(wPawnEastPromotionsNonPinned);
		auto attackSquaresWest = ChessBoard::BitBoardToSquares(wPawnWestAttacksNonPinned);
		auto promotionSquaresWest = ChessBoard::BitBoardToSquares(wPawnWestPromotionsNonPinned);


		for (const auto square : singlePushPromotionSquares)
		{
			moves.push_back({ (uint)_Move::Flag::KnightPromotion, (uint)square - 8, (uint)square, PieceType::Pawn, t });
			moves.push_back({ (uint)_Move::Flag::QueenPromotion, (uint)square - 8, (uint)square, PieceType::Pawn, t });
		}
		for (const auto square : singlePushQuietSquares)
		{
			moves.push_back({ (uint)_Move::Flag::None, (uint)square - 8, (uint)square, PieceType::Pawn, t });
		}
		for (const auto square : doublePushSquares)
			moves.push_back({ (uint)_Move::Flag::PawnDoublePush, (uint)square - 16, (uint)square, PieceType::Pawn, t });
		for (const auto square : attackSquaresEast)
		{
			auto capturePT = brd.ParseCapture(square);
			moves.push_back({ (uint)_Move::Flag::Capture, (uint)square - 9, (uint)square, PieceType::Pawn, capturePT, t });
		}
		for (const auto square : promotionSquaresEast)
		{
			auto capturePT = brd.ParseCapture(square);
			moves.push_back({ (uint)_Move::Flag::KnightPromotionCapture, (uint)square - 9, (uint)square, PieceType::Pawn, capturePT, t });
			moves.push_back({ (uint)_Move::Flag::QueenPromotionCapture, (uint)square - 9, (uint)square, PieceType::Pawn, capturePT, t });
		}
		for (const auto square : attackSquaresWest)
		{
			auto capturePT = brd.ParseCapture(square);
			moves.push_back({ (uint)_Move::Flag::Capture, (uint)square - 7, (uint)square, PieceType::Pawn, capturePT, t });
		}
		for (const auto square : promotionSquaresWest)
		{
			auto capturePT = brd.ParseCapture(square);
			moves.push_back({ (uint)_Move::Flag::KnightPromotionCapture, (uint)square - 7, (uint)square, PieceType::Pawn, capturePT, t });
			moves.push_back({ (uint)_Move::Flag::QueenPromotionCapture, (uint)square - 7, (uint)square, PieceType::Pawn, capturePT, t });
		}
		return moves;
	}


	BitBoard bPawns = pieceBBs[ChessBoard::BBIndex::Pawns] & pieceBBs[ChessBoard::BBIndex::Black];
	auto freePawns = bPawns ^ (bPawns & pins);
	BitBoard singlePushTargetsNonPinned = PseudoLegalMoveGenerator::SinglePushTargetsBlack(freePawns, empty) & checkCorridor;
	BitBoard singlePushPromotionsNonPinned = singlePushTargetsNonPinned & ChessBoard::Rank1 & checkCorridor;
	singlePushTargetsNonPinned &= ChessBoard::NotRank1;
	BitBoard doublePushNonPinned = PseudoLegalMoveGenerator::DoublePushTargetsBlack(freePawns, empty) & checkCorridor;
	BitBoard bPawnEastAttacksNonPinned = BBTwiddler::BlackPawnEastAttacks(freePawns) & pieceBBs[ChessBoard::BBIndex::White] & checkCorridor;
	BitBoard bPawnEastPromotionsNonPinned = bPawnEastAttacksNonPinned & ChessBoard::Rank1 & checkCorridor;
	bPawnEastAttacksNonPinned ^= bPawnEastPromotionsNonPinned;
	BitBoard bPawnWestAttacksNonPinned = BBTwiddler::BlackPawnWestAttacks(freePawns) & pieceBBs[ChessBoard::BBIndex::White] & checkCorridor;
	BitBoard bPawnWestPromotionsNonPinned = bPawnWestAttacksNonPinned & ChessBoard::Rank1 & checkCorridor;
	bPawnWestAttacksNonPinned ^= bPawnWestPromotionsNonPinned;

	auto singlePushPromotionSquares = ChessBoard::BitBoardToSquares(singlePushPromotionsNonPinned);
	auto singlePushQuietSquares = ChessBoard::BitBoardToSquares(singlePushTargetsNonPinned);
	auto doublePushSquares = ChessBoard::BitBoardToSquares(doublePushNonPinned);
	auto attackSquaresEast = ChessBoard::BitBoardToSquares(bPawnEastAttacksNonPinned);
	auto promotionSquaresEast = ChessBoard::BitBoardToSquares(bPawnEastPromotionsNonPinned);
	auto attackSquaresWest = ChessBoard::BitBoardToSquares(bPawnWestAttacksNonPinned);
	auto promotionSquaresWest = ChessBoard::BitBoardToSquares(bPawnWestPromotionsNonPinned);


	for (const auto square : singlePushPromotionSquares)
	{
		moves.push_back({ (uint)_Move::Flag::KnightPromotion, (uint)square + 8, (uint)square, PieceType::Pawn, t });
		moves.push_back({ (uint)_Move::Flag::QueenPromotion, (uint)square + 8, (uint)square, PieceType::Pawn, t });
	}
	for (const auto square : singlePushQuietSquares)
	{
		moves.push_back({ (uint)_Move::Flag::None, (uint)square + 8, (uint)square, PieceType::Pawn, t });
	}
	for (const auto square : doublePushSquares)
		moves.push_back({ (uint)_Move::Flag::PawnDoublePush, (uint)square + 16, (uint)square, PieceType::Pawn, t });
	for (const auto square : attackSquaresEast)
	{
		auto capturePT = brd.ParseCapture(square);
		moves.push_back({ (uint)_Move::Flag::Capture, (uint)square + 7, (uint)square, PieceType::Pawn, capturePT, t });
	}
	for (const auto square : promotionSquaresEast)
	{
		auto capturePT = brd.ParseCapture(square);
		moves.push_back({ (uint)_Move::Flag::KnightPromotionCapture, (uint)square + 7, (uint)square, PieceType::Pawn, capturePT, t });
		moves.push_back({ (uint)_Move::Flag::QueenPromotionCapture, (uint)square + 7, (uint)square, PieceType::Pawn, capturePT, t });
	}
	for (const auto square : attackSquaresWest)
	{
		auto capturePT = brd.ParseCapture(square);
		moves.push_back({ (uint)_Move::Flag::Capture, (uint)square + 9, (uint)square, PieceType::Pawn, capturePT, t });
	}
	for (const auto square : promotionSquaresWest)
	{
		auto capturePT = brd.ParseCapture(square);
		moves.push_back({ (uint)_Move::Flag::KnightPromotionCapture, (uint)square + 9, (uint)square, PieceType::Pawn, capturePT, t });
		moves.push_back({ (uint)_Move::Flag::QueenPromotionCapture, (uint)square + 9, (uint)square, PieceType::Pawn, capturePT, t });
	}
	return moves;
}

std::vector<_Move> GenerateRookMoves(Team t, const ChessBoard& brd)
{
	std::vector<_Move> rookMoves;
	Team other = (Team)(1 - (int)t);
	auto pieceBBs = brd.GetPieceBBs();
	auto RayAttacks = brd.GetRayAttacks();
	auto occupied = pieceBBs[ChessBoard::BBIndex::Occupied];
	auto rooks = pieceBBs[t] & pieceBBs[ChessBoard::BBIndex::Rooks];
	auto pins = brd.GetPins();
	auto corridors = brd.GetCorridors();

	//handle pins first
	auto pinnedRooks = rooks & pins;
	auto pinnedRookSquares = ChessBoard::BitBoardToSquares(pinnedRooks);
	for (const auto square : pinnedRookSquares)
	{
		BitBoard attacks = BBTwiddler::GetRookAttackBBSingle(occupied, (int)square, RayAttacks) & corridors[square];
		BitBoard quiets = attacks & brd.GetEmptyBB();
		BitBoard captures = attacks & pieceBBs[other];

		auto quietSquares = ChessBoard::BitBoardToSquares(quiets);
		auto captureSquares = ChessBoard::BitBoardToSquares(captures);

		for (const auto target : quietSquares)
			rookMoves.push_back({ (ushort)_Move::Flag::None, (ushort)square, (ushort)target, PieceType::Rook, t });
		for (const auto target : captureSquares)
		{
			auto capturePT = brd.ParseCapture(target);
			rookMoves.push_back({ (ushort)_Move::Flag::Capture, (ushort)square, (ushort)target, PieceType::Rook, capturePT, t });
		}
	}

	//handle non-pinned rooks
	auto freeRooks = rooks ^ pinnedRooks;
	auto freeRookSquares = ChessBoard::BitBoardToSquares(freeRooks);
	for (const auto square : freeRookSquares)
	{
		BitBoard attacks = BBTwiddler::GetRookAttackBBSingle(occupied, (int)square, RayAttacks);
		BitBoard quiets = attacks & brd.GetEmptyBB();
		BitBoard captures = attacks & pieceBBs[other];

		auto quietSquares = ChessBoard::BitBoardToSquares(quiets);
		auto captureSquares = ChessBoard::BitBoardToSquares(captures);

		for (const auto target : quietSquares)
			rookMoves.push_back({ (ushort)_Move::Flag::None, (ushort)square, (ushort)target, PieceType::Rook, t });
		for (const auto target : captureSquares)
		{
			auto capturePT = brd.ParseCapture(target);
			rookMoves.push_back({ (ushort)_Move::Flag::Capture, (ushort)square, (ushort)target, PieceType::Rook, capturePT, t });
		}
	}
	return rookMoves;
}

std::vector<_Move> GenerateBishopMoves(Team t, const ChessBoard& brd)
{
	std::vector<_Move> bishopMoves;
	Team other = (Team)(1 - (int)t);
	auto pieceBBs = brd.GetPieceBBs();
	auto occupied = pieceBBs[ChessBoard::BBIndex::Occupied];
	auto RayAttacks = brd.GetRayAttacks();
	auto bishops = pieceBBs[t] & pieceBBs[ChessBoard::BBIndex::Bishops];
	auto pins = brd.GetPins();
	auto corridors = brd.GetCorridors();


	auto pinnedBishops = bishops & pins;
	auto pinnedBishopSquares = ChessBoard::BitBoardToSquares(pinnedBishops);
	for (const auto square : pinnedBishopSquares)
	{
		BitBoard attacks = BBTwiddler::GetBishopAttackBBSingle(occupied, (int)square, RayAttacks) & corridors[square];
		BitBoard quiets = attacks & brd.GetEmptyBB();
		BitBoard captures = attacks & pieceBBs[other];

		auto quietSquares = ChessBoard::BitBoardToSquares(quiets);
		auto captureSquares = ChessBoard::BitBoardToSquares(captures);

		for (const auto target : quietSquares)
			bishopMoves.push_back({ (ushort)_Move::Flag::None, (ushort)square, (ushort)target, PieceType::Bishop, t });
		for (const auto target : captureSquares)
		{
			auto capturePT = brd.ParseCapture(target);
			bishopMoves.push_back({ (ushort)_Move::Flag::Capture, (ushort)square, (ushort)target, PieceType::Bishop, capturePT, t });
		}
	}

	//handle non-pinned bishops
	auto freeBishops = bishops ^ pinnedBishops;
	auto freeBishopSquares = ChessBoard::BitBoardToSquares(freeBishops);
	for (const auto square : freeBishopSquares)
	{
		BitBoard attacks = BBTwiddler::GetBishopAttackBBSingle(occupied, (int)square, RayAttacks);
		BitBoard quiets = attacks & brd.GetEmptyBB();
		BitBoard captures = attacks & pieceBBs[other];

		auto quietSquares = ChessBoard::BitBoardToSquares(quiets);
		auto captureSquares = ChessBoard::BitBoardToSquares(captures);

		for (const auto target : quietSquares)
			bishopMoves.push_back({ (ushort)_Move::Flag::None, (ushort)square, (ushort)target, PieceType::Bishop, t });
		for (const auto target : captureSquares)
		{
			auto capturePT = brd.ParseCapture(target);
			bishopMoves.push_back({ (ushort)_Move::Flag::Capture, (ushort)square, (ushort)target, PieceType::Bishop, capturePT, t });
		}
	}
	return bishopMoves;
}

std::vector<_Move> GenerateQueenMoves(Team t, const ChessBoard& brd)
{
	std::vector<_Move> queenMoves;
	Team other = (Team)(1 - (int)t);
	auto pieceBBs = brd.GetPieceBBs();
	auto occupied = pieceBBs[ChessBoard::BBIndex::Occupied];
	auto RayAttacks = brd.GetRayAttacks();
	auto queens = pieceBBs[t] & pieceBBs[ChessBoard::BBIndex::Queens];
	auto pins = brd.GetPins();
	auto corridors = brd.GetCorridors();


	auto pinnedQueens = queens & pins;
	auto pinnedQueenSquares = ChessBoard::BitBoardToSquares(pinnedQueens);
	for (const auto square : pinnedQueenSquares)
	{
		BitBoard attacks = BBTwiddler::GetQueenAttackBBSingle(occupied, (int)square, RayAttacks) & corridors[square];
		BitBoard quiets = attacks & brd.GetEmptyBB();
		BitBoard captures = attacks & pieceBBs[other];

		auto quietSquares = ChessBoard::BitBoardToSquares(quiets);
		auto captureSquares = ChessBoard::BitBoardToSquares(captures);

		for (const auto target : quietSquares)
			queenMoves.push_back({ (ushort)_Move::Flag::None, (ushort)square, (ushort)target, PieceType::Queen, t });
		for (const auto target : captureSquares)
		{
			auto capturePT = brd.ParseCapture(target);
			queenMoves.push_back({ (ushort)_Move::Flag::Capture, (ushort)square, (ushort)target, PieceType::Queen, capturePT, t });
		}
	}

	//handle non-pinned queens
	auto freeQueens = queens ^ pinnedQueens;
	auto freeQueenSquares = ChessBoard::BitBoardToSquares(freeQueens);
	for (const auto square : freeQueenSquares)
	{
		BitBoard attacks = BBTwiddler::GetQueenAttackBBSingle(occupied, (int)square, RayAttacks);
		BitBoard quiets = attacks & brd.GetEmptyBB();
		BitBoard captures = attacks & pieceBBs[other];

		auto quietSquares = ChessBoard::BitBoardToSquares(quiets);
		auto captureSquares = ChessBoard::BitBoardToSquares(captures);

		for (const auto target : quietSquares)
			queenMoves.push_back({ (ushort)_Move::Flag::None, (ushort)square, (ushort)target, PieceType::Queen, t });
		for (const auto target : captureSquares)
		{
			auto capturePT = brd.ParseCapture(target);
			queenMoves.push_back({ (ushort)_Move::Flag::Capture, (ushort)square, (ushort)target, PieceType::Queen, capturePT, t });
		}
	}
	return queenMoves;
}

std::vector<_Move> LegalMoveGenerator::GenerateSlidingMoves(Team t, const ChessBoard& brd)
{
	std::vector<_Move> result = GenerateRookMoves(t, brd);
	auto moreMoves = GenerateBishopMoves(t, brd);
	result.insert(result.end(), moreMoves.begin(), moreMoves.end());
	moreMoves = GenerateQueenMoves(t, brd);
	result.insert(result.end(), moreMoves.begin(), moreMoves.end());
	return result;
}

//TODO - Check if this code works, and if some of the cases can be simplified.
std::vector<_Move> LegalMoveGenerator::GeneratePawnMoves(Team t, const ChessBoard& brd)
{
	std::vector<_Move> moves;
	auto pieceBBs = brd.GetPieceBBs();
	auto pins = brd.GetPins();
	auto corridors = brd.GetCorridors();
	auto empty = brd.GetEmptyBB();
	auto enPassantAttackers = brd.GetEnPassantAttackers();
	if (t == Team::WHITE)
	{

		BitBoard wPawns = pieceBBs[ChessBoard::BBIndex::Pawns] & pieceBBs[ChessBoard::BBIndex::White];
		auto pinnedPawns = pins & wPawns;
		auto freePawns = wPawns ^ pinnedPawns;
		BitBoard singlePushTargetsPinned = PseudoLegalMoveGenerator::SinglePushTargetsWhite(pinnedPawns, empty);
		BitBoard singlePushTargetsNonPinned = PseudoLegalMoveGenerator::SinglePushTargetsWhite(freePawns, empty);
		BitBoard singlePushPromotionsPinned = singlePushTargetsPinned & ChessBoard::Rank8;
		BitBoard singlePushPromotionsNonPinned = singlePushTargetsNonPinned & ChessBoard::Rank8;
		singlePushTargetsNonPinned &= ChessBoard::NotRank8;
		singlePushTargetsNonPinned &= ChessBoard::NotRank8;
		BitBoard doublePushTargets = PseudoLegalMoveGenerator::DoublePushTargetsWhite(wPawns, empty);
		BitBoard doublePushNonPinned = PseudoLegalMoveGenerator::DoublePushTargetsWhite(freePawns, empty);
		BitBoard wPawnEastAttacksPinned = BBTwiddler::WhitePawnEastAttacks(pinnedPawns) & pieceBBs[ChessBoard::BBIndex::Black];
		BitBoard wPawnEastAttacksNonPinned = BBTwiddler::WhitePawnEastAttacks(freePawns) & pieceBBs[ChessBoard::BBIndex::Black];
		BitBoard wPawnEastPromotionsPinned = wPawnEastAttacksPinned & ChessBoard::Rank8;
		BitBoard wPawnEastPromotionsNonPinned = wPawnEastAttacksNonPinned & ChessBoard::Rank8;
		wPawnEastAttacksPinned ^= wPawnEastPromotionsPinned;
		wPawnEastAttacksNonPinned ^= wPawnEastPromotionsNonPinned;
		BitBoard wPawnWestAttacksPinned = BBTwiddler::WhitePawnWestAttacks(pinnedPawns) & pieceBBs[ChessBoard::BBIndex::Black];
		BitBoard wPawnWestAttacksNonPinned = BBTwiddler::WhitePawnWestAttacks(freePawns) & pieceBBs[ChessBoard::BBIndex::Black];
		BitBoard wPawnWestPromotionsPinned = wPawnWestAttacksPinned & ChessBoard::Rank8;
		BitBoard wPawnWestPromotionsNonPinned = wPawnWestAttacksNonPinned & ChessBoard::Rank8;
		wPawnWestAttacksNonPinned ^= wPawnWestPromotionsNonPinned;
		wPawnWestAttacksPinned ^= wPawnWestPromotionsPinned;

		auto pinnedPassantAttackers = enPassantAttackers & pins;
		auto freePassantAttackers = enPassantAttackers ^ pinnedPassantAttackers;

		auto singlePushPromotionSquares = ChessBoard::BitBoardToSquares(singlePushPromotionsNonPinned);
		auto singlePushQuietSquares = ChessBoard::BitBoardToSquares(singlePushTargetsNonPinned);
		auto doublePushSquares = ChessBoard::BitBoardToSquares(doublePushNonPinned);
		auto attackSquaresEast = ChessBoard::BitBoardToSquares(wPawnEastAttacksNonPinned);
		auto promotionSquaresEast = ChessBoard::BitBoardToSquares(wPawnEastPromotionsNonPinned);
		auto attackSquaresWest = ChessBoard::BitBoardToSquares(wPawnWestAttacksNonPinned);
		auto promotionSquaresWest = ChessBoard::BitBoardToSquares(wPawnWestPromotionsNonPinned);
		auto passantSquaresPinned = ChessBoard::BitBoardToSquares(pinnedPassantAttackers);
		auto passantSquaresFree = ChessBoard::BitBoardToSquares(freePassantAttackers);

		//handle pinned pawns
		auto pinnedPawnSquares = ChessBoard::BitBoardToSquares(pinnedPawns);
		for (const auto square : pinnedPawnSquares)
		{
			auto pinCorridor = corridors[square];
			auto singlePush = singlePushTargetsPinned & pinCorridor;
			if (singlePush)
			{
				ChessBoard::Square singleTarget = ChessBoard::BitBoardToSquare(singlePush);
				moves.push_back({ (uint)_Move::Flag::PawnDoublePush, (uint)square, (uint)singleTarget, PieceType::Pawn, t });

				//cant double push if we can't single push
				auto doublePush = doublePushTargets & corridors[square];
				if (doublePush)
				{
					ChessBoard::Square doubleTarget = ChessBoard::BitBoardToSquare(doublePush);
					moves.push_back({ (uint)_Move::Flag::PawnDoublePush, (uint)square, (uint)doubleTarget, PieceType::Pawn, t });
				}
			}

			auto eastAttack = wPawnEastAttacksPinned & pinCorridor & pieceBBs[ChessBoard::BBIndex::Black];
			auto westAttack = wPawnWestAttacksPinned & pinCorridor & pieceBBs[ChessBoard::BBIndex::Black];
			if (eastAttack)
			{
				ChessBoard::Square eastTarget = ChessBoard::BitBoardToSquare(eastAttack);
				auto capturePT = brd.ParseCapture(eastTarget);
				moves.push_back({ (uint)_Move::Flag::Capture, (uint)square, (uint)eastTarget, PieceType::Pawn, capturePT, t });
			}
			else if (westAttack)
			{
				ChessBoard::Square westTarget = ChessBoard::BitBoardToSquare(westAttack);
				auto capturePT = brd.ParseCapture(westTarget);
				moves.push_back({ (uint)_Move::Flag::Capture, (uint)square, (uint)westTarget, PieceType::Pawn, capturePT, t });
			}

			auto eastPromotion = wPawnEastPromotionsPinned & pinCorridor & pieceBBs[ChessBoard::BBIndex::Black];
			auto westPromotion = wPawnWestPromotionsPinned & pinCorridor & pieceBBs[ChessBoard::BBIndex::Black];
			if (eastPromotion)
			{
				ChessBoard::Square eastTarget = ChessBoard::BitBoardToSquare(eastPromotion);
				auto capturePT = brd.ParseCapture(eastTarget);
				moves.push_back({ (uint)_Move::Flag::KnightPromotionCapture, (uint)square, (uint)eastTarget, PieceType::Pawn, capturePT, t });
				moves.push_back({ (uint)_Move::Flag::QueenPromotionCapture, (uint)square, (uint)eastTarget, PieceType::Pawn, capturePT, t });
			}
			else if (westPromotion)
			{
				ChessBoard::Square westTarget = ChessBoard::BitBoardToSquare(westPromotion);
				auto capturePT = brd.ParseCapture(westTarget);
				moves.push_back({ (uint)_Move::Flag::KnightPromotionCapture, (uint)square, (uint)westTarget, PieceType::Pawn, capturePT, t });
				moves.push_back({ (uint)_Move::Flag::QueenPromotionCapture, (uint)square, (uint)westTarget, PieceType::Pawn, capturePT, t });
			}
		}
		for (const auto square : passantSquaresPinned)
		{
			auto pinCorridor = corridors[square];
			auto enPassantSquareBB = brd.GetEnPassantSquareBB();
			auto passantAttack = enPassantSquareBB & pinCorridor;
			if (passantAttack)
			{
				moves.push_back({ (uint)_Move::Flag::EnPassant, (uint)square, (uint)ChessBoard::BitBoardToSquare(enPassantSquareBB), PieceType::Pawn, PieceType::Pawn, t });
			}
		}
		for (const auto square : passantSquaresFree)
		{
			auto enPassantSquareBB = brd.GetEnPassantSquareBB();
			moves.push_back({ (uint)_Move::Flag::EnPassant, (uint)square, (uint)ChessBoard::BitBoardToSquare(enPassantSquareBB), PieceType::Pawn, PieceType::Pawn, t});
		}
		for (const auto square : singlePushPromotionSquares)
		{
			moves.push_back({ (uint)_Move::Flag::KnightPromotion, (uint)square - 8, (uint)square, PieceType::Pawn, t });
			moves.push_back({ (uint)_Move::Flag::QueenPromotion, (uint)square - 8, (uint)square, PieceType::Pawn, t });
		}
		for (const auto square : singlePushQuietSquares)
		{
			moves.push_back({ (uint)_Move::Flag::None, (uint)square - 8, (uint)square, PieceType::Pawn, t });
		}
		for (const auto square : doublePushSquares)
			moves.push_back({ (uint)_Move::Flag::PawnDoublePush, (uint)square - 16, (uint)square, PieceType::Pawn, t });
		for (const auto square : attackSquaresEast)
		{
			auto capturePT = brd.ParseCapture(square);
			moves.push_back({ (uint)_Move::Flag::Capture, (uint)square - 9, (uint)square, PieceType::Pawn, capturePT, t });
		}
		for (const auto square : promotionSquaresEast)
		{
			auto capturePT = brd.ParseCapture(square);
			moves.push_back({ (uint)_Move::Flag::KnightPromotionCapture, (uint)square - 9, (uint)square, PieceType::Pawn, capturePT, t });
			moves.push_back({ (uint)_Move::Flag::QueenPromotionCapture, (uint)square - 9, (uint)square, PieceType::Pawn, capturePT, t });
		}
		for (const auto square : attackSquaresWest)
		{
			auto capturePT = brd.ParseCapture(square);
			moves.push_back({ (uint)_Move::Flag::Capture, (uint)square - 7, (uint)square, PieceType::Pawn, capturePT, t });
		}
		for (const auto square : promotionSquaresWest)
		{
			auto capturePT = brd.ParseCapture(square);
			moves.push_back({ (uint)_Move::Flag::KnightPromotionCapture, (uint)square - 7, (uint)square, PieceType::Pawn, capturePT, t });
			moves.push_back({ (uint)_Move::Flag::QueenPromotionCapture, (uint)square - 7, (uint)square, PieceType::Pawn, capturePT, t });
		}
		return moves;
	}


	BitBoard bPawns = pieceBBs[ChessBoard::BBIndex::Pawns] & pieceBBs[ChessBoard::BBIndex::Black];
	auto pinnedPawns = pins & bPawns;
	auto freePawns = bPawns ^ pinnedPawns;
	auto pinnedPassantAttackers = enPassantAttackers & pins;
	auto freePassantAttackers = enPassantAttackers ^ pinnedPassantAttackers;
	BitBoard singlePushTargetsPinned = PseudoLegalMoveGenerator::SinglePushTargetsBlack(pinnedPawns, empty);
	BitBoard singlePushTargetsNonPinned = PseudoLegalMoveGenerator::SinglePushTargetsBlack(freePawns, empty);
	BitBoard singlePushPromotionsPinned = singlePushTargetsPinned & ChessBoard::Rank1;
	BitBoard singlePushPromotionsNonPinned = singlePushTargetsNonPinned & ChessBoard::Rank1;
	singlePushTargetsNonPinned &= ChessBoard::NotRank1;
	singlePushTargetsNonPinned &= ChessBoard::NotRank1;
	BitBoard doublePushTargets = PseudoLegalMoveGenerator::DoublePushTargetsBlack(bPawns, empty);
	BitBoard doublePushNonPinned = PseudoLegalMoveGenerator::DoublePushTargetsBlack(freePawns, empty);
	BitBoard bPawnEastAttacksPinned = BBTwiddler::BlackPawnEastAttacks(pinnedPawns) & pieceBBs[ChessBoard::BBIndex::White];
	BitBoard bPawnEastAttacksNonPinned = BBTwiddler::BlackPawnEastAttacks(freePawns) & pieceBBs[ChessBoard::BBIndex::White];
	BitBoard bPawnEastPromotionsPinned = bPawnEastAttacksPinned & ChessBoard::Rank1;
	BitBoard bPawnEastPromotionsNonPinned = bPawnEastAttacksNonPinned & ChessBoard::Rank1;
	bPawnEastAttacksPinned ^= bPawnEastPromotionsPinned;
	bPawnEastAttacksNonPinned ^= bPawnEastPromotionsNonPinned;
	BitBoard bPawnWestAttacksPinned = BBTwiddler::BlackPawnWestAttacks(pinnedPawns) & pieceBBs[ChessBoard::BBIndex::White];
	BitBoard bPawnWestAttacksNonPinned = BBTwiddler::BlackPawnWestAttacks(freePawns) & pieceBBs[ChessBoard::BBIndex::White];
	BitBoard bPawnWestPromotionsPinned = bPawnWestAttacksPinned & ChessBoard::Rank1;
	BitBoard bPawnWestPromotionsNonPinned = bPawnWestAttacksNonPinned & ChessBoard::Rank1;
	bPawnWestAttacksNonPinned ^= bPawnWestPromotionsNonPinned;
	bPawnWestAttacksPinned ^= bPawnWestPromotionsPinned;

	auto singlePushPromotionSquares = ChessBoard::BitBoardToSquares(singlePushPromotionsNonPinned);
	auto singlePushQuietSquares = ChessBoard::BitBoardToSquares(singlePushTargetsNonPinned);
	auto doublePushSquares = ChessBoard::BitBoardToSquares(doublePushNonPinned);
	auto attackSquaresEast = ChessBoard::BitBoardToSquares(bPawnEastAttacksNonPinned);
	auto promotionSquaresEast = ChessBoard::BitBoardToSquares(bPawnEastPromotionsNonPinned);
	auto attackSquaresWest = ChessBoard::BitBoardToSquares(bPawnWestAttacksNonPinned);
	auto promotionSquaresWest = ChessBoard::BitBoardToSquares(bPawnWestPromotionsNonPinned);
	auto passantSquaresPinned = ChessBoard::BitBoardToSquares(pinnedPassantAttackers);
	auto passantSquaresFree = ChessBoard::BitBoardToSquares(freePassantAttackers);

	//handle pinned pawns
	auto pinnedPawnSquares = ChessBoard::BitBoardToSquares(pinnedPawns);
	for (const auto square : pinnedPawnSquares)
	{
		auto pinCorridor = corridors[square];
		auto singlePush = singlePushTargetsPinned & pinCorridor;
		if (singlePush)
		{
			ChessBoard::Square singleTarget = ChessBoard::BitBoardToSquare(singlePush);
			moves.push_back({ (uint)_Move::Flag::PawnDoublePush, (uint)square, (uint)singleTarget, PieceType::Pawn, t });

			//cant double push if we can't single push
			auto doublePush = doublePushTargets & corridors[square];
			if (doublePush)
			{
				ChessBoard::Square doubleTarget = ChessBoard::BitBoardToSquare(doublePush);
				moves.push_back({ (uint)_Move::Flag::PawnDoublePush, (uint)square, (uint)doubleTarget, PieceType::Pawn, t });
			}
		}

		auto eastAttack = bPawnEastAttacksPinned & pinCorridor & pieceBBs[ChessBoard::BBIndex::White];
		auto westAttack = bPawnWestAttacksPinned & pinCorridor & pieceBBs[ChessBoard::BBIndex::White];
		if (eastAttack)
		{
			ChessBoard::Square eastTarget = ChessBoard::BitBoardToSquare(eastAttack);
			auto capturePT = brd.ParseCapture(eastTarget);
			moves.push_back({ (uint)_Move::Flag::Capture, (uint)square, (uint)eastTarget, PieceType::Pawn, capturePT, t });
		}
		else if (westAttack)
		{
			ChessBoard::Square westTarget = ChessBoard::BitBoardToSquare(westAttack);
			auto capturePT = brd.ParseCapture(westTarget);
			moves.push_back({ (uint)_Move::Flag::Capture, (uint)square, (uint)westTarget, PieceType::Pawn, capturePT, t });
		}

		auto eastPromotion = bPawnEastPromotionsPinned & pinCorridor & pieceBBs[ChessBoard::BBIndex::White];
		auto westPromotion = bPawnWestPromotionsPinned & pinCorridor & pieceBBs[ChessBoard::BBIndex::White];
		if (eastPromotion)
		{
			ChessBoard::Square eastTarget = ChessBoard::BitBoardToSquare(eastPromotion);
			auto capturePT = brd.ParseCapture(eastTarget);
			moves.push_back({ (uint)_Move::Flag::KnightPromotionCapture, (uint)square, (uint)eastTarget, PieceType::Pawn, capturePT, t });
			moves.push_back({ (uint)_Move::Flag::QueenPromotionCapture, (uint)square, (uint)eastTarget, PieceType::Pawn, capturePT, t });
		}
		else if (westPromotion)
		{
			ChessBoard::Square westTarget = ChessBoard::BitBoardToSquare(westPromotion);
			auto capturePT = brd.ParseCapture(westTarget);
			moves.push_back({ (uint)_Move::Flag::KnightPromotionCapture, (uint)square, (uint)westTarget, PieceType::Pawn, capturePT, t });
			moves.push_back({ (uint)_Move::Flag::QueenPromotionCapture, (uint)square, (uint)westTarget, PieceType::Pawn, capturePT, t });
		}
	}

	for (const auto square : passantSquaresPinned)
	{
		auto pinCorridor = corridors[square];
		auto enPassantSquareBB = brd.GetEnPassantSquareBB();
		auto passantAttack = enPassantSquareBB & pinCorridor;
		if (passantAttack)
		{
			moves.push_back({ (uint)_Move::Flag::EnPassant, (uint)square, (uint)ChessBoard::BitBoardToSquare(enPassantSquareBB), PieceType::Pawn, PieceType::Pawn, t });
		}
	}
	for (const auto square : passantSquaresFree)
	{
		auto enPassantSquareBB = brd.GetEnPassantSquareBB();
		moves.push_back({ (uint)_Move::Flag::EnPassant, (uint)square, (uint)ChessBoard::BitBoardToSquare(enPassantSquareBB), PieceType::Pawn, PieceType::Pawn, t });
	}
	for (const auto square : singlePushPromotionSquares)
	{
		moves.push_back({ (uint)_Move::Flag::KnightPromotion, (uint)square + 8, (uint)square, PieceType::Pawn, t });
		moves.push_back({ (uint)_Move::Flag::QueenPromotion, (uint)square + 8, (uint)square, PieceType::Pawn, t });
	}
	for (const auto square : singlePushQuietSquares)
	{
		moves.push_back({ (uint)_Move::Flag::None, (uint)square + 8, (uint)square, PieceType::Pawn, t });
	}
	for (const auto square : doublePushSquares)
		moves.push_back({ (uint)_Move::Flag::PawnDoublePush, (uint)square + 16, (uint)square, PieceType::Pawn, t });
	for (const auto square : attackSquaresEast)
	{
		auto capturePT = brd.ParseCapture(square);
		moves.push_back({ (uint)_Move::Flag::Capture, (uint)square + 7, (uint)square, PieceType::Pawn, capturePT, t });
	}
	for (const auto square : promotionSquaresEast)
	{
		auto capturePT = brd.ParseCapture(square);
		moves.push_back({ (uint)_Move::Flag::KnightPromotionCapture, (uint)square + 7, (uint)square, PieceType::Pawn, capturePT, t });
		moves.push_back({ (uint)_Move::Flag::QueenPromotionCapture, (uint)square + 7, (uint)square, PieceType::Pawn, capturePT, t });
	}
	for (const auto square : attackSquaresWest)
	{
		auto capturePT = brd.ParseCapture(square);
		moves.push_back({ (uint)_Move::Flag::Capture, (uint)square + 9, (uint)square, PieceType::Pawn, capturePT, t });
	}
	for (const auto square : promotionSquaresWest)
	{
		auto capturePT = brd.ParseCapture(square);
		moves.push_back({ (uint)_Move::Flag::KnightPromotionCapture, (uint)square + 9, (uint)square, PieceType::Pawn, capturePT, t });
		moves.push_back({ (uint)_Move::Flag::QueenPromotionCapture, (uint)square + 9, (uint)square, PieceType::Pawn, capturePT, t });
	}
	return moves;
}

std::vector<_Move> LegalMoveGenerator::GenerateMoves(Team t, const ChessBoard& brd)
{
	if (brd.IsInCheck(t))
	{
		if (brd.IsDoubleCheck(t))
		{
			return GenerateKingMovesCheck(t, brd);
		}

		auto moves = GenerateKingMovesCheck(t, brd);
		auto knightMoves = GenerateKnightMovesCheck(t, brd);
		moves.insert(moves.end(), knightMoves.begin(), knightMoves.end());
		auto slidingMoves = GenerateSlidingMovesCheck(t, brd);
		moves.insert(moves.end(), slidingMoves.begin(), slidingMoves.end());
		auto pawnMoves = GeneratePawnMovesCheck(t, brd);
		moves.insert(moves.end(), pawnMoves.begin(), pawnMoves.end());
		return moves;
	}
	auto moves = GenerateKingMoves(t, brd);
	auto moreMoves = GenerateKnightMoves(t, brd);
	moves.insert(moves.end(), moreMoves.begin(), moreMoves.end());
	moreMoves = GenerateSlidingMoves(t, brd);
	moves.insert(moves.end(), moreMoves.begin(), moreMoves.end());
	moreMoves = GeneratePawnMoves(t, brd);
	moves.insert(moves.end(), moreMoves.begin(), moreMoves.end());
	return moves;
}