#include "ChessBoard.h"
#include "Cell.h"
#include "PseudoLegalMoveGenerator.h"
#include "LegalMoveGenerator.h"

void ChessBoard::GenerateRayAttackBBs()
{
	int squaresToEdge[64][8];
	//numbers correspond to N,E,S,W,NE,SE,SW,NW respectively
	int directionOffsets[8] = { 8,1,-8,-1,9,-7,-9,7 };
	for (int square = 0; square < 64; square++)
	{
		auto coords = SquareToCoords((Square)square);
		squaresToEdge[square][0] = 7 - coords.y;
		squaresToEdge[square][1] = 7 - coords.x;
		squaresToEdge[square][2] = coords.y;
		squaresToEdge[square][3] = coords.x;
		squaresToEdge[square][4] = std::min(squaresToEdge[square][0], squaresToEdge[square][1]);
		squaresToEdge[square][5] = std::min(squaresToEdge[square][2], squaresToEdge[square][1]);
		squaresToEdge[square][6] = std::min(squaresToEdge[square][2], squaresToEdge[square][3]);
		squaresToEdge[square][7] = std::min(squaresToEdge[square][0], squaresToEdge[square][3]);

		std::vector<BitBoard> attacks;
		for (int dir = 0; dir < 8; dir++)
		{
			int offset = directionOffsets[dir];
			BitBoard ray = 0;
			for (int i = 0; i < squaresToEdge[square][dir]; i++)
			{
				int curRaySquare = square + (i + 1) * offset;
				ray |= SquareToBitBoard((Square)curRaySquare);
			}
			attacks.push_back(ray);
		}
		RayAttacks.push_back(attacks);
	}
}

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

const std::vector<std::vector<BitBoard>> ChessBoard::GetRayAttacks() const
{
	return RayAttacks;
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
			cells[LinearizeCoords(loc)] = std::make_shared<Cell>(sh, loc);

			Square sq = CoordsToSquare({ col,row });
		}
	}
	GenerateRayAttackBBs();
	GenerateKnightAttackBBs();
	GenerateKingAttackBBs();
	GenerateMoves(Team::WHITE);
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
		bb ^= (BitBoard)0x1 << idx;
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

int ChessBoard::LinearizeCoords(const Vei2& loc)
{
	return loc.x + 8 * loc.y;
}

ChessBoard::Square ChessBoard::CoordsToSquare(const Vei2& coords)
{
	return (Square)(8 * coords.y + coords.x);
}

