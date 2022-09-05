#include "ChessBoard.h"
#include "Cell.h"
#include "PseudoLegalMoveGenerator.h"
#include "LegalMoveGenerator.h"

void ChessBoard::GenerateKnightAttackBBs()
{
	for (int count = 0; count < 64; count++)
	{
		BitBoard pos = (BitBoard)1 << count;
		BitBoard result = 0;
		result |= (pos & NotHFile) << 17;
		result |= (pos & NotGHFile) << 10;
		result |= (pos & NotGHFile) >> 6;
		result |= (pos & NotHFile) >> 15;
		result |= (pos & NotAFile) << 15;
		result |= (pos & NotABFile) << 6;
		result |= (pos & NotABFile) >> 10;
		result |= (pos & NotAFile) >> 17;

		KnightAttacks[count] = result;
	}

}

void ChessBoard::GenerateKingAttackBBs()
{
	for (int count = 0; count < 64; count++)
	{
		BitBoard pos = (BitBoard)1 << count;
		KingAttacks[count] = BBTwiddler::KingAttacks(pos);
	}
}

BitBoard ChessBoard::GetKingDangerSquares(Team t) const
{
	return kingDangerSquares[(int)t];
}
BitBoard ChessBoard::GetEmptyBB() const
{
	return pieceBBs[BBIndex::Occupied] ^ Universe;
}
ChessBoard::ChessBoard(const Vei2& topLeft)
	:
	topLeft(topLeft),
	pieceBBs({ 
		Rank1 | Rank2, //White pieces
		Rank7 | Rank8, //Black pieces
		Rank2 | Rank7, //Pawns
		0x8100000000000081, //Rooks
		0x4200000000000042, //Knights
		0x2400000000000024, //Bishops
		0x0800000000000008, //Queens
		0x1000000000000010, //Kings
		0xFFFF00000000FFFF //Occupied
		})
{
	for (int row = 0; row < 8; row++)
	{
		Cell::Shade startShade = row % 2 ? Cell::Shade::LIGHT : Cell::Shade::DARK;
		for (int col = 0; col < 8; col++)
		{
			Vei2 loc = { col,row };
			auto sh = Cell::Shade(((int)startShade + col) % 2);
			auto sq = CoordsToSquare(loc);
			cells[(int)sq] = std::make_unique<Cell>(sh, loc);

		}
	}
	GenerateKnightAttackBBs();
	GenerateKingAttackBBs();
	GenerateMoves(Team::WHITE);
	std::vector<BitBoard> dangers;
	dangers.push_back(0x0000ff0000000000);
	dangers.push_back(0x0000000000ff0000);
	kingDangerSquares = dangers;
	kingDangerStack.push(kingDangerSquares);
}

EnumArray<ChessBoard::BBIndex, BitBoard> ChessBoard::GetPieceBBs() const
{
	return pieceBBs;
}

std::vector<ChessBoard::Square> ChessBoard::BitBoardToSquares(BitBoard bb)
{
	std::vector<Square> squares;
	while (bb)
	{
		int idx = BBTwiddler::bitScanForward(bb);
		squares.push_back((Square)idx);
		bb &= (bb - 1);
	}
	return squares;
}

ChessBoard::Square ChessBoard::BitBoardToSquare(BitBoard bb)
{
	return (Square)BBTwiddler::bitScanForward(bb);
}

Vei2 ChessBoard::SquareToCoords(const Square sq)
{
	int rank = (int)sq / 8;
	int file = (int)sq % 8;
	return { file, rank };
}

std::vector<Vei2> ChessBoard::SquaresToCoords(const std::vector<Square>& squares)
{
	std::vector<Vei2> result;
	for (const Square& square : squares)
	{
		result.push_back(SquareToCoords(square));
	}
	return result;
}

Vei2 ChessBoard::GetScreenCoords(const Square sq) const
{
	Vei2 gridCoords = SquareToCoords(sq);
	auto offset = GetOffset();
	return { gridCoords.x * cellSize + offset.x, (7 - gridCoords.y) * cellSize + offset.y };
}

std::vector<Vei2> ChessBoard::GetScreenCoords(const std::vector<Square>& squares) const
{
	std::vector<Vei2> result;
	for (const auto& square : squares)
	{
		result.push_back(GetScreenCoords(square));
	}
	return result;
}

