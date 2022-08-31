#include "ChessBoard.h"
#include "Cell.h"
#include "PseudoLegalMoveGenerator.h"

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
		KingAttacks[count] = PseudoLegalMoveGenerator::KingAttacks(pos);
	}
}

const std::vector<std::vector<BitBoard>> ChessBoard::GetRayAttacks() const
{
	return RayAttacks;
}
ChessBoard::ChessBoard(const Vei2& topLeft)
	:
	topLeft(topLeft),
	b("Results.txt")
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

std::vector<BitBoard> ChessBoard::GetPieceBBs() const
{
	return pieceBBs;
}

std::vector<ChessBoard::Square> ChessBoard::BitBoardToSquares(const BitBoard bb)
{
	std::vector<ChessBoard::Square> squares;
	for (int i = 0; i < 64; i++)
	{
		BitBoard mask = (BitBoard)0x1 << i;
		if (bb & mask)
		{
			squares.push_back(Square(i));
		}
	}
	return squares;
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
	BitBoard blackPawns = pieceBBs[(int)BBIndex::Pawns] & pieceBBs[(int)BBIndex::Black];
	std::vector<Vei2> locs = GetScreenCoords(BitBoardToSquares(blackPawns));
	for (const auto& loc : locs)
		Piece::Draw(gfx, loc, Piece::PieceType::Pawn, Team::BLACK);

	BitBoard whitePawns = pieceBBs[(int)BBIndex::Pawns] & pieceBBs[(int)BBIndex::White];
	locs = GetScreenCoords(BitBoardToSquares(whitePawns));
	for (const auto& loc : locs)
		Piece::Draw(gfx, loc, Piece::PieceType::Pawn, Team::WHITE);

	BitBoard blackRooks = pieceBBs[(int)BBIndex::Rooks] & pieceBBs[(int)BBIndex::Black];
	locs = GetScreenCoords(BitBoardToSquares(blackRooks));
	for (const auto& loc : locs)
		Piece::Draw(gfx, loc, Piece::PieceType::Rook, Team::BLACK);

	BitBoard whiteRooks = pieceBBs[(int)BBIndex::Rooks] & pieceBBs[(int)BBIndex::White];
	locs = GetScreenCoords(BitBoardToSquares(whiteRooks));
	for (const auto& loc : locs)
		Piece::Draw(gfx, loc, Piece::PieceType::Rook, Team::WHITE);

	BitBoard blackKnights = pieceBBs[(int)BBIndex::Knights] & pieceBBs[(int)BBIndex::Black];
	locs = GetScreenCoords(BitBoardToSquares(blackKnights));
	for (const auto& loc : locs)
		Piece::Draw(gfx, loc, Piece::PieceType::Knight, Team::BLACK);

	BitBoard whiteKnights = pieceBBs[(int)BBIndex::Knights] & pieceBBs[(int)BBIndex::White];
	locs = GetScreenCoords(BitBoardToSquares(whiteKnights));
	for (const auto& loc : locs)
		Piece::Draw(gfx, loc, Piece::PieceType::Knight, Team::WHITE);

	BitBoard blackBishops = pieceBBs[(int)BBIndex::Bishops] & pieceBBs[(int)BBIndex::Black];
	locs = GetScreenCoords(BitBoardToSquares(blackBishops));
	for (const auto& loc : locs)
		Piece::Draw(gfx, loc, Piece::PieceType::Bishop, Team::BLACK);

	BitBoard whiteBishops = pieceBBs[(int)BBIndex::Bishops] & pieceBBs[(int)BBIndex::White];
	locs = GetScreenCoords(BitBoardToSquares(whiteBishops));
	for (const auto& loc : locs)
		Piece::Draw(gfx, loc, Piece::PieceType::Bishop, Team::WHITE);

	BitBoard blackQueens = pieceBBs[(int)BBIndex::Queens] & pieceBBs[(int)BBIndex::Black];
	locs = GetScreenCoords(BitBoardToSquares(blackQueens));
	for (const auto& loc : locs)
		Piece::Draw(gfx, loc, Piece::PieceType::Queen, Team::BLACK);

	BitBoard whiteQueens = pieceBBs[(int)BBIndex::Queens] & pieceBBs[(int)BBIndex::White];
	locs = GetScreenCoords(BitBoardToSquares(whiteQueens));
	for (const auto& loc : locs)
		Piece::Draw(gfx, loc, Piece::PieceType::Queen, Team::WHITE);

	BitBoard blackKing = pieceBBs[(int)BBIndex::Kings] & pieceBBs[(int)BBIndex::Black];
	locs = GetScreenCoords(BitBoardToSquares(blackKing));
	for (const auto& loc : locs)
		Piece::Draw(gfx, loc, Piece::PieceType::King, Team::BLACK);

	BitBoard whiteKing = pieceBBs[(int)BBIndex::Kings] & pieceBBs[(int)BBIndex::White];
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

Vei2 ChessBoard::Dimensify(int loc)
{
	return { loc % 8, loc / 8 };
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

_Move::PieceType ChessBoard::ParseCapture(Square sq) const
{
	auto bb = SquareToBitBoard(sq);
	if (pieceBBs[(int)BBIndex::Pawns] & bb)
		return _Move::PieceType::Pawn;
	if (pieceBBs[(int)BBIndex::Rooks] & bb)
		return _Move::PieceType::Rook;
	if (pieceBBs[(int)BBIndex::Knights] & bb)
		return _Move::PieceType::Knight;
	if (pieceBBs[(int)BBIndex::Bishops] & bb)
		return _Move::PieceType::Bishop;
	return _Move::PieceType::Queen;
}

void ChessBoard::GenerateMoves(Team t)
{
	b.Start();
	userPossibleMoves = PseudoLegalMoveGenerator::GenerateMoves(t, *this);
	b.End();
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

//_Move ChessBoard::Move(_Move move)
//{
//	auto srcCell = CellAt(move.src);
//	auto team = srcCell->GetPiece()->GetTeam();
//	auto destCell = CellAt(move.dest);
//	switch (move.type)
//	{
//	case MoveType::Normal:
//	{
//		destCell->GivePiece(srcCell->GetPiece());
//		srcCell->Clear();
//		break;
//	}
//	case MoveType::EnPassant:
//	{
//		destCell->GivePiece(srcCell->GetPiece());
//		srcCell->Clear();
//		CellAt(enPassantPawnLoc)->Clear();
//		break;
//	}
//	case MoveType::QueenPromotion:
//		destCell->GivePiece(std::make_shared<Queen>(team, move.dest));
//		srcCell->Clear();
//		break;
//	case MoveType::KnightPromotion:
//		destCell->GivePiece(std::make_shared<Knight>(team, move.dest));
//		srcCell->Clear();
//		break;
//	case MoveType::KingsideCastle:
//		if (team == Team::WHITE)
//		{
//			CellAt({ 6,0 })->GivePiece(CellAt({ 4,0 })->GetPiece());
//			CellAt({ 6,0 })->GetPiece()->Update({ 6,0 });
//			CellAt({ 4,0 })->Clear();
//			CellAt({ 5,0 })->GivePiece(CellAt({ 7,0 })->GetPiece());
//			CellAt({ 5,0 })->GetPiece()->Update({ 5,0 });
//			CellAt({ 7,0 })->Clear();
//			hasCastledWhite = true;
//		}
//		else
//		{
//			CellAt({ 6,7 })->GivePiece(CellAt({ 4,7 })->GetPiece());
//			CellAt({ 6,7 })->GetPiece()->Update({ 6,7 });
//			CellAt({ 4,7 })->Clear();
//			CellAt({ 5,7 })->GivePiece(CellAt({ 7,7 })->GetPiece());
//			CellAt({ 5,7 })->GetPiece()->Update({ 5,7 });
//			CellAt({ 7,7 })->Clear();
//			hasCastledBlack = true;
//		}
//		break;
//	case MoveType::QueensideCastle:
//		if (team == Team::WHITE)
//		{
//			CellAt({ 2,0 })->GivePiece(CellAt({ 4,0 })->GetPiece());
//			CellAt({ 2,0 })->GetPiece()->Update({ 2,0 });
//			CellAt({ 4,0 })->Clear();
//			CellAt({ 3,0 })->GivePiece(CellAt({ 0,0 })->GetPiece());
//			CellAt({ 3,0 })->GetPiece()->Update({ 3,0 });
//			CellAt({ 0,0 })->Clear();
//			hasCastledWhite = true;
//		}
//		else
//		{
//			CellAt({ 2,7 })->GivePiece(CellAt({ 4,7 })->GetPiece());
//			CellAt({ 2,7 })->GetPiece()->Update({ 2,7 });
//			CellAt({ 4,7 })->Clear();
//			CellAt({ 3,7 })->GivePiece(CellAt({ 0,7 })->GetPiece());
//			CellAt({ 3,7 })->GetPiece()->Update({ 3,7 });
//			CellAt({ 0,7 })->Clear();
//			hasCastledBlack = true;
//		}
//	}
//
//	if(move.type != MoveType::KingsideCastle && move.type != MoveType::QueensideCastle)
//		PostMoveUpdate(CellAt(move.dest)->GetPiece(), move.dest);
//	return move;
//}

//calling function's team is the team the player is on, so we check if the opponent has been put in check (which is why it may appear backward).
//void ChessBoard::IsInCheck(Team t)
//{
//	if (t == Team::WHITE)
//	{
//		blackInCheck = IsBlackInCheck();
//		if (blackInCheck)
//		{
//			IsCheckmate(Team::BLACK);
//		}
//
//	}
//	else
//	{
//		whiteInCheck = IsWhiteInCheck();
//		if (whiteInCheck)
//		{
//			IsCheckmate(Team::WHITE);
//		}
//	}
//}
//
//bool ChessBoard::IsWhiteInCheck() const
//{
//	for (const auto& cell : cells)
//	{
//		if (!cell->Empty())
//		{
//			auto p = cell->GetPiece();
//			if (p->GetTeam() == Team::BLACK)
//			{
//				auto moves = p->GetPossibleAttackMoves(*this);
//				for (const auto& move : moves)
//				{
//					auto c = CellAt(move.dest);
//					if (!c->Empty() && c->GetPiece()->GetTeam() == Team::WHITE && typeid(*c->GetPiece()) == typeid(King))
//						return true;
//				}
//			}
//		}
//	}
//	return false;
//}
//bool ChessBoard::IsBlackInCheck() const
//{
//	for (const auto& cell : cells)
//	{
//		if (!cell->Empty())
//		{
//			auto p = cell->GetPiece();
//			if (p->GetTeam() == Team::WHITE)
//			{
//				auto moves = p->GetPossibleAttackMoves(*this);
//				for (const auto& move : moves)
//				{
//					auto c = CellAt(move.dest);
//					if (!c->Empty() && c->GetPiece()->GetTeam() == Team::BLACK && typeid(*c->GetPiece()) == typeid(King))
//						return true;
//				}
//			}
//		}
//	}
//	return false;
//}
//bool ChessBoard::SimulateAndCheck(_Move move)
//{
//	if (move.type == MoveType::KingsideCastle || move.type == MoveType::QueensideCastle)
//		return false;
//	auto t = CellAt(move.src)->GetPiece()->GetTeam();
//	auto srcCell = CellAt(move.src);
//	auto destCell = CellAt(move.dest);
//
//	//make a copy of the piece at destCell
//	auto destPiece = CellAt(move.dest)->GetPiece();
//
//	destCell->GivePiece(srcCell->GetPiece());
//	srcCell->Clear();
//
//	std::shared_ptr<Piece> passant = nullptr;
//	bool passantified = false;
//
//	if (move.type == MoveType::EnPassant)
//	{
//		passant = CellAt(enPassantPawnLoc)->GetPiece();
//		CellAt(enPassantPawnLoc)->Clear();
//		passantified = true;
//	}
//
//	//test if this move causes king to be in check
//	bool badMove = t == Team::WHITE ? IsWhiteInCheck() : IsBlackInCheck();
//
//	//restore pieces to their original position
//	srcCell->GivePiece(destCell->GetPiece());
//	destCell->GivePiece(destPiece);
//	if (passantified)
//		CellAt(enPassantPawnLoc)->GivePiece(passant);
//
//	return badMove;
//}
//std::vector<_Move> ChessBoard::GetValidMoves(const Vei2& loc)
//{
//	if (CellAt(loc)->Empty())
//		return {};
//	std::vector<_Move> ans;
//	auto moves = CellAt(loc)->GetPiece()->GetPossibleMoves(*this);
//	for (const auto& move : moves)
//	{
//		if (!SimulateAndCheck(move))
//			ans.push_back(move);
//	}
//	return ans;
//}
//
//bool ChessBoard::IsUnderAttack(Team t, const Vei2& loc) const
//{
//	for (const auto& cell : cells)
//	{
//		if (!cell->Empty())
//		{
//			auto piece = cell->GetPiece();
//			if (piece->GetTeam() != t)
//			{
//				auto attackMoves = piece->GetPossibleAttackMoves(*this);
//				for (const auto& l : attackMoves)
//				{
//					if (l.dest == loc)
//						return true;
//				}
//			}
//		}
//	}
//	return false;
//}
//
//bool ChessBoard::CanCastleKingside(Team t) const
//{
//	if (t == Team::WHITE)
//	{
//		if (hasCastledWhite)
//			return false;
//		if (!CellAt({ 4,0 })->Empty() && !CellAt({ 7,0 })->Empty())
//		{
//			auto k = CellAt({ 4,0 })->GetPiece();
//			auto r = CellAt({ 7,0 })->GetPiece();
//			if (typeid(*k) == typeid(King) && typeid(*r) == typeid(Rook) && !k->HasMoved() && !r->HasMoved() && CellAt({ 5,0 })->Empty() && CellAt({ 6,0 })->Empty() && !IsUnderAttack(t, { 5,0 }) && !IsUnderAttack(t, { 6,0 }) && !IsWhiteInCheck())
//				return true;
//		}
//	}
//	else
//	{
//		if (hasCastledBlack)
//			return false;
//		if (!CellAt({ 4,7 })->Empty() && !CellAt({ 7,7 })->Empty())
//		{
//			auto k = CellAt({ 4,7 })->GetPiece();
//			auto r = CellAt({ 7,7 })->GetPiece();
//			if (typeid(*k) == typeid(King) && typeid(*r) == typeid(Rook) && !k->HasMoved() && !r->HasMoved() && CellAt({ 5,7 })->Empty() && CellAt({ 6,7 })->Empty() && !IsUnderAttack(t, { 5,7 }) && !IsUnderAttack(t, { 6,7 }) && !IsBlackInCheck())
//				return true;
//		}
//	}
//	return false;
//}
//bool ChessBoard::CanCastleQueenside(Team t) const
//{
//	if (t == Team::WHITE)
//	{
//		if (hasCastledWhite)
//			return false;
//		if (!CellAt({ 4,0 })->Empty() && !CellAt({ 0,0 })->Empty())
//		{
//			auto k = CellAt({ 4,0 })->GetPiece();
//			auto r = CellAt({ 0,0 })->GetPiece();
//			if (typeid(*k) == typeid(King) && typeid(*r) == typeid(Rook) && !k->HasMoved() && !r->HasMoved() && CellAt({ 1,0 })->Empty() && CellAt({ 2,0 })->Empty() && CellAt({ 3,0 })->Empty() && !IsUnderAttack(t, { 2,0 }) && !IsUnderAttack(t, { 3,0 }))
//				return true;
//		}
//	}
//	else
//	{
//		if (hasCastledBlack)
//			return false;
//		if (!CellAt({ 4,7 })->Empty() && !CellAt({ 0,7 })->Empty())
//		{
//			auto k = CellAt({ 4,7 })->GetPiece();
//			auto r = CellAt({ 0,7 })->GetPiece();
//			if (typeid(*k) == typeid(King) && typeid(*r) == typeid(Rook) && !k->HasMoved() && !r->HasMoved() && CellAt({ 1,7 })->Empty() && CellAt({ 2,7 })->Empty() && CellAt({ 3,7 })->Empty() && !IsUnderAttack(t, { 2,7 }) && !IsUnderAttack(t, { 3,7 }))
//				return true;
//		}
//	}
//	return false;
//}
//void ChessBoard::PostMoveUpdate(const std::shared_ptr<Piece> p, const Vei2& loc)
//{
//	p->Update(loc);
//	if (typeid(*p) == typeid(Pawn))
//	{
//		if (p->GetNumMoves() == 1 && (loc.y == 4 || loc.y == 3))
//		{
//			isEnPassantable = true;
//			enPassantSquare = p->GetTeam() == Team::BLACK ? loc + Vei2{ 0, 1 } : loc + Vei2{ 0,-1 };
//			enPassantPawnLoc = loc;
//			passantTeam = p->GetTeam();
//			return;
//		}
//		else if ((loc.y == 7 && p->GetTeam() == Team::WHITE) || (loc.y == 0 && p->GetTeam() == Team::BLACK))
//			isPromoting = true;
//	}
//	isEnPassantable = false;
//}
//void ChessBoard::IsCheckmate(Team t)
//{
//	for (const auto& c : cells)
//	{
//		if (!c->Empty() && c->GetPiece()->GetTeam() == t)
//		{
//			std::vector<_Move> moves = GetValidMoves(c->GetLoc());
//			if (moves.size() > 0)
//				return;
//		}
//	}
//	//if no moves can be made after looping through each cell, then we must be in a checkmate state.
//	isCheckmate = true;
//}
bool ChessBoard::IsCheckmate() const
{
	return isCheckmate;
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

void ChessBoard::ApplyMove(_Move m, Team t)
{
	auto flag = m.GetFlag();
	_Move::PieceType srcPiece = m.GetSourcePiece();
	_Move::PieceType captured = m.GetCapturedPiece();
	Square src = (Square)m.GetSource();
	Square dest = (Square)m.GetTarget();
	auto srcBB = SquareToBitBoard(src);
	auto destBB = SquareToBitBoard(dest);
	//no matter what kind of move it is, we always clear out the source square. So we will update the bitboards accordingly.
	pieceBBs[(int)BBIndex::Occupied] ^= srcBB;
	pieceBBs[(int)BBIndex::Empty] |= srcBB;
	pieceBBs[(int)BBIndex::White] &= pieceBBs[(int)BBIndex::Occupied];
	pieceBBs[(int)BBIndex::Black] &= pieceBBs[(int)BBIndex::Occupied];
	pieceBBs[PieceTypeMatcher(srcPiece)] &= pieceBBs[(int)BBIndex::Occupied];

	switch (flag)
	{
	case _Move::Flag::KnightPromotionCapture:
		if (t == Team::WHITE)
		{
			//remove captured piece
			pieceBBs[(int)BBIndex::Black] ^= destBB;

			//add piece as appropriate
			pieceBBs[(int)BBIndex::White] |= destBB;
		}
		else
		{
			//remove captured piece
			pieceBBs[(int)BBIndex::White] ^= destBB;

			//add piece as appropriate
			pieceBBs[(int)BBIndex::Black] |= destBB;
		}
		pieceBBs[PieceTypeMatcher(captured)] ^= destBB;
		pieceBBs[(int)BBIndex::Knights] |= destBB;
		break;
	case _Move::Flag::QueenPromotionCapture:
		if (t == Team::WHITE)
		{
			//remove captured piece
			pieceBBs[(int)BBIndex::Black] ^= destBB;

			//add piece as appropriate
			pieceBBs[(int)BBIndex::White] |= destBB;
		}
		else
		{
			//remove captured piece
			pieceBBs[(int)BBIndex::White] ^= destBB;

			//add piece as appropriate
			pieceBBs[(int)BBIndex::Black] |= destBB;
		}
		pieceBBs[PieceTypeMatcher(captured)] ^= destBB;
		pieceBBs[(int)BBIndex::Queens] |= destBB;
		break;
	case _Move::Flag::KnightPromotion:
		pieceBBs[(int)BBIndex::Occupied] |= destBB;
		pieceBBs[(int)BBIndex::Empty] ^= destBB;
		pieceBBs[(int)BBIndex::Knights] |= destBB;
		pieceBBs[(int)BBIndex::Pawns] &= pieceBBs[(int)BBIndex::Occupied];
		if (t == Team::WHITE)
			pieceBBs[(int)BBIndex::White] |= destBB;
		else
			pieceBBs[(int)BBIndex::Black] |= destBB;
		break;
	case _Move::Flag::QueenPromotion:
		pieceBBs[(int)BBIndex::Occupied] |= destBB;
		pieceBBs[(int)BBIndex::Empty] ^= destBB;
		pieceBBs[(int)BBIndex::Queens] |= destBB;
		pieceBBs[(int)BBIndex::Pawns] &= pieceBBs[(int)BBIndex::Occupied];
		if (t == Team::WHITE)
			pieceBBs[(int)BBIndex::White] |= destBB;
		else
			pieceBBs[(int)BBIndex::Black] |= destBB;
		break;
	case _Move::Flag::Capture:
		if (t == Team::WHITE)
		{
			//remove captured piece
			pieceBBs[(int)BBIndex::Black] ^= destBB;

			//add piece as appropriate
			pieceBBs[(int)BBIndex::White] |= destBB;
		}
		else
		{
			//remove captured piece
			pieceBBs[(int)BBIndex::White] ^= destBB;

			//add piece as appropriate
			pieceBBs[(int)BBIndex::Black] |= destBB;
		}
		pieceBBs[PieceTypeMatcher(captured)] ^= destBB;
		pieceBBs[PieceTypeMatcher(srcPiece)] |= destBB;
		break;
	case _Move::Flag::PawnDoublePush:
		isEnPassantable = true;
		//TODO - figure out enpassant square
		pieceBBs[(int)BBIndex::Occupied] |= destBB;
		pieceBBs[(int)BBIndex::Empty] ^= destBB;
		pieceBBs[(int)BBIndex::Pawns] |= destBB;
		if (t == Team::WHITE)
			pieceBBs[(int)BBIndex::White] |= destBB;
		else
			pieceBBs[(int)BBIndex::Black] |= destBB;
		break;
	case _Move::Flag::None:
		pieceBBs[(int)BBIndex::Occupied] |= destBB;
		pieceBBs[(int)BBIndex::Empty] ^= destBB;
		pieceBBs[PieceTypeMatcher(srcPiece)] |= destBB;
		pieceBBs[PieceTypeMatcher(srcPiece)] &= pieceBBs[(int)BBIndex::Occupied];
		if (t == Team::WHITE)
			pieceBBs[(int)BBIndex::White] |= destBB;
		else
			pieceBBs[(int)BBIndex::Black] |= destBB;
		break;
	}
	plies.push(m);

	//finally, need to update other team's king danger squares

}

int ChessBoard::PieceTypeMatcher(_Move::PieceType p) const
{
	switch (p)
	{
	case _Move::PieceType::Pawn:
		return (int)BBIndex::Pawns;
	case _Move::PieceType::Rook:
		return (int)BBIndex::Rooks;
	case _Move::PieceType::Knight:
		return (int)BBIndex::Knights;
	case _Move::PieceType::Bishop:
		return (int)BBIndex::Bishops;
	case _Move::PieceType::Queen:
		return (int)BBIndex::Queens;
	case _Move::PieceType::King:
		return (int)BBIndex::Kings;
	default:
		return -1;
	}
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
			moveMade = *std::find_if(userPossibleMoves.begin(), userPossibleMoves.end(), [this](_Move m) {return m.GetFlag() == _Move::Flag::QueenPromotionCapture && (Square)m.GetTarget() == squarePreviouslyHighlighted; });
			ApplyMove(moveMade, t);
		}
		else
		{
			moveMade = *std::find_if(userPossibleMoves.begin(), userPossibleMoves.end(), [this](_Move m) {return m.GetFlag() == _Move::Flag::QueenPromotion && (Square)m.GetTarget() == squarePreviouslyHighlighted; });
			ApplyMove(moveMade, t);
		}
		turnSwap = true;
		isPromoting = false;
		//IsInCheck(t);
	}

	else if (type == MoveType::KnightPromotion)
	{
		if (CellAt(squarePreviouslyHighlighted)->GetHighlight() == Cell::HighlightType::RED)
		{
			moveMade = *std::find_if(userPossibleMoves.begin(), userPossibleMoves.end(), [this](_Move m) {return m.GetFlag() == _Move::Flag::KnightPromotionCapture && (Square)m.GetTarget() == squarePreviouslyHighlighted; });
			ApplyMove(moveMade, t);
		}
		else
		{
			moveMade = *std::find_if(userPossibleMoves.begin(), userPossibleMoves.end(), [this](_Move m) {return m.GetFlag() == _Move::Flag::KnightPromotion && (Square)m.GetTarget() == squarePreviouslyHighlighted; });
			ApplyMove(moveMade, t);
		}
		turnSwap = true;
		isPromoting = false;
		//IsInCheck(t);
	}
	ClearHighlights();

	//if (t == Team::WHITE)
	//{
	//	whiteInCheck = false;
	//}
	//else
	//{
	//	blackInCheck = false;
	//}
}

void ChessBoard::HandleMoveClick(const Square sq, Team t)
{
	_Move selectedMove;
	for (const auto& move : userPossibleMoves)
	{
		if ((Square)move.GetTarget() == sq && (Square)move.GetSource() == squarePreviouslyHighlighted)
		{
			selectedMove = move;
			moveMade = selectedMove;
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
		//as we are not promoting a piece, we now test to see if we have put the other player in check.
		turnSwap = true;
		//if white successfully makes a move, then white necessarily cannot be in check.
		//if (t == Team::WHITE)
		//{
		//	whiteInCheck = false;
		//}
		////similarly for black.
		//else
		//{
		//	blackInCheck = false;
		//}
		//IsInCheck(t);
		ClearHighlights();
	}
}

void ChessBoard::HandleSelectionClick(const Vei2& loc, Team t)
{
	Square sq = CoordsToSquare(loc);
	BitBoard pos = SquareToBitBoard(sq);

	auto c = CellAt(loc);
	ClearHighlights();
	if(pieceBBs[(int)t] & pos )
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

