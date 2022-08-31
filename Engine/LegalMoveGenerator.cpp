#include "LegalMoveGenerator.h"

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