void ChessBoard::DrawPieces(Graphics& gfx) const
{
	BitBoard blackPawns = pieceBBs[BBIndex::Pawns] & pieceBBs[BBIndex::Black];
	std::vector<Vei2> locs = GetScreenCoords(BitBoardToSquares(blackPawns));
	for (const auto& loc : locs)
		Piece::Draw(gfx, loc, Piece::PieceType::Pawn, Team::BLACK);

	BitBoard whitePawns = pieceBBs[BBIndex::Pawns] & pieceBBs[BBIndex::White];
	locs = GetScreenCoords(BitBoardToSquares(whitePawns));
	for (const auto& loc : locs)
		Piece::Draw(gfx, loc, Piece::PieceType::Pawn, Team::WHITE);

	BitBoard blackRooks = pieceBBs[BBIndex::Rooks] & pieceBBs[BBIndex::Black];
	locs = GetScreenCoords(BitBoardToSquares(blackRooks));
	for (const auto& loc : locs)
		Piece::Draw(gfx, loc, Piece::PieceType::Rook, Team::BLACK);

	BitBoard whiteRooks = pieceBBs[BBIndex::Rooks] & pieceBBs[BBIndex::White];
	locs = GetScreenCoords(BitBoardToSquares(whiteRooks));
	for (const auto& loc : locs)
		Piece::Draw(gfx, loc, Piece::PieceType::Rook, Team::WHITE);

	BitBoard blackKnights = pieceBBs[BBIndex::Knights] & pieceBBs[BBIndex::Black];
	locs = GetScreenCoords(BitBoardToSquares(blackKnights));
	for (const auto& loc : locs)
		Piece::Draw(gfx, loc, Piece::PieceType::Knight, Team::BLACK);

	BitBoard whiteKnights = pieceBBs[BBIndex::Knights] & pieceBBs[BBIndex::White];
	locs = GetScreenCoords(BitBoardToSquares(whiteKnights));
	for (const auto& loc : locs)
		Piece::Draw(gfx, loc, Piece::PieceType::Knight, Team::WHITE);

	BitBoard blackBishops = pieceBBs[BBIndex::Bishops] & pieceBBs[BBIndex::Black];
	locs = GetScreenCoords(BitBoardToSquares(blackBishops));
	for (const auto& loc : locs)
		Piece::Draw(gfx, loc, Piece::PieceType::Bishop, Team::BLACK);

	BitBoard whiteBishops = pieceBBs[BBIndex::Bishops] & pieceBBs[BBIndex::White];
	locs = GetScreenCoords(BitBoardToSquares(whiteBishops));
	for (const auto& loc : locs)
		Piece::Draw(gfx, loc, Piece::PieceType::Bishop, Team::WHITE);

	BitBoard blackQueens = pieceBBs[BBIndex::Queens] & pieceBBs[BBIndex::Black];
	locs = GetScreenCoords(BitBoardToSquares(blackQueens));
	for (const auto& loc : locs)
		Piece::Draw(gfx, loc, Piece::PieceType::Queen, Team::BLACK);

	BitBoard whiteQueens = pieceBBs[BBIndex::Queens] & pieceBBs[BBIndex::White];
	locs = GetScreenCoords(BitBoardToSquares(whiteQueens));
	for (const auto& loc : locs)
		Piece::Draw(gfx, loc, Piece::PieceType::Queen, Team::WHITE);

	BitBoard blackKing = pieceBBs[BBIndex::Kings] & pieceBBs[BBIndex::Black];
	locs = GetScreenCoords(BitBoardToSquares(blackKing));
	for (const auto& loc : locs)
		Piece::Draw(gfx, loc, Piece::PieceType::King, Team::BLACK);

	BitBoard whiteKing = pieceBBs[BBIndex::Kings] & pieceBBs[BBIndex::White];
	locs = GetScreenCoords(BitBoardToSquares(whiteKing));
	for (const auto& loc : locs)
		Piece::Draw(gfx, loc, Piece::PieceType::King, Team::WHITE);
}

void ChessBoard::Draw(Graphics& gfx) const
{
	for (const auto& c : cells)
	{
		auto offSet = GetOffset();
		c->Draw(gfx, offSet);
	}
	DrawPieces(gfx);
}

ChessBoard::Square ChessBoard::CoordsToSquare(const Vei2& coords)
{
	return (Square)(8 * coords.y + coords.x);
}

BitBoard ChessBoard::SquareToBitBoard(const Square sq)
{
	BitBoard position = 0x1;
	//sets a 1 bit for the position encoded by Square using LERF enumeration.
	return position << (int)sq;
}

BitBoard ChessBoard::SquaresToBitBoard(const std::vector<Square> squares)
{
	BitBoard result = 0;
	for (const auto& square : squares)
	{
		result |= SquareToBitBoard(square);
	}
	return result;
}

PieceType ChessBoard::ParseCapture(Square sq) const
{
	auto bb = SquareToBitBoard(sq);
	if (pieceBBs[BBIndex::Pawns] & bb)
		return PieceType::Pawn;
	if (pieceBBs[BBIndex::Rooks] & bb)
		return PieceType::Rook;
	if (pieceBBs[BBIndex::Knights] & bb)
		return PieceType::Knight;
	if (pieceBBs[BBIndex::Bishops] & bb)
		return PieceType::Bishop;
	return PieceType::Queen;
}

void ChessBoard::GenerateMoves(Team t)
{
	userPossibleMoves = LegalMoveGenerator::GenerateMoves(t, *this);
}

Vei2 ChessBoard::GetOffset() const
{
	return topLeft;
}


Cell& ChessBoard::CellAt(const Square sq)
{
	return *cells[(int)sq];
}

void ChessBoard::ClearHighlights()
{
	for (auto& c : cells)
	{
		c->ClearHighlight();
	}
}

bool ChessBoard::IsValidLoc(const Vei2& loc)
{
	if (loc.x >= 0 && loc.x < 8 && loc.y >= 0 && loc.y < 8)
		return true;
	return false;
}

std::vector<_Move> ChessBoard::GetPossibleMoves(const Square square) const
{
	std::vector<_Move> result;
	for (const auto& move : userPossibleMoves)
	{
		Square source = (Square)move.GetSource();
		if (square == source)
			result.push_back(move);
	}
	return result;
}


