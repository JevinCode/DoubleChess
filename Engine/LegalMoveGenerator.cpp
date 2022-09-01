#include "LegalMoveGenerator.h"
#include "PseudoLegalMoveGenerator.h"
std::vector<_Move> LegalMoveGenerator::GenerateKingMoves(Team t, const ChessBoard& brd)
{
	std::vector<_Move> moves;
	auto pieceBBs = brd.GetPieceBBs();
	auto kingPos = pieceBBs[(int)ChessBoard::BBIndex::Kings] & pieceBBs[(int)t];
	auto attackBB = BBTwiddler::KingAttacks(kingPos);
	auto dangers = brd.GetKingDangerSquares(t);
	//for debugging purposes only
	auto dangerSquares = ChessBoard::BitBoardToSquares(dangers);
	//
	auto safes = ChessBoard::Universe ^ dangers;
	auto attackSquares = ChessBoard::BitBoardToSquares(attackBB & pieceBBs[1 - (int)t] & safes);
	auto quietSquares = ChessBoard::BitBoardToSquares(attackBB & pieceBBs[(int)ChessBoard::BBIndex::Empty] & safes);

	auto kingSquare = ChessBoard::BitBoardToSquares(kingPos);
	for (const auto square : attackSquares)
	{
		auto capturePT = brd.ParseCapture(square);
		moves.push_back({ (uint)_Move::Flag::Capture, (uint)kingSquare[0], (uint)square, _Move::PieceType::King, capturePT });
	}
	for (const auto square : quietSquares)
	{
		moves.push_back({ (uint)_Move::Flag::None, (uint)kingSquare[0], (uint)square, _Move::PieceType::King });
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
		auto quiets = attacks & pieceBBs[ChessBoard::BBIndex::Empty];
		auto captures = attacks & pieceBBs[other];

		auto quietSquares = ChessBoard::BitBoardToSquares(quiets);
		auto captureSquares = ChessBoard::BitBoardToSquares(captures);
		for (const auto& sq : quietSquares)
			knightMoves.push_back({ (ushort)_Move::Flag::None, (ushort)square, (ushort)sq, _Move::PieceType::Knight });
		for (const auto& sq : captureSquares)
		{
			auto capturePT = brd.ParseCapture(sq);
			knightMoves.push_back({ (ushort)_Move::Flag::Capture, (ushort)square, (ushort)sq, _Move::PieceType::Knight, capturePT });
		}
	}

	//handle non-pinned knights
	const auto freeKnights = pieceBBs[ChessBoard::BBIndex::Knights] & pieceBBs[t] & (ChessBoard::Universe ^ pins);
	const auto freeSquares = ChessBoard::BitBoardToSquares(freeKnights);
	for (const auto& square : freeSquares)
	{
		auto attacks = brd.KnightAttacks[square];
		auto quiets = attacks & pieceBBs[ChessBoard::BBIndex::Empty];
		auto captures = attacks & pieceBBs[other];

		auto quietSquares = ChessBoard::BitBoardToSquares(quiets);
		auto captureSquares = ChessBoard::BitBoardToSquares(captures);
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
		BitBoard quiets = attacks & pieceBBs[ChessBoard::BBIndex::Empty];
		BitBoard captures = attacks & pieceBBs[other];

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

	//handle non-pinned rooks
	auto freeRooks = rooks ^ pinnedRooks;
	auto freeRookSquares = ChessBoard::BitBoardToSquares(freeRooks);
	for (const auto square : freeRookSquares)
	{
		BitBoard attacks = BBTwiddler::GetRookAttackBBSingle(occupied, (int)square, RayAttacks);
		BitBoard quiets = attacks & pieceBBs[ChessBoard::BBIndex::Empty];
		BitBoard captures = attacks & pieceBBs[other];

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
		BitBoard quiets = attacks & pieceBBs[ChessBoard::BBIndex::Empty];
		BitBoard captures = attacks & pieceBBs[other];

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

	//handle non-pinned bishops
	auto freeBishops = bishops ^ pinnedBishops;
	auto freeBishopSquares = ChessBoard::BitBoardToSquares(freeBishops);
	for (const auto square : freeBishopSquares)
	{
		BitBoard attacks = BBTwiddler::GetBishopAttackBBSingle(occupied, (int)square, RayAttacks);
		BitBoard quiets = attacks & pieceBBs[ChessBoard::BBIndex::Empty];
		BitBoard captures = attacks & pieceBBs[other];

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
		BitBoard quiets = attacks & pieceBBs[ChessBoard::BBIndex::Empty];
		BitBoard captures = attacks & pieceBBs[other];

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

	//handle non-pinned queens
	auto freeQueens = queens ^ pinnedQueens;
	auto freeQueenSquares = ChessBoard::BitBoardToSquares(freeQueens);
	for (const auto square : freeQueenSquares)
	{
		BitBoard attacks = BBTwiddler::GetQueenAttackBBSingle(occupied, (int)square, RayAttacks);
		BitBoard quiets = attacks & pieceBBs[ChessBoard::BBIndex::Empty];
		BitBoard captures = attacks & pieceBBs[other];

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
	auto empty = pieceBBs[ChessBoard::BBIndex::Empty];
	if (t == Team::WHITE)
	{

		BitBoard wPawns = pieceBBs[ChessBoard::BBIndex::Pawns] & pieceBBs[ChessBoard::BBIndex::White];
		auto pinnedPawns = pins & wPawns;
		auto freePawns = wPawns ^ pinnedPawns;
		BitBoard singlePushTargetsPinned = PseudoLegalMoveGenerator::SinglePushTargetsWhite(pinnedPawns, empty);
		BitBoard singlePushTargetsNonPinned = PseudoLegalMoveGenerator::SinglePushTargetsWhite(freePawns, empty);
		BitBoard singlePushPromotionsPinned = singlePushTargetsPinned & ChessBoard::Rank8;
		BitBoard singlePushPromotionsNonPinned = singlePushTargetsNonPinned & ChessBoard::Rank8;
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

		auto singlePushPromotionSquares = ChessBoard::BitBoardToSquares(singlePushPromotionsNonPinned);
		auto singlePushQuietSquares = ChessBoard::BitBoardToSquares(singlePushTargetsNonPinned);
		auto doublePushSquares = ChessBoard::BitBoardToSquares(doublePushNonPinned);
		auto attackSquaresEast = ChessBoard::BitBoardToSquares(wPawnEastAttacksNonPinned);
		auto promotionSquaresEast = ChessBoard::BitBoardToSquares(wPawnEastPromotionsNonPinned);
		auto attackSquaresWest = ChessBoard::BitBoardToSquares(wPawnWestAttacksNonPinned);
		auto promotionSquaresWest = ChessBoard::BitBoardToSquares(wPawnWestPromotionsNonPinned);

		//handle pinned pawns
		auto pinnedPawnSquares = ChessBoard::BitBoardToSquares(pinnedPawns);
		for (const auto square : pinnedPawnSquares)
		{
			auto pinCorridor = corridors[square];
			auto singlePush = singlePushTargetsPinned & pinCorridor;
			if (singlePush)
			{
				ChessBoard::Square singleTarget = ChessBoard::BitBoardToSquare(singlePush);
				moves.push_back({ (uint)_Move::Flag::PawnDoublePush, (uint)square, (uint)singleTarget, _Move::PieceType::Pawn });

				//cant double push if we can't single push
				auto doublePush = doublePushTargets & corridors[square];
				if (doublePush)
				{
					ChessBoard::Square doubleTarget = ChessBoard::BitBoardToSquare(doublePush);
					moves.push_back({ (uint)_Move::Flag::PawnDoublePush, (uint)square, (uint)doubleTarget, _Move::PieceType::Pawn });
				}
			}

			auto eastAttack = wPawnEastAttacksPinned & pinCorridor & pieceBBs[ChessBoard::BBIndex::Black];
			auto westAttack = wPawnWestAttacksPinned & pinCorridor & pieceBBs[ChessBoard::BBIndex::Black];
			if (eastAttack)
			{
				ChessBoard::Square eastTarget = ChessBoard::BitBoardToSquare(eastAttack);
				auto capturePT = brd.ParseCapture(eastTarget);
				moves.push_back({ (uint)_Move::Flag::Capture, (uint)square, (uint)eastTarget, _Move::PieceType::Pawn, capturePT });
			}
			else if (westAttack)
			{
				ChessBoard::Square westTarget = ChessBoard::BitBoardToSquare(westAttack);
				auto capturePT = brd.ParseCapture(westTarget);
				moves.push_back({ (uint)_Move::Flag::Capture, (uint)square, (uint)westTarget, _Move::PieceType::Pawn, capturePT });
			}

			auto eastPromotion = wPawnEastPromotionsPinned & pinCorridor & pieceBBs[ChessBoard::BBIndex::Black];
			auto westPromotion = wPawnWestPromotionsPinned & pinCorridor & pieceBBs[ChessBoard::BBIndex::Black];
			if (eastPromotion)
			{
				ChessBoard::Square eastTarget = ChessBoard::BitBoardToSquare(eastPromotion);
				auto capturePT = brd.ParseCapture(eastTarget);
				moves.push_back({ (uint)_Move::Flag::KnightPromotionCapture, (uint)square, (uint)eastTarget, _Move::PieceType::Pawn, capturePT });
				moves.push_back({ (uint)_Move::Flag::QueenPromotionCapture, (uint)square, (uint)eastTarget, _Move::PieceType::Pawn, capturePT });
			}
			else if (westPromotion)
			{
				ChessBoard::Square westTarget = ChessBoard::BitBoardToSquare(westPromotion);
				auto capturePT = brd.ParseCapture(westTarget);
				moves.push_back({ (uint)_Move::Flag::KnightPromotionCapture, (uint)square, (uint)westTarget, _Move::PieceType::Pawn, capturePT });
				moves.push_back({ (uint)_Move::Flag::QueenPromotionCapture, (uint)square, (uint)westTarget, _Move::PieceType::Pawn, capturePT });
			}
		}

		for (const auto square : singlePushPromotionSquares)
		{
			moves.push_back({ (uint)_Move::Flag::KnightPromotion, (uint)square - 8, (uint)square, _Move::PieceType::Pawn });
			moves.push_back({ (uint)_Move::Flag::QueenPromotion, (uint)square - 8, (uint)square, _Move::PieceType::Pawn });
		}
		for (const auto square : singlePushQuietSquares)
		{
			moves.push_back({ (uint)_Move::Flag::None, (uint)square - 8, (uint)square, _Move::PieceType::Pawn });
		}
		for (const auto square : doublePushSquares)
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


	BitBoard bPawns = pieceBBs[ChessBoard::BBIndex::Pawns] & pieceBBs[ChessBoard::BBIndex::Black];
	auto pinnedPawns = pins & bPawns;
	auto freePawns = bPawns ^ pinnedPawns;
	BitBoard singlePushTargetsPinned = PseudoLegalMoveGenerator::SinglePushTargetsBlack(pinnedPawns, empty);
	BitBoard singlePushTargetsNonPinned = PseudoLegalMoveGenerator::SinglePushTargetsBlack(freePawns, empty);
	BitBoard singlePushPromotionsPinned = singlePushTargetsPinned & ChessBoard::Rank1;
	BitBoard singlePushPromotionsNonPinned = singlePushTargetsNonPinned & ChessBoard::Rank1;
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

	//handle pinned pawns
	auto pinnedPawnSquares = ChessBoard::BitBoardToSquares(pinnedPawns);
	for (const auto square : pinnedPawnSquares)
	{
		auto pinCorridor = corridors[square];
		auto singlePush = singlePushTargetsPinned & pinCorridor;
		if (singlePush)
		{
			ChessBoard::Square singleTarget = ChessBoard::BitBoardToSquare(singlePush);
			moves.push_back({ (uint)_Move::Flag::PawnDoublePush, (uint)square, (uint)singleTarget, _Move::PieceType::Pawn });

			//cant double push if we can't single push
			auto doublePush = doublePushTargets & corridors[square];
			if (doublePush)
			{
				ChessBoard::Square doubleTarget = ChessBoard::BitBoardToSquare(doublePush);
				moves.push_back({ (uint)_Move::Flag::PawnDoublePush, (uint)square, (uint)doubleTarget, _Move::PieceType::Pawn });
			}
		}

		auto eastAttack = bPawnEastAttacksPinned & pinCorridor & pieceBBs[ChessBoard::BBIndex::White];
		auto westAttack = bPawnWestAttacksPinned & pinCorridor & pieceBBs[ChessBoard::BBIndex::White];
		if (eastAttack)
		{
			ChessBoard::Square eastTarget = ChessBoard::BitBoardToSquare(eastAttack);
			auto capturePT = brd.ParseCapture(eastTarget);
			moves.push_back({ (uint)_Move::Flag::Capture, (uint)square, (uint)eastTarget, _Move::PieceType::Pawn, capturePT });
		}
		else if (westAttack)
		{
			ChessBoard::Square westTarget = ChessBoard::BitBoardToSquare(westAttack);
			auto capturePT = brd.ParseCapture(westTarget);
			moves.push_back({ (uint)_Move::Flag::Capture, (uint)square, (uint)westTarget, _Move::PieceType::Pawn, capturePT });
		}

		auto eastPromotion = bPawnEastPromotionsPinned & pinCorridor & pieceBBs[ChessBoard::BBIndex::White];
		auto westPromotion = bPawnWestPromotionsPinned & pinCorridor & pieceBBs[ChessBoard::BBIndex::White];
		if (eastPromotion)
		{
			ChessBoard::Square eastTarget = ChessBoard::BitBoardToSquare(eastPromotion);
			auto capturePT = brd.ParseCapture(eastTarget);
			moves.push_back({ (uint)_Move::Flag::KnightPromotionCapture, (uint)square, (uint)eastTarget, _Move::PieceType::Pawn, capturePT });
			moves.push_back({ (uint)_Move::Flag::QueenPromotionCapture, (uint)square, (uint)eastTarget, _Move::PieceType::Pawn, capturePT });
		}
		else if (westPromotion)
		{
			ChessBoard::Square westTarget = ChessBoard::BitBoardToSquare(westPromotion);
			auto capturePT = brd.ParseCapture(westTarget);
			moves.push_back({ (uint)_Move::Flag::KnightPromotionCapture, (uint)square, (uint)westTarget, _Move::PieceType::Pawn, capturePT });
			moves.push_back({ (uint)_Move::Flag::QueenPromotionCapture, (uint)square, (uint)westTarget, _Move::PieceType::Pawn, capturePT });
		}
	}

	for (const auto square : singlePushPromotionSquares)
	{
		moves.push_back({ (uint)_Move::Flag::KnightPromotion, (uint)square + 8, (uint)square, _Move::PieceType::Pawn });
		moves.push_back({ (uint)_Move::Flag::QueenPromotion, (uint)square + 8, (uint)square, _Move::PieceType::Pawn });
	}
	for (const auto square : singlePushQuietSquares)
	{
		moves.push_back({ (uint)_Move::Flag::None, (uint)square + 8, (uint)square, _Move::PieceType::Pawn });
	}
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

std::vector<_Move> LegalMoveGenerator::GenerateMoves(Team t, const ChessBoard& brd)
{
	auto moves = GenerateKnightMoves(t, brd);
	auto moreMoves = GenerateSlidingMoves(t, brd);
	moves.insert(moves.end(), moreMoves.begin(), moreMoves.end());
	moreMoves = GeneratePawnMoves(t, brd);
	moves.insert(moves.end(), moreMoves.begin(), moreMoves.end());
	moreMoves = GenerateKingMoves(t, brd);
	moves.insert(moves.end(), moreMoves.begin(), moreMoves.end());
	return moves;
}