BitBoard ChessBoard::SquareToBitBoard(const Square sq)
{
	BitBoard position = 0x0000000000000001;
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

std::shared_ptr<Cell> ChessBoard::CellAt(const Vei2& loc) const
{
	return cells[LinearizeCoords(loc)];
}

std::shared_ptr<Cell> ChessBoard::CellAt(const Square sq) const
{
	return cells[(int)sq];
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
	return userPossibleMoves.size() == 0;
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
Team ChessBoard::GetPassantTeam() const
{
	return passantTeam;
}
bool ChessBoard::IsEnPassantable() const
{
	return isEnPassantable;
}
Vei2 ChessBoard::GetEnPassantSquare() const
{
	return enPassantSquare;
}
Vei2 ChessBoard::GetEnPassantPawnLoc() const
{
	return enPassantPawnLoc;
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
void ChessBoard::ApplyMove(_Move m, Team t)
{
	auto flag = m.GetFlag();
	PieceType srcPiece = m.GetSourcePiece();
	PieceType captured = m.GetCapturedPiece();
	Square src = (Square)m.GetSource();
	Square dest = (Square)m.GetTarget();
	auto srcBB = SquareToBitBoard(src);
	auto destBB = SquareToBitBoard(dest);
	//no matter what kind of move it is, we always clear out the source square. So we will update the bitboards accordingly.
	pieceBBs[BBIndex::Occupied] ^= srcBB;
	pieceBBs[BBIndex::White] &= pieceBBs[BBIndex::Occupied];
	pieceBBs[BBIndex::Black] &= pieceBBs[BBIndex::Occupied];
	pieceBBs[PieceTypeMatcher(srcPiece)] &= pieceBBs[BBIndex::Occupied];

	switch (flag)
	{
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
		break;
	case _Move::Flag::KnightPromotion:
		pieceBBs[BBIndex::Occupied] |= destBB;
		pieceBBs[BBIndex::Knights] |= destBB;
		pieceBBs[BBIndex::Pawns] &= pieceBBs[BBIndex::Occupied];
		if (t == Team::WHITE)
			pieceBBs[BBIndex::White] |= destBB;
		else
			pieceBBs[BBIndex::Black] |= destBB;
		break;
	case _Move::Flag::QueenPromotion:
		pieceBBs[BBIndex::Occupied] |= destBB;
		pieceBBs[BBIndex::Queens] |= destBB;
		pieceBBs[BBIndex::Pawns] &= pieceBBs[BBIndex::Occupied];
		if (t == Team::WHITE)
			pieceBBs[BBIndex::White] |= destBB;
		else
			pieceBBs[BBIndex::Black] |= destBB;
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
		break;
	case _Move::Flag::PawnDoublePush:
		isEnPassantable = true;
		//TODO - figure out enpassant square
		pieceBBs[BBIndex::Occupied] |= destBB;
		pieceBBs[BBIndex::Pawns] |= destBB;
		if (t == Team::WHITE)
			pieceBBs[BBIndex::White] |= destBB;
		else
			pieceBBs[BBIndex::Black] |= destBB;
		break;
	case _Move::Flag::None:
		pieceBBs[BBIndex::Occupied] |= destBB;
		pieceBBs[PieceTypeMatcher(srcPiece)] |= destBB;
		pieceBBs[PieceTypeMatcher(srcPiece)] &= pieceBBs[BBIndex::Occupied];
		if (t == Team::WHITE)
			pieceBBs[BBIndex::White] |= destBB;
		else
			pieceBBs[BBIndex::Black] |= destBB;
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
	pins = CalculatePins(other);
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
			break;
		}
		case _Move::Flag::KnightPromotionCapture:
			pieceBBs[PieceTypeMatcher(captured)] |= destBB;
			pieceBBs[other] |= destBB;
			pieceBBs[BBIndex::Occupied] |= destBB;
			pieceBBs[BBIndex::Knights] ^= destBB;
			break;
		case _Move::Flag::QueenPromotionCapture:

			pieceBBs[PieceTypeMatcher(captured)] |= destBB;
			pieceBBs[other] |= destBB;
			pieceBBs[BBIndex::Occupied] |= destBB;
			pieceBBs[BBIndex::Queens] ^= destBB;
			break;
		case _Move::Flag::KnightPromotion:
			pieceBBs[BBIndex::Knights] ^= destBB;
			break;
		case _Move::Flag::QueenPromotion:
			pieceBBs[BBIndex::Queens] ^= destBB;
			break;
		case _Move::Flag::Capture:

			pieceBBs[PieceTypeMatcher(srcPiece)] ^= destBB;
			pieceBBs[PieceTypeMatcher(captured)] |= destBB;
			pieceBBs[other] |= destBB;
			pieceBBs[BBIndex::Occupied] |= destBB;
			break;
		case _Move::Flag::PawnDoublePush:
			isEnPassantable = false;
			pieceBBs[BBIndex::Pawns] ^= destBB;
			pieceBBs[BBIndex::Occupied] ^= destBB;
			break;
		case _Move::Flag::None:
			pieceBBs[PieceTypeMatcher(srcPiece)] ^= destBB;
			pieceBBs[BBIndex::Occupied] ^= destBB;
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
	kingDangerSquares = kingDangerStack.top();
	kingDangerStack.pop();
		pins = CalculatePins(other);
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
	auto knightSquares = BitBoardToSquares(knights);
	BitBoard result = Empty;
	for (const auto sq : knightSquares)
	{
		result |= KnightAttacks[sq];
	}
	return result;
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
	BitBoard dangers = BBTwiddler::GetRookAttackBB(occupied, rooks, RayAttacks);
	dangers |= BBTwiddler::GetBishopAttackBB(occupied, bishops, RayAttacks);
	dangers |= BBTwiddler::GetQueenAttackBB(occupied, queens, RayAttacks);
	dangers |= GetKnightAttackBB(Team(1 - (int)t));
	if (t == Team::WHITE)
		dangers |= BBTwiddler::BlackPawnAttacks(pawns);
	else
		dangers |= BBTwiddler::WhitePawnAttacks(pawns);
	dangers |= BBTwiddler::KingAttacks(otherKing);

	return dangers;

}

BitBoard ChessBoard::CalculatePins(Team t)
{
	BitBoard pin = 0;
	Team other = (Team)(1 - (int)t);
	BitBoard friendlies = pieceBBs[t];
	BitBoard rq = (pieceBBs[BBIndex::Rooks] | pieceBBs[BBIndex::Queens]) & pieceBBs[other];
	BitBoard bq = (pieceBBs[BBIndex::Bishops] | pieceBBs[BBIndex::Queens]) & pieceBBs[other];
	BitBoard occ = pieceBBs[BBIndex::Occupied];
	BitBoard opponentPieces = pieceBBs[other];
	Square kingLoc = BitBoardToSquare(pieceBBs[BBIndex::Kings] & pieceBBs[t]);

	BitBoard northCorridor = RayAttacks[(int)kingLoc][(int)Direction::North] & opponentPieces;
	BitBoard first = northCorridor & -northCorridor;

	//if first opposing piece along this ray is a rook or queen, we proceed with the check -- otherwise there cannot be a pin
	if (first & rq)
	{
		Square firstSquare = BitBoardToSquare(first);
		BitBoard northCorridorFriendly = RayAttacks[(int)kingLoc][(int)Direction::North] & RayAttacks[(int)firstSquare][(int)Direction::South] & friendlies;
		if (BBTwiddler::SingleElement(northCorridorFriendly))
		{
			northCorridor = (RayAttacks[(int)kingLoc][(int)Direction::North] & RayAttacks[(int)firstSquare][(int)Direction::South]) | first;
			pinCorridors[BitBoardToSquare(northCorridorFriendly)] = northCorridor;
			pin |= northCorridorFriendly;
		}
	}

	BitBoard eastCorridor = RayAttacks[(int)kingLoc][(int)Direction::East] & opponentPieces;
	first = eastCorridor & -eastCorridor;

	if (first & rq)
	{
		Square firstSquare = BitBoardToSquare(first);
		BitBoard eastCorridorFriendly = RayAttacks[(int)kingLoc][(int)Direction::East] & RayAttacks[(int)firstSquare][(int)Direction::West] & friendlies;
		if (BBTwiddler::SingleElement(eastCorridorFriendly))
		{
			eastCorridor = (RayAttacks[(int)kingLoc][(int)Direction::East] & RayAttacks[(int)firstSquare][(int)Direction::West]) | first;
			pinCorridors[BitBoardToSquare(eastCorridorFriendly)] = eastCorridor;
			pin |= eastCorridorFriendly;
		}
	}

	BitBoard southCorridor = RayAttacks[(int)kingLoc][(int)Direction::South] & opponentPieces;
	first = southCorridor & -southCorridor;

	if (first & rq)
	{
		Square firstSquare = BitBoardToSquare(first);
		BitBoard southCorridorFriendly = RayAttacks[(int)kingLoc][(int)Direction::South] & RayAttacks[(int)firstSquare][(int)Direction::North] & friendlies;
		if (BBTwiddler::SingleElement(southCorridorFriendly))
		{
			southCorridor = (RayAttacks[(int)kingLoc][(int)Direction::South] & RayAttacks[(int)firstSquare][(int)Direction::North]) | first;
			pinCorridors[BitBoardToSquare(southCorridorFriendly)] = southCorridor;
			pin |= southCorridorFriendly;
		}
	}

	BitBoard westCorridor = RayAttacks[(int)kingLoc][(int)Direction::West] & opponentPieces;
	first = westCorridor & -westCorridor;

	if (first & rq)
	{
		Square firstSquare = BitBoardToSquare(first);
		BitBoard westCorridorFriendly = RayAttacks[(int)kingLoc][(int)Direction::West] & RayAttacks[(int)firstSquare][(int)Direction::East] & friendlies;
		if (BBTwiddler::SingleElement(westCorridorFriendly))
		{
			westCorridor = (RayAttacks[(int)kingLoc][(int)Direction::West] & RayAttacks[(int)firstSquare][(int)Direction::East]) | first;
			pinCorridors[BitBoardToSquare(westCorridorFriendly)] = westCorridor;
			pin |= westCorridorFriendly;
		}
	}

	BitBoard neCorridor = RayAttacks[(int)kingLoc][(int)Direction::Northeast] & opponentPieces;
	first = neCorridor & -neCorridor;

	if (first & bq)
	{
		Square firstSquare = BitBoardToSquare(first);
		BitBoard neCorridorFriendly = RayAttacks[(int)kingLoc][(int)Direction::Northeast] & RayAttacks[(int)firstSquare][(int)Direction::Southwest] & friendlies;
		if (BBTwiddler::SingleElement(neCorridorFriendly))
		{
			neCorridor = (RayAttacks[(int)kingLoc][(int)Direction::Northeast] & RayAttacks[(int)firstSquare][(int)Direction::Southwest]) | first;
			pinCorridors[BitBoardToSquare(neCorridorFriendly)] = neCorridor;
			pin |= neCorridorFriendly;
		}
	}

	BitBoard seCorridor = RayAttacks[(int)kingLoc][(int)Direction::Southeast] & opponentPieces;
	first = seCorridor & -seCorridor;

	if (first & bq)
	{
		Square firstSquare = BitBoardToSquare(first);
		BitBoard seCorridorFriendly = RayAttacks[(int)kingLoc][(int)Direction::Southeast] & RayAttacks[(int)firstSquare][(int)Direction::Northwest] & friendlies;
		if (BBTwiddler::SingleElement(seCorridorFriendly))
		{
			seCorridor = (RayAttacks[(int)kingLoc][(int)Direction::Southeast] & RayAttacks[(int)firstSquare][(int)Direction::Northwest]) | first;
			pinCorridors[BitBoardToSquare(seCorridorFriendly)] = seCorridor;
			pin |= seCorridorFriendly;
		}
	}

	BitBoard swCorridor = RayAttacks[(int)kingLoc][(int)Direction::Southwest] & opponentPieces;
	first = swCorridor & -swCorridor;

	if (first & bq)
	{
		Square firstSquare = BitBoardToSquare(first);
		BitBoard swCorridorFriendly = RayAttacks[(int)kingLoc][(int)Direction::Southwest] & RayAttacks[(int)firstSquare][(int)Direction::Northeast] & friendlies;
		if (BBTwiddler::SingleElement(swCorridorFriendly))
		{
			swCorridor = (RayAttacks[(int)kingLoc][(int)Direction::Southwest] & RayAttacks[(int)firstSquare][(int)Direction::Northeast]) | first;
			pinCorridors[BitBoardToSquare(swCorridorFriendly)] = swCorridor;
			pin |= swCorridorFriendly;
		}
	}

	BitBoard nwCorridor = RayAttacks[(int)kingLoc][(int)Direction::Northwest] & opponentPieces;
	first = nwCorridor & -nwCorridor;

	if (first & bq)
	{
		Square firstSquare = BitBoardToSquare(first);
		BitBoard nwCorridorFriendly = RayAttacks[(int)kingLoc][(int)Direction::Northwest] & RayAttacks[(int)firstSquare][(int)Direction::Southeast] & friendlies;
		if (BBTwiddler::SingleElement(nwCorridorFriendly))
		{
			nwCorridor = (RayAttacks[(int)kingLoc][(int)Direction::Northwest] & RayAttacks[(int)firstSquare][(int)Direction::Southeast]) | first;
			pinCorridors[BitBoardToSquare(nwCorridorFriendly)] = nwCorridor;
			pin |= nwCorridorFriendly;
		}
	}
	return pin;
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

	attackers |= BBTwiddler::GetRookAttackBBSingle(occupied, (int)kingLoc, RayAttacks) & rq;
	attackers |= BBTwiddler::GetBishopAttackBBSingle(occupied, (int)kingLoc, RayAttacks) & bq;
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
		return BBTwiddler::GetPositiveRayAttack(occupied, Direction::Northeast, (int)kingLoc, RayAttacks);
	if (delta.x < 0 && delta.y > 0)
		return BBTwiddler::GetNegativeRayAttack(occupied, Direction::Southeast, (int)kingLoc, RayAttacks);
	if (delta.x < 0)
		return BBTwiddler::GetPositiveRayAttack(occupied, Direction::East, (int)kingLoc, RayAttacks);
	if(delta.x > 0 && delta.y < 0)
		return BBTwiddler::GetPositiveRayAttack(occupied, Direction::Northwest, (int)kingLoc, RayAttacks);
	if(delta.x > 0 && delta.y > 0)
		return BBTwiddler::GetNegativeRayAttack(occupied, Direction::Southwest, (int)kingLoc, RayAttacks);
	if(delta.x > 0)
		return BBTwiddler::GetNegativeRayAttack(occupied, Direction::West, (int)kingLoc, RayAttacks);
	if(delta.y < 0)
		return BBTwiddler::GetPositiveRayAttack(occupied, Direction::North, (int)kingLoc, RayAttacks);

	return BBTwiddler::GetNegativeRayAttack(occupied, Direction::South, (int)kingLoc, RayAttacks);
}
std::unordered_map<ChessBoard::Square, BitBoard> ChessBoard::GetCorridors() const
{
	return pinCorridors;
}
BitBoard ChessBoard::GetPins() const
{
	return pins;
}
void ChessBoard::OnClick(const Vei2& loc, Team t)
{
	Square sq = CoordsToSquare(loc);
	auto c = CellAt(loc);
	if (c->GetHighlight() == Cell::HighlightType::YELLOW || c->GetHighlight() == Cell::HighlightType::RED)
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
		if (CellAt(squarePreviouslyHighlighted)->GetHighlight() == Cell::HighlightType::RED)
		{
			auto moveMade = *std::find_if(userPossibleMoves.begin(), userPossibleMoves.end(), [this](_Move m) {return m.GetFlag() == _Move::Flag::QueenPromotionCapture && (Square)m.GetTarget() == squarePreviouslyHighlighted; });
			ApplyMove(moveMade, t);
		}
		else
		{
			auto moveMade = *std::find_if(userPossibleMoves.begin(), userPossibleMoves.end(), [this](_Move m) {return m.GetFlag() == _Move::Flag::QueenPromotion && (Square)m.GetTarget() == squarePreviouslyHighlighted; });
			ApplyMove(moveMade, t);
		}
		isPromoting = false;
	}

	else if (type == MoveType::KnightPromotion)
	{
		if (CellAt(squarePreviouslyHighlighted)->GetHighlight() == Cell::HighlightType::RED)
		{
			auto moveMade = *std::find_if(userPossibleMoves.begin(), userPossibleMoves.end(), [this](_Move m) {return m.GetFlag() == _Move::Flag::KnightPromotionCapture && (Square)m.GetTarget() == squarePreviouslyHighlighted; });
			ApplyMove(moveMade, t);
		}
		else
		{
			auto moveMade = *std::find_if(userPossibleMoves.begin(), userPossibleMoves.end(), [this](_Move m) {return m.GetFlag() == _Move::Flag::KnightPromotion && (Square)m.GetTarget() == squarePreviouslyHighlighted; });
			ApplyMove(moveMade, t);
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
		ApplyMove(selectedMove, t);
		ClearHighlights();
	}
}

void ChessBoard::HandleSelectionClick(const Vei2& loc, Team t)
{
	Square sq = CoordsToSquare(loc);
	BitBoard pos = SquareToBitBoard(sq);

	auto c = CellAt(loc);
	ClearHighlights();
	if(pieceBBs[t] & pos )
	{
		c->Highlight(Cell::HighlightType::BLUE);
		squarePreviouslyHighlighted = sq;

		auto moves = GetPossibleMoves(sq);
		for (const auto& move : moves)
		{
			auto flag = move.GetFlag();
			auto target = move.GetTarget();
			if (flag == _Move::Flag::Capture || flag == _Move::Flag::EnPassant || flag == _Move::Flag::KnightPromotionCapture || flag == _Move::Flag::QueenPromotionCapture)
				CellAt((Square)target)->Highlight(Cell::HighlightType::RED);
			else
				CellAt((Square)target)->Highlight(Cell::HighlightType::YELLOW);
		}
	}
}

bool ChessBoard::IsStalemate() const
{
	int numPieces = BBTwiddler::PopCount(pieceBBs[BBIndex::Occupied]);
	switch (numPieces)
	{
	case 2:
		return true;
	case 3:
		if (pieceBBs[BBIndex::Bishops] || pieceBBs[BBIndex::Knights])
			return true;
	default:
		return false;
	}
}