bool ChessBoard::IsCheckmate() const
{
	return userPossibleMoves.size() == 0 && (IsInCheck(Team::BLACK) || IsInCheck(Team::WHITE));
}
bool ChessBoard::IsInCheck(Team t) const
{
	return pieceBBs[BBIndex::Kings] & pieceBBs[(BBIndex)t] & kingDangerSquares[(int)t];
}
bool ChessBoard::IsDoubleCheck(Team t) const
{
	assert(IsInCheck(t));
	return !BBTwiddler::SingleElement(kingAttackers);
}
bool ChessBoard::IsEnPassantable() const
{
	return isEnPassantable;
}
BitBoard ChessBoard::GetEnPassantSquareBB() const
{
	return enPassantSquareBB;
}
bool ChessBoard::CanCastleKingside(Team t) const
{
	if (t == Team::WHITE)
	{
		if (hasCastledWhite || !canCastleKingsideWhite)
			return false;
		return pieceBBs[BBIndex::Rooks] & 0x80 && !(kingDangerSquares[(int)t] & 0x60) && ((GetEmptyBB() & 0x60) == 0x60);
	}
	if (hasCastledBlack || !canCastleKingsideBlack)
		return false;
	return pieceBBs[BBIndex::Rooks] & 0x8000000000000000 && !(kingDangerSquares[(int)t] & 0x6000000000000000) && ((GetEmptyBB() & 0x6000000000000000) == 0x6000000000000000);
}
bool ChessBoard::CanCastleQueenside(Team t) const
{
	if (t == Team::WHITE)
	{
		if (hasCastledWhite || !canCastleQueensideWhite)
			return false;
		return pieceBBs[BBIndex::Rooks] & 0x1 && !(kingDangerSquares[(int)t] & 0xe) && ((GetEmptyBB() & 0xe) == 0xe);
	}
	if (hasCastledBlack || !canCastleKingsideBlack)
		return false;
	return pieceBBs[BBIndex::Rooks] & 0x0100000000000000 && !(kingDangerSquares[(int)t] & 0x0e00000000000000) && ((GetEmptyBB() & 0x0e00000000000000) == 0x0e00000000000000);
}
void ChessBoard::SetCastleFlag(Team t)
{
	if (t == Team::WHITE)
		hasCastledWhite = true;
	else
		hasCastledBlack = true;
}
void ChessBoard::ApplyMove(_Move m)
{
	auto flag = m.GetFlag();
	PieceType srcPiece = m.GetSourcePiece();
	PieceType captured = m.GetCapturedPiece();
	Square src = (Square)m.GetSource();
	Square dest = (Square)m.GetTarget();
	Team t = m.GetTeam();
	auto srcBB = SquareToBitBoard(src);
	auto destBB = SquareToBitBoard(dest);
	//no matter what kind of move it is, we always clear out the source square. So we will update the bitboards accordingly.
	pieceBBs[BBIndex::Occupied] ^= srcBB;
	pieceBBs[BBIndex::White] &= pieceBBs[BBIndex::Occupied];
	pieceBBs[BBIndex::Black] &= pieceBBs[BBIndex::Occupied];
	pieceBBs[PieceTypeMatcher(srcPiece)] &= pieceBBs[BBIndex::Occupied];

	switch (flag)
	{
	case _Move::Flag::EnPassant:
	{
		if (t == Team::WHITE)
		{
			auto capturedPieceBB = BBTwiddler::SouthOne(destBB);
			pieceBBs[BBIndex::Occupied] |= destBB;
			pieceBBs[BBIndex::Occupied] ^= capturedPieceBB;

			pieceBBs[BBIndex::Pawns] |= destBB;
			pieceBBs[BBIndex::Pawns] ^= capturedPieceBB;

			pieceBBs[BBIndex::White] |= destBB;
			pieceBBs[BBIndex::Black] ^= capturedPieceBB;
		}
		else
		{
			auto capturedPieceBB = BBTwiddler::NorthOne(destBB);
			pieceBBs[BBIndex::Occupied] |= destBB;
			pieceBBs[BBIndex::Occupied] ^= capturedPieceBB;

			pieceBBs[BBIndex::Pawns] |= destBB;
			pieceBBs[BBIndex::Pawns] ^= capturedPieceBB;

			pieceBBs[BBIndex::Black] |= destBB;
			pieceBBs[BBIndex::White] ^= capturedPieceBB;
		}
		isEnPassantable = false;
		break;
	}
	case _Move::Flag::KingsideCastle:
	{
		if (t == Team::WHITE)
		{
			//remove rook on h1 from bitboards
			BitBoard h1 = SquareToBitBoard(Square::h1);
			BitBoard e1 = SquareToBitBoard(Square::e1);
			BitBoard f1 = SquareToBitBoard(Square::f1);
			pieceBBs[BBIndex::Occupied] ^= h1;
			pieceBBs[BBIndex::White] ^= h1;
			pieceBBs[BBIndex::Rooks] ^= h1;


			//add the pieces in new positions
			pieceBBs[BBIndex::Occupied] |= f1 | destBB;
			pieceBBs[BBIndex::White] |= f1 | destBB;
			pieceBBs[BBIndex::Rooks] |= f1;
			pieceBBs[BBIndex::Kings] |= destBB;
			hasCastledWhite = true;
		}
		else
		{
			//remove rook on h8 from bitboards
			BitBoard h8 = SquareToBitBoard(Square::h8);
			BitBoard e8 = SquareToBitBoard(Square::e8);
			BitBoard f8 = SquareToBitBoard(Square::f8);
			pieceBBs[BBIndex::Occupied] ^= h8;
			pieceBBs[BBIndex::Black] ^= h8;
			pieceBBs[BBIndex::Rooks] ^= h8;

			//add the pieces in new positions
			pieceBBs[BBIndex::Occupied] |= f8 | destBB;
			pieceBBs[BBIndex::Black] |= f8 | destBB;
			pieceBBs[BBIndex::Rooks] |= f8;
			pieceBBs[BBIndex::Kings] |= destBB;
			hasCastledBlack = true;
		}
		isEnPassantable = false;
		break;
	}
	case _Move::Flag::QueensideCastle:
	{		
		if (t == Team::WHITE)
		{
			//remove rook on h1 from bitboards
			BitBoard a1 = SquareToBitBoard(Square::a1);
			BitBoard e1 = SquareToBitBoard(Square::e1);
			BitBoard d1 = SquareToBitBoard(Square::d1);
			pieceBBs[BBIndex::Occupied] ^= a1;
			pieceBBs[BBIndex::White] ^= a1;
			pieceBBs[BBIndex::Rooks] ^= a1;

			//add the pieces in new positions
			pieceBBs[BBIndex::Occupied] |= d1 | destBB;
			pieceBBs[BBIndex::White] |= d1 | destBB;
			pieceBBs[BBIndex::Rooks] |= d1;
			pieceBBs[BBIndex::Kings] |= destBB;
			hasCastledWhite = true;
		}
		else
		{
			//remove rook on a8 from bitboards
			BitBoard a8 = SquareToBitBoard(Square::a8);
			BitBoard e8 = SquareToBitBoard(Square::e8);
			BitBoard d8 = SquareToBitBoard(Square::d8);
			pieceBBs[BBIndex::Occupied] ^= a8;
			pieceBBs[BBIndex::Black] ^= a8;
			pieceBBs[BBIndex::Rooks] ^= a8;

			//add the pieces in new positions
			pieceBBs[BBIndex::Occupied] |= d8 | destBB;
			pieceBBs[BBIndex::Black] |= d8 | destBB;
			pieceBBs[BBIndex::Rooks] |= d8;
			pieceBBs[BBIndex::Kings] |= destBB;
			hasCastledBlack = true;
		}
		isEnPassantable = false;
		break; 
	}
	case _Move::Flag::KnightPromotionCapture:
		if (t == Team::WHITE)
		{
			//remove captured piece
			pieceBBs[BBIndex::Black] ^= destBB;

			//add piece as appropriate
			pieceBBs[BBIndex::White] |= destBB;
		}
		else
		{
			//remove captured piece
			pieceBBs[BBIndex::White] ^= destBB;

			//add piece as appropriate
			pieceBBs[BBIndex::Black] |= destBB;
		}
		pieceBBs[PieceTypeMatcher(captured)] ^= destBB;
		pieceBBs[BBIndex::Knights] |= destBB;

		isEnPassantable = false;
		break;
	case _Move::Flag::QueenPromotionCapture:
		if (t == Team::WHITE)
		{
			//remove captured piece
			pieceBBs[BBIndex::Black] ^= destBB;

			//add piece as appropriate
			pieceBBs[BBIndex::White] |= destBB;
		}
		else
		{
			//remove captured piece
			pieceBBs[BBIndex::White] ^= destBB;

			//add piece as appropriate
			pieceBBs[BBIndex::Black] |= destBB;
		}
		pieceBBs[PieceTypeMatcher(captured)] ^= destBB;
		pieceBBs[BBIndex::Queens] |= destBB;
		isEnPassantable = false;
		break;
	case _Move::Flag::KnightPromotion:
		pieceBBs[BBIndex::Occupied] |= destBB;
		pieceBBs[BBIndex::Knights] |= destBB;
		pieceBBs[BBIndex::Pawns] &= pieceBBs[BBIndex::Occupied];
		if (t == Team::WHITE)
			pieceBBs[BBIndex::White] |= destBB;
		else
			pieceBBs[BBIndex::Black] |= destBB;

		isEnPassantable = false;
		break;
	case _Move::Flag::QueenPromotion:
		pieceBBs[BBIndex::Occupied] |= destBB;
		pieceBBs[BBIndex::Queens] |= destBB;
		pieceBBs[BBIndex::Pawns] &= pieceBBs[BBIndex::Occupied];
		if (t == Team::WHITE)
			pieceBBs[BBIndex::White] |= destBB;
		else
			pieceBBs[BBIndex::Black] |= destBB;

		isEnPassantable = false;
		break;
	case _Move::Flag::Capture:
		if (t == Team::WHITE)
		{
			//remove captured piece
			pieceBBs[BBIndex::Black] ^= destBB;

			//add piece as appropriate
			pieceBBs[BBIndex::White] |= destBB;
		}
		else
		{
			//remove captured piece
			pieceBBs[BBIndex::White] ^= destBB;

			//add piece as appropriate
			pieceBBs[BBIndex::Black] |= destBB;
		}
		pieceBBs[PieceTypeMatcher(captured)] ^= destBB;
		pieceBBs[PieceTypeMatcher(srcPiece)] |= destBB;

		isEnPassantable = false;
		break;
	case _Move::Flag::PawnDoublePush:
		isEnPassantable = true;
		//TODO - figure out enpassant square
		pieceBBs[BBIndex::Occupied] |= destBB;
		pieceBBs[BBIndex::Pawns] |= destBB;
		enPassantAttackers = (BBTwiddler::EastOne(destBB) | BBTwiddler::WestOne(destBB)) & pieceBBs[BBIndex::Pawns];
		if (t == Team::WHITE)
		{
			pieceBBs[BBIndex::White] |= destBB;
			enPassantAttackers &= pieceBBs[BBIndex::Black];
			if (enPassantAttackers)
			{
				isEnPassantable = true;
				enPassantSquareBB = BBTwiddler::SouthOne(destBB);
			}
		}
		else
		{
			pieceBBs[BBIndex::Black] |= destBB;
			enPassantAttackers &= pieceBBs[BBIndex::White];
			if (enPassantAttackers)
			{
				isEnPassantable = true;
				enPassantSquareBB = BBTwiddler::NorthOne(destBB);
			}
		}
		break;
	case _Move::Flag::None:
		pieceBBs[BBIndex::Occupied] |= destBB;
		pieceBBs[PieceTypeMatcher(srcPiece)] |= destBB;
		pieceBBs[PieceTypeMatcher(srcPiece)] &= pieceBBs[BBIndex::Occupied];
		if (t == Team::WHITE)
			pieceBBs[BBIndex::White] |= destBB;
		else
			pieceBBs[BBIndex::Black] |= destBB;

		isEnPassantable = false;
		break;
	}
	plies.push(m);

	if (srcPiece == PieceType::King)
	{
		SetCastleFlag(t);
	}

	//check if we move a (rook) out of it's starting place
	else if (srcPiece == PieceType::Rook)
	{
		switch (src)
		{
		case Square::a1:
			if (canCastleQueensideWhite)
			{
				canCastleQueensideWhite = false;
				queensideWhitePlies = plies.size();
			}
			break;
		case Square::h1:
			if (canCastleKingsideWhite)
			{
				canCastleKingsideWhite = false;
				kingsideWhitePlies = plies.size();
			}
			break;
		case Square::a8:
			if (canCastleQueensideBlack)
			{
				canCastleQueensideBlack = false;
				queensideBlackPlies = plies.size();
			}
			break;
		case Square::h8:
			if (canCastleKingsideBlack)
			{
				canCastleKingsideBlack = false;
				kingsideBlackPlies = plies.size();
			}
			break;
		}
	}

	//finally, need to update pins and other team's king danger squares
	Team other = (Team)(1 - (int)t);
	kingDangerSquares[(int)other] = CalculateKingDangerSquares(other);
	kingDangerSquares[(int)t] = CalculateKingDangerSquares(t);
	kingDangerStack.push(kingDangerSquares);
	pins[(int)other] = LegalMoveGenerator::CalculatePins(*this, other);
	pins[(int)t] = LegalMoveGenerator::CalculatePins(*this, t);
	if (IsInCheck(other))
	{
		kingAttackers = GetKingAttackers(other);
		checkCorridor = GetCheckCorridor(other, dest, srcPiece);
	}
	turnSwap = true;
}
void ChessBoard::RevertMove()
{
	_Move m = plies.top();
	plies.pop();
	Team t = m.GetTeam();
	Team other = (Team)(1 - (int)t);

		auto flag = m.GetFlag();
		PieceType srcPiece = m.GetSourcePiece();
		PieceType captured = m.GetCapturedPiece();
		Square src = (Square)m.GetSource();
		Square dest = (Square)m.GetTarget();
		auto srcBB = SquareToBitBoard(src);
		auto destBB = SquareToBitBoard(dest);
		//no matter what kind of move it is, we always clear out the source square. So we will update the bitboards accordingly.
		pieceBBs[BBIndex::Occupied] |= srcBB;
		pieceBBs[t] |= srcBB;
		pieceBBs[t] ^= destBB;
		pieceBBs[PieceTypeMatcher(srcPiece)] |= srcBB;

		switch (flag)
		{
		case _Move::Flag::EnPassant:
			pieceBBs[BBIndex::Occupied] ^= destBB;
			pieceBBs[BBIndex::Pawns] ^= destBB;
			if (t == Team::WHITE)
			{
				auto capturedBB = BBTwiddler::SouthOne(destBB);
				pieceBBs[BBIndex::Pawns] |= capturedBB;
				pieceBBs[BBIndex::Black] |= capturedBB;
				pieceBBs[BBIndex::Occupied] |= capturedBB;
			}
			else
			{
				auto capturedBB = BBTwiddler::NorthOne(destBB);
				pieceBBs[BBIndex::Pawns] |= capturedBB;
				pieceBBs[BBIndex::White] |= capturedBB;
				pieceBBs[BBIndex::Occupied] |= capturedBB;
			}
			isEnPassantable = true;
			break;
		case _Move::Flag::KingsideCastle:
		{
			pieceBBs[BBIndex::Kings] ^= destBB;
			if (t == Team::WHITE)
			{
				//remove rook on h1 from bitboards
				BitBoard h1 = SquareToBitBoard(Square::h1);
				BitBoard e1 = SquareToBitBoard(Square::e1);
				BitBoard f1 = SquareToBitBoard(Square::f1);
				pieceBBs[BBIndex::Occupied] ^= (f1 | destBB);
				pieceBBs[BBIndex::White] ^= f1;
				pieceBBs[BBIndex::Rooks] ^= f1;


				//add the pieces in new positions
				pieceBBs[BBIndex::Occupied] |= h1;
				pieceBBs[BBIndex::White] |= h1;
				pieceBBs[BBIndex::Rooks] |= h1;
				hasCastledWhite = false;
			}
			else
			{
				//remove rook on h8 from bitboards
				BitBoard h8 = SquareToBitBoard(Square::h8);
				BitBoard e8 = SquareToBitBoard(Square::e8);
				BitBoard f8 = SquareToBitBoard(Square::f8);
				pieceBBs[BBIndex::Occupied] ^= (f8 | destBB);
				pieceBBs[BBIndex::Black] ^= f8;
				pieceBBs[BBIndex::Rooks] ^= f8;

				//add the pieces in new positions
				pieceBBs[BBIndex::Occupied] |= h8;
				pieceBBs[BBIndex::Black] |= h8;
				pieceBBs[BBIndex::Rooks] |= h8;
				hasCastledBlack = false;
			}
			isEnPassantable = false;
			break;
		}
		case _Move::Flag::QueensideCastle:
		{
			pieceBBs[BBIndex::Kings] ^= destBB;
			if (t == Team::WHITE)
			{
				//remove rook on h1 from bitboards
				BitBoard a1 = SquareToBitBoard(Square::a1);
				BitBoard e1 = SquareToBitBoard(Square::e1);
				BitBoard d1 = SquareToBitBoard(Square::d1);
				pieceBBs[BBIndex::Occupied] ^= (d1 | destBB);
				pieceBBs[BBIndex::White] ^= d1;
				pieceBBs[BBIndex::Rooks] ^= d1;


				//add the pieces in new positions
				pieceBBs[BBIndex::Occupied] |= a1;
				pieceBBs[BBIndex::White] |= a1;
				pieceBBs[BBIndex::Rooks] |= a1;
				hasCastledWhite = false;
			}
			else
			{
				//remove rook on h8 from bitboards
				BitBoard a8 = SquareToBitBoard(Square::a8);
				BitBoard e8 = SquareToBitBoard(Square::e8);
				BitBoard d8 = SquareToBitBoard(Square::d8);
				pieceBBs[BBIndex::Occupied] ^= (d8 | destBB);
				pieceBBs[BBIndex::Black] ^= d8;
				pieceBBs[BBIndex::Rooks] ^= d8;

				//add the pieces in new positions
				pieceBBs[BBIndex::Occupied] |= a8;
				pieceBBs[BBIndex::Black] |= a8;
				pieceBBs[BBIndex::Rooks] |= a8;
				hasCastledBlack = false;
			}
			isEnPassantable = false;
			break;
		}
		case _Move::Flag::KnightPromotionCapture:
			pieceBBs[PieceTypeMatcher(captured)] |= destBB;
			pieceBBs[other] |= destBB;
			pieceBBs[BBIndex::Occupied] |= destBB;
			pieceBBs[BBIndex::Knights] ^= destBB;
			isEnPassantable = false;
			break;
		case _Move::Flag::QueenPromotionCapture:

			pieceBBs[PieceTypeMatcher(captured)] |= destBB;
			pieceBBs[other] |= destBB;
			pieceBBs[BBIndex::Occupied] |= destBB;
			pieceBBs[BBIndex::Queens] ^= destBB;
			isEnPassantable = false;
			break;
		case _Move::Flag::KnightPromotion:
			pieceBBs[BBIndex::Knights] ^= destBB;
			isEnPassantable = false;
			break;
		case _Move::Flag::QueenPromotion:
			pieceBBs[BBIndex::Queens] ^= destBB;
			isEnPassantable = false;
			break;
		case _Move::Flag::Capture:

			pieceBBs[PieceTypeMatcher(srcPiece)] ^= destBB;
			pieceBBs[PieceTypeMatcher(captured)] |= destBB;
			pieceBBs[other] |= destBB;
			pieceBBs[BBIndex::Occupied] |= destBB;
			isEnPassantable = false;
			break;
		case _Move::Flag::PawnDoublePush:
			isEnPassantable = false;
			pieceBBs[BBIndex::Pawns] ^= destBB;
			pieceBBs[BBIndex::Occupied] ^= destBB;
			isEnPassantable = false;
			break;
		case _Move::Flag::None:
			pieceBBs[PieceTypeMatcher(srcPiece)] ^= destBB;
			pieceBBs[BBIndex::Occupied] ^= destBB;
			isEnPassantable = false;
			break;
		}


	if (srcPiece == PieceType::Rook)
	{
		switch (src)
		{
		case Square::a1:
			if (plies.size() < queensideWhitePlies)
			{
				canCastleQueensideWhite = true;
			}
			break;
		case Square::h1:
			if (plies.size() < kingsideWhitePlies)
			{
				canCastleKingsideWhite = true;
			}
			break;
		case Square::a8:
			if (plies.size() < queensideBlackPlies)
			{
				canCastleQueensideBlack = true;
			}
			break;
		case Square::h8:
			if (plies.size() < kingsideBlackPlies)
			{
				canCastleKingsideBlack = true;
			}
			break;
		}
	}

		//need to save a history of these in a stack along with the plies in the future
		//will save computation time when the AI is reverting moves
	kingDangerStack.pop();
	kingDangerSquares = kingDangerStack.top();
	pins[(int)other] = LegalMoveGenerator::CalculatePins(*this, other);
	pins[(int)t] = LegalMoveGenerator::CalculatePins(*this, t);
	if (IsInCheck(other))
	{
		kingAttackers = GetKingAttackers(other);
		checkCorridor = GetCheckCorridor(other, dest, srcPiece);
	}
	turnSwap = true;
}
BitBoard ChessBoard::GetCheckCorridor() const
{
	return checkCorridor;
}
BitBoard ChessBoard::GetEnPassantAttackers() const
{
	return enPassantAttackers;
}
ChessBoard::BBIndex ChessBoard::PieceTypeMatcher(PieceType p) const
{
	switch (p)
	{
	case PieceType::Pawn:
		return BBIndex::Pawns;
	case PieceType::Rook:
		return BBIndex::Rooks;
	case PieceType::Knight:
		return BBIndex::Knights;
	case PieceType::Bishop:
		return BBIndex::Bishops;
	case PieceType::Queen:
		return BBIndex::Queens;
	case PieceType::King:
		return BBIndex::Kings;
	default:
		return BBIndex::Kings;
	}
}

BitBoard ChessBoard::GetKnightAttackBB(Team t)
{
	auto knights = pieceBBs[BBIndex::Knights] & pieceBBs[t];
	return BBTwiddler::NeeOne(knights) | BBTwiddler::NneOne(knights) | BBTwiddler::NwwOne(knights) | BBTwiddler::NnwOne(knights) | 
		   BBTwiddler::SeeOne(knights) | BBTwiddler::SseOne(knights) | BBTwiddler::SswOne(knights) | BBTwiddler::SwwOne(knights);
}
BitBoard ChessBoard::CalculateKingDangerSquares(Team t)
{
	auto occupied = pieceBBs[(int)BBIndex::Occupied]; 
	auto otherTeam = pieceBBs[1 - (int)t];
	occupied ^= pieceBBs[(int)BBIndex::Kings] & pieceBBs[(int)t];
	auto rooks = pieceBBs[(int)BBIndex::Rooks] & otherTeam;
	auto bishops = pieceBBs[(int)BBIndex::Bishops] & otherTeam;
	auto queens = pieceBBs[(int)BBIndex::Queens] & otherTeam;
	auto knights = pieceBBs[(int)BBIndex::Knights] & otherTeam;
	auto pawns = pieceBBs[(int)BBIndex::Pawns] & otherTeam;

	auto otherKing = pieceBBs[(int)BBIndex::Kings] & pieceBBs[1 - (int)t];
	BitBoard dangers = LegalMoveGenerator::GetRookAttackBB(occupied, rooks);
	dangers |= LegalMoveGenerator::GetBishopAttackBB(occupied, bishops);
	dangers |= LegalMoveGenerator::GetQueenAttackBB(occupied, queens);
	dangers |= GetKnightAttackBB(Team(1 - (int)t));
	if (t == Team::WHITE)
		dangers |= BBTwiddler::BlackPawnAttacks(pawns);
	else
		dangers |= BBTwiddler::WhitePawnAttacks(pawns);
	dangers |= BBTwiddler::KingAttacks(otherKing);

	return dangers;

}

