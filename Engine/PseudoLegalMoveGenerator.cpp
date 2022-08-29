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
				GetPositiveRayAttack(brd.occupied, Direction::Southeast, square, brd) |
				GetNegativeRayAttack(brd.occupied, Direction::Southwest, square, brd) |
				GetNegativeRayAttack(brd.occupied, Direction::Northwest, square, brd);
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
				GetPositiveRayAttack(brd.occupied, Direction::Southeast, square, brd) |
				GetNegativeRayAttack(brd.occupied, Direction::Southwest, square, brd) |
				GetNegativeRayAttack(brd.occupied, Direction::Northwest, square, brd);
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
			GetPositiveRayAttack(brd.occupied, Direction::Southeast, square, brd) |
			GetNegativeRayAttack(brd.occupied, Direction::Southwest, square, brd) |
			GetNegativeRayAttack(brd.occupied, Direction::Northwest, square, brd);
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
			GetPositiveRayAttack(brd.occupied, Direction::Southeast, square, brd) |
			GetNegativeRayAttack(brd.occupied, Direction::Southwest, square, brd) |
			GetNegativeRayAttack(brd.occupied, Direction::Northwest, square, brd);
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
	return moves;
}