void ChessBoard::SetPinCorridor(Square sq, BitBoard bb)
{
	pinCorridors[sq] = bb;
}

BitBoard ChessBoard::GetKingAttackers(Team t)
{
	assert(IsInCheck(t));
	BitBoard kingBB = pieceBBs[BBIndex::Kings] & pieceBBs[t];
	BitBoard occupied = pieceBBs[BBIndex::Occupied];
	Square kingLoc = BitBoardToSquare(kingBB);

	Team other = (Team)(1 - (int)t);
	BitBoard rq = (pieceBBs[BBIndex::Rooks] | pieceBBs[BBIndex::Queens]) & pieceBBs[other];
	BitBoard bq = (pieceBBs[BBIndex::Bishops] | pieceBBs[BBIndex::Queens]) & pieceBBs[other];
	BitBoard attackers = KnightAttacks[kingLoc] & pieceBBs[BBIndex::Knights] & pieceBBs[other];

	if (t == Team::WHITE)
	{
		BitBoard pawnAttackerBB = BBTwiddler::NortheastOne(kingBB) | BBTwiddler::NorthwestOne(kingBB);
		attackers |= pawnAttackerBB & pieceBBs[BBIndex::Pawns] & pieceBBs[BBIndex::Black];
	}
	else
	{
		BitBoard pawnAttackerBB = BBTwiddler::SoutheastOne(kingBB) | BBTwiddler::SouthwestOne(kingBB);
		attackers |= pawnAttackerBB & pieceBBs[BBIndex::Pawns] & pieceBBs[BBIndex::White];
	}

	attackers |= LegalMoveGenerator::GetRookAttackBBSingle(occupied, kingLoc) & rq;
	attackers |= LegalMoveGenerator::GetBishopAttackBBSingle(occupied, kingLoc) & bq;
	return attackers;
}
BitBoard ChessBoard::GetCheckCorridor(Team t, Square sq, PieceType p)
{
	BitBoard corridor = SquareToBitBoard(sq);
	if (p == PieceType::Knight || p == PieceType::Pawn)
		return corridor;
	auto kingLoc = BitBoardToSquare(pieceBBs[BBIndex::Kings] & pieceBBs[t]);
	BitBoard occupied = pieceBBs[BBIndex::Occupied];
	Vei2 kingCoords = SquareToCoords(kingLoc);
	Vei2 pieceCoords = SquareToCoords(sq);
	Vei2 delta = kingCoords - pieceCoords;
	if (delta.x < 0 && delta.y < 0)
		return LegalMoveGenerator::GetPositiveRayAttack(occupied, Direction::Northeast, kingLoc);
	if (delta.x < 0 && delta.y > 0)
		return LegalMoveGenerator::GetNegativeRayAttack(occupied, Direction::Southeast, kingLoc);
	if (delta.x < 0)
		return LegalMoveGenerator::GetPositiveRayAttack(occupied, Direction::East, kingLoc);
	if(delta.x > 0 && delta.y < 0)
		return LegalMoveGenerator::GetPositiveRayAttack(occupied, Direction::Northwest, kingLoc);
	if(delta.x > 0 && delta.y > 0)
		return LegalMoveGenerator::GetNegativeRayAttack(occupied, Direction::Southwest, kingLoc);
	if(delta.x > 0)
		return LegalMoveGenerator::GetNegativeRayAttack(occupied, Direction::West, kingLoc);
	if(delta.y < 0)
		return LegalMoveGenerator::GetPositiveRayAttack(occupied, Direction::North, kingLoc);

	return LegalMoveGenerator::GetNegativeRayAttack(occupied, Direction::South, kingLoc);
}
const std::unordered_map<ChessBoard::Square, BitBoard>& ChessBoard::GetCorridors() const
{
	return pinCorridors;
}
BitBoard ChessBoard::GetPins(Team t) const
{
	return pins[(int)t];
}
void ChessBoard::OnClick(const Vei2& loc, Team t)
{
	Square sq = CoordsToSquare(loc);
	auto c = CellAt(sq);
	if (c.GetHighlight() == Cell::HighlightType::YELLOW || c.GetHighlight() == Cell::HighlightType::RED)
	{
		HandleMoveClick(sq, t);
		return;
	}

	HandleSelectionClick(loc, t);
}

void ChessBoard::HandlePromotionClick(Team t, MoveType type)
{
	if (type == MoveType::QueenPromotion)
	{
		if (CellAt(squarePreviouslyHighlighted).GetHighlight() == Cell::HighlightType::RED)
		{
			auto moveMade = *std::find_if(userPossibleMoves.begin(), userPossibleMoves.end(), [this](_Move m) {return m.GetFlag() == _Move::Flag::QueenPromotionCapture && (Square)m.GetTarget() == squarePreviouslyHighlighted; });
			ApplyMove(moveMade);
		}
		else
		{
			auto moveMade = *std::find_if(userPossibleMoves.begin(), userPossibleMoves.end(), [this](_Move m) {return m.GetFlag() == _Move::Flag::QueenPromotion && (Square)m.GetTarget() == squarePreviouslyHighlighted; });
			ApplyMove(moveMade);
		}
		isPromoting = false;
	}

	else if (type == MoveType::KnightPromotion)
	{
		if (CellAt(squarePreviouslyHighlighted).GetHighlight() == Cell::HighlightType::RED)
		{
			auto moveMade = *std::find_if(userPossibleMoves.begin(), userPossibleMoves.end(), [this](_Move m) {return m.GetFlag() == _Move::Flag::KnightPromotionCapture && (Square)m.GetTarget() == squarePreviouslyHighlighted; });
			ApplyMove(moveMade);
		}
		else
		{
			auto moveMade = *std::find_if(userPossibleMoves.begin(), userPossibleMoves.end(), [this](_Move m) {return m.GetFlag() == _Move::Flag::KnightPromotion && (Square)m.GetTarget() == squarePreviouslyHighlighted; });
			ApplyMove(moveMade);
		}
		isPromoting = false;
	}
	ClearHighlights();

}

void ChessBoard::HandleMoveClick(const Square sq, Team t)
{
	_Move selectedMove;
	for (const auto& move : userPossibleMoves)
	{
		if ((Square)move.GetTarget() == sq && (Square)move.GetSource() == squarePreviouslyHighlighted)
		{
			selectedMove = move;
			break;
		}
	}
	auto flag = selectedMove.GetFlag();
	if (flag == _Move::Flag::KnightPromotion || flag == _Move::Flag::KnightPromotionCapture || flag == _Move::Flag::QueenPromotion || flag == _Move::Flag::QueenPromotionCapture)
	{
		squarePreviouslyHighlighted = sq;
		isPromoting = true;
	}

	if (!isPromoting)
	{
		ApplyMove(selectedMove);
		ClearHighlights();
	}
}

void ChessBoard::HandleSelectionClick(const Vei2& loc, Team t)
{
	Square sq = CoordsToSquare(loc);
	BitBoard pos = SquareToBitBoard(sq);

	auto c = CellAt(sq);
	ClearHighlights();
	if(pieceBBs[t] & pos )
	{
		c.Highlight(Cell::HighlightType::BLUE);
		squarePreviouslyHighlighted = sq;

		auto moves = GetPossibleMoves(sq);
		for (const auto& move : moves)
		{
			auto flag = move.GetFlag();
			auto target = move.GetTarget();
			if (flag == _Move::Flag::Capture || flag == _Move::Flag::EnPassant || flag == _Move::Flag::KnightPromotionCapture || flag == _Move::Flag::QueenPromotionCapture)
				CellAt((Square)target).Highlight(Cell::HighlightType::RED);
			else
				CellAt((Square)target).Highlight(Cell::HighlightType::YELLOW);
		}
	}
}

bool ChessBoard::IsStalemate() const
{
	if (userPossibleMoves.size() == 0)
		return true;
	int numPieces = BBTwiddler::PopCount(pieceBBs[BBIndex::Occupied]);
	switch (numPieces)
	{
	case 2:
		return true;
	case 3:
		if (pieceBBs[BBIndex::Bishops] || pieceBBs[BBIndex::Knights])
			return true;
		return false;
	case 4:
		if (BBTwiddler::PopCount(pieceBBs[BBIndex::Bishops]) == 2 && (!(pieceBBs[BBIndex::Bishops] & LightSquares) || !(pieceBBs[BBIndex::Bishops] & DarkSquares)))
			return true;
		return false;
	default:
		return false;
	}
}