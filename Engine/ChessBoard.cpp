#include "ChessBoard.h"
#include "Cell.h"

ChessBoard::ChessBoard(const Vei2& topLeft)
	:
	topLeft(topLeft)
{
	for (int row = 0; row < 8; row++)
	{
		Cell::Shade startShade = row % 2 ? Cell::Shade::DARK : Cell::Shade::LIGHT;
		for (int col = 0; col < 8; col++)
		{
			Vei2 loc = { row,col };
			auto sh = Cell::Shade(((int)startShade + col) % 2);
			cells[LinearizeCoords(loc)] = std::make_shared<Cell>(sh, loc);
		}
	}
	for (int x = 0; x < 8; x++)
	{
		int i = LinearizeCoords({ x,6 });
		cells[i]->GivePiece(std::make_shared<Pawn>(Team::BLACK, Vei2{ x,6 }));
	}
	cells[LinearizeCoords({ 0,7 })]->GivePiece( std::make_shared<Rook>(Team::BLACK,  Vei2{ 0,7 }));
	cells[LinearizeCoords({ 1,7 })]->GivePiece( std::make_shared<Knight>(Team::BLACK, Vei2{ 1,7 }));
	cells[LinearizeCoords({ 2,7 })]->GivePiece( std::make_shared<Bishop>(Team::BLACK, Vei2{ 2,7 }));
	cells[LinearizeCoords({ 3,7 })]->GivePiece( std::make_shared<Queen>(Team::BLACK, Vei2{ 3,7 }));
	cells[LinearizeCoords({ 4,7 })]->GivePiece( std::make_shared<King>(Team::BLACK, Vei2{4,7 }));
	cells[LinearizeCoords({ 5,7 })]->GivePiece( std::make_shared<Bishop>(Team::BLACK, Vei2{ 5,7 }));
	cells[LinearizeCoords({ 6,7 })]->GivePiece( std::make_shared<Knight>(Team::BLACK, Vei2{ 6,7 }));
	cells[LinearizeCoords({ 7,7 })]->GivePiece( std::make_shared<Rook>(Team::BLACK, Vei2{ 7,7 }));

	for (int x = 0; x < 8; x++)
	{
		int i = LinearizeCoords({ x,1 });
		cells[i]->GivePiece(std::make_shared<Pawn>(Team::WHITE, Vei2{x,1}));
	}
	cells[LinearizeCoords({ 0,0 })]->GivePiece(std::make_shared<Rook>(Team::WHITE, Vei2{ 0,0 }));
	cells[LinearizeCoords({ 1,0 })]->GivePiece(std::make_shared<Knight>(Team::WHITE, Vei2{ 1,0 }));
	cells[LinearizeCoords({ 2,0 })]->GivePiece(std::make_shared<Bishop>(Team::WHITE, Vei2{ 2,0 }));
	cells[LinearizeCoords({ 3,0 })]->GivePiece(std::make_shared<Queen>(Team::WHITE, Vei2{ 3,0 }));
	cells[LinearizeCoords({ 4,0 })]->GivePiece(std::make_shared<King>(Team::WHITE, Vei2{ 4,0 }));
	cells[LinearizeCoords({ 5,0 })]->GivePiece(std::make_shared<Bishop>(Team::WHITE, Vei2{ 5,0 }));
	cells[LinearizeCoords({ 6,0 })]->GivePiece(std::make_shared<Knight>(Team::WHITE, Vei2{ 6,0 }));
	cells[LinearizeCoords({ 7,0 })]->GivePiece(std::make_shared<Rook>(Team::WHITE, Vei2{ 7,0 }));
	
}

void ChessBoard::Draw(Graphics& gfx) const
{
	for (const auto& c : cells)
	{
		auto offSet = GetOffset();
		c->DrawCell(gfx, offSet);
	}
}

int ChessBoard::LinearizeCoords(const Vei2& loc)
{
	return loc.x + 8 * loc.y;
}

Vei2 ChessBoard::Dimensify(int loc)
{
	return { loc % 8, loc / 8 };
}

ChessBoard::Square ChessBoard::CoordsToSquare(const Vei2& coords) const
{
	return (Square)(8 * coords.y + coords.x);
}

BitBoard ChessBoard::SquareToBitboard(const Square sq) const
{
	BitBoard position = 0x0000000000000001;
	//sets a 1 bit for the position encoded by Square using LERF enumeration.
	return position << (int)sq;
}

Vei2 ChessBoard::GetOffset() const
{
	auto offSet = topLeft;
	offSet.y += 8 * cellSize;
	return offSet;
}

std::shared_ptr<Cell> ChessBoard::CellAt(const Vei2& loc)
{
	return cells[LinearizeCoords(loc)];
}

std::shared_ptr<Cell> ChessBoard::CellAt(const Vei2& loc) const
{
	return cells[LinearizeCoords(loc)];
}

void ChessBoard::ReleaseHighlights()
{
	for (auto& c : cells)
	{
		c->ReleaseHighlight();
	}
}

bool ChessBoard::IsValidLoc(const Vei2& loc)
{
	if (loc.x >= 0 && loc.x < 8 && loc.y >= 0 && loc.y < 8)
		return true;
	else if (loc.x == 8 && loc.y >= 0 && loc.y < 2)
		return true;
	return false;
}

std::vector<_Move> ChessBoard::GetPossibleMoves(const Vei2& loc) const
{
	return CellAt(loc)->GetPiece()->GetPossibleMoves(*this);
}

_Move ChessBoard::Move(_Move move)
{
	auto srcCell = CellAt(move.src);
	auto team = srcCell->GetPiece()->GetTeam();
	auto destCell = CellAt(move.dest);
	switch (move.type)
	{
	case MoveType::Normal:
	{
		destCell->GivePiece(srcCell->GetPiece());
		srcCell->Clear();
		break;
	}
	case MoveType::EnPassant:
	{
		destCell->GivePiece(srcCell->GetPiece());
		srcCell->Clear();
		CellAt(enPassantPawnLoc)->Clear();
		break;
	}
	case MoveType::QueenPromotion:
		destCell->GivePiece(std::make_shared<Queen>(team, move.dest));
		srcCell->Clear();
		break;
	case MoveType::KnightPromotion:
		destCell->GivePiece(std::make_shared<Knight>(team, move.dest));
		srcCell->Clear();
		break;
	case MoveType::KingsideCastle:
		if (team == Team::WHITE)
		{
			CellAt({ 6,0 })->GivePiece(CellAt({ 4,0 })->GetPiece());
			CellAt({ 6,0 })->GetPiece()->Update({ 6,0 });
			CellAt({ 4,0 })->Clear();
			CellAt({ 5,0 })->GivePiece(CellAt({ 7,0 })->GetPiece());
			CellAt({ 5,0 })->GetPiece()->Update({ 5,0 });
			CellAt({ 7,0 })->Clear();
			hasCastledWhite = true;
		}
		else
		{
			CellAt({ 6,7 })->GivePiece(CellAt({ 4,7 })->GetPiece());
			CellAt({ 6,7 })->GetPiece()->Update({ 6,7 });
			CellAt({ 4,7 })->Clear();
			CellAt({ 5,7 })->GivePiece(CellAt({ 7,7 })->GetPiece());
			CellAt({ 5,7 })->GetPiece()->Update({ 5,7 });
			CellAt({ 7,7 })->Clear();
			hasCastledBlack = true;
		}
		break;
	case MoveType::QueensideCastle:
		if (team == Team::WHITE)
		{
			CellAt({ 2,0 })->GivePiece(CellAt({ 4,0 })->GetPiece());
			CellAt({ 2,0 })->GetPiece()->Update({ 2,0 });
			CellAt({ 4,0 })->Clear();
			CellAt({ 3,0 })->GivePiece(CellAt({ 0,0 })->GetPiece());
			CellAt({ 3,0 })->GetPiece()->Update({ 3,0 });
			CellAt({ 0,0 })->Clear();
			hasCastledWhite = true;
		}
		else
		{
			CellAt({ 2,7 })->GivePiece(CellAt({ 4,7 })->GetPiece());
			CellAt({ 2,7 })->GetPiece()->Update({ 2,7 });
			CellAt({ 4,7 })->Clear();
			CellAt({ 3,7 })->GivePiece(CellAt({ 0,7 })->GetPiece());
			CellAt({ 3,7 })->GetPiece()->Update({ 3,7 });
			CellAt({ 0,7 })->Clear();
			hasCastledBlack = true;
		}
	}

	if(move.type != MoveType::KingsideCastle && move.type != MoveType::QueensideCastle)
		PostMoveUpdate(CellAt(move.dest)->GetPiece(), move.dest);
	return move;
}

//calling function's team is the team the player is on, so we check if the opponent has been put in check (which is why it may appear backward).
void ChessBoard::IsInCheck(Team t)
{
	if (t == Team::WHITE)
	{
		blackInCheck = IsBlackInCheck();
		if (blackInCheck)
		{
			IsCheckmate(Team::BLACK);
		}

	}
	else
	{
		whiteInCheck = IsWhiteInCheck();
		if (whiteInCheck)
		{
			IsCheckmate(Team::WHITE);
		}
	}
}

bool ChessBoard::IsWhiteInCheck() const
{
	for (const auto& cell : cells)
	{
		if (!cell->Empty())
		{
			auto p = cell->GetPiece();
			if (p->GetTeam() == Team::BLACK)
			{
				auto moves = p->GetPossibleAttackMoves(*this);
				for (const auto& move : moves)
				{
					auto c = CellAt(move.dest);
					if (!c->Empty() && c->GetPiece()->GetTeam() == Team::WHITE && typeid(*c->GetPiece()) == typeid(King))
						return true;
				}
			}
		}
	}
	return false;
}
bool ChessBoard::IsBlackInCheck() const
{
	for (const auto& cell : cells)
	{
		if (!cell->Empty())
		{
			auto p = cell->GetPiece();
			if (p->GetTeam() == Team::WHITE)
			{
				auto moves = p->GetPossibleAttackMoves(*this);
				for (const auto& move : moves)
				{
					auto c = CellAt(move.dest);
					if (!c->Empty() && c->GetPiece()->GetTeam() == Team::BLACK && typeid(*c->GetPiece()) == typeid(King))
						return true;
				}
			}
		}
	}
	return false;
}
bool ChessBoard::SimulateAndCheck(_Move move)
{
	if (move.type == MoveType::KingsideCastle || move.type == MoveType::QueensideCastle)
		return false;
	auto t = CellAt(move.src)->GetPiece()->GetTeam();
	auto srcCell = CellAt(move.src);
	auto destCell = CellAt(move.dest);

	//make a copy of the piece at destCell
	auto destPiece = CellAt(move.dest)->GetPiece();

	destCell->GivePiece(srcCell->GetPiece());
	srcCell->Clear();

	std::shared_ptr<Piece> passant = nullptr;
	bool passantified = false;

	if (move.type == MoveType::EnPassant)
	{
		passant = CellAt(enPassantPawnLoc)->GetPiece();
		CellAt(enPassantPawnLoc)->Clear();
		passantified = true;
	}

	//test if this move causes king to be in check
	bool badMove = t == Team::WHITE ? IsWhiteInCheck() : IsBlackInCheck();

	//restore pieces to their original position
	srcCell->GivePiece(destCell->GetPiece());
	destCell->GivePiece(destPiece);
	if (passantified)
		CellAt(enPassantPawnLoc)->GivePiece(passant);

	return badMove;
}
std::vector<_Move> ChessBoard::GetValidMoves(const Vei2& loc)
{
	if (CellAt(loc)->Empty())
		return {};
	std::vector<_Move> ans;
	auto moves = CellAt(loc)->GetPiece()->GetPossibleMoves(*this);
	for (const auto& move : moves)
	{
		if (!SimulateAndCheck(move))
			ans.push_back(move);
	}
	return ans;
}

bool ChessBoard::IsUnderAttack(Team t, const Vei2& loc) const
{
	for (const auto& cell : cells)
	{
		if (!cell->Empty())
		{
			auto piece = cell->GetPiece();
			if (piece->GetTeam() != t)
			{
				auto attackMoves = piece->GetPossibleAttackMoves(*this);
				for (const auto& l : attackMoves)
				{
					if (l.dest == loc)
						return true;
				}
			}
		}
	}
	return false;
}

bool ChessBoard::CanCastleKingside(Team t) const
{
	if (t == Team::WHITE)
	{
		if (hasCastledWhite)
			return false;
		if (!CellAt({ 4,0 })->Empty() && !CellAt({ 7,0 })->Empty())
		{
			auto k = CellAt({ 4,0 })->GetPiece();
			auto r = CellAt({ 7,0 })->GetPiece();
			if (typeid(*k) == typeid(King) && typeid(*r) == typeid(Rook) && !k->HasMoved() && !r->HasMoved() && CellAt({ 5,0 })->Empty() && CellAt({ 6,0 })->Empty() && !IsUnderAttack(t, { 5,0 }) && !IsUnderAttack(t, { 6,0 }) && !IsWhiteInCheck())
				return true;
		}
	}
	else
	{
		if (hasCastledBlack)
			return false;
		if (!CellAt({ 4,7 })->Empty() && !CellAt({ 7,7 })->Empty())
		{
			auto k = CellAt({ 4,7 })->GetPiece();
			auto r = CellAt({ 7,7 })->GetPiece();
			if (typeid(*k) == typeid(King) && typeid(*r) == typeid(Rook) && !k->HasMoved() && !r->HasMoved() && CellAt({ 5,7 })->Empty() && CellAt({ 6,7 })->Empty() && !IsUnderAttack(t, { 5,7 }) && !IsUnderAttack(t, { 6,7 }) && !IsBlackInCheck())
				return true;
		}
	}
	return false;
}
bool ChessBoard::CanCastleQueenside(Team t) const
{
	if (t == Team::WHITE)
	{
		if (hasCastledWhite)
			return false;
		if (!CellAt({ 4,0 })->Empty() && !CellAt({ 0,0 })->Empty())
		{
			auto k = CellAt({ 4,0 })->GetPiece();
			auto r = CellAt({ 0,0 })->GetPiece();
			if (typeid(*k) == typeid(King) && typeid(*r) == typeid(Rook) && !k->HasMoved() && !r->HasMoved() && CellAt({ 1,0 })->Empty() && CellAt({ 2,0 })->Empty() && CellAt({ 3,0 })->Empty() && !IsUnderAttack(t, { 2,0 }) && !IsUnderAttack(t, { 3,0 }))
				return true;
		}
	}
	else
	{
		if (hasCastledBlack)
			return false;
		if (!CellAt({ 4,7 })->Empty() && !CellAt({ 0,7 })->Empty())
		{
			auto k = CellAt({ 4,7 })->GetPiece();
			auto r = CellAt({ 0,7 })->GetPiece();
			if (typeid(*k) == typeid(King) && typeid(*r) == typeid(Rook) && !k->HasMoved() && !r->HasMoved() && CellAt({ 1,7 })->Empty() && CellAt({ 2,7 })->Empty() && CellAt({ 3,7 })->Empty() && !IsUnderAttack(t, { 2,7 }) && !IsUnderAttack(t, { 3,7 }))
				return true;
		}
	}
	return false;
}
void ChessBoard::PostMoveUpdate(const std::shared_ptr<Piece> p, const Vei2& loc)
{
	p->Update(loc);
	if (typeid(*p) == typeid(Pawn))
	{
		if (p->GetNumMoves() == 1 && (loc.y == 4 || loc.y == 3))
		{
			isEnPassantable = true;
			enPassantSquare = p->GetTeam() == Team::BLACK ? loc + Vei2{ 0, 1 } : loc + Vei2{ 0,-1 };
			enPassantPawnLoc = loc;
			passantTeam = p->GetTeam();
			return;
		}
		else if ((loc.y == 7 && p->GetTeam() == Team::WHITE) || (loc.y == 0 && p->GetTeam() == Team::BLACK))
			isPromoting = true;
	}
	isEnPassantable = false;
}
void ChessBoard::IsCheckmate(Team t)
{
	for (const auto& c : cells)
	{
		if (!c->Empty() && c->GetPiece()->GetTeam() == t)
		{
			std::vector<_Move> moves = GetValidMoves(c->GetLoc());
			if (moves.size() > 0)
				return;
		}
	}
	//if no moves can be made after looping through each cell, then we must be in a checkmate state.
	isCheckmate = true;
}
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

/*This function will do a variety of things, based on where we are at in the move pipeline. If we click on a cell with a piece of the same color as our team, then we highlight the moves it can make.
If we click on a highlighted square (either yellow or red), we will move the selected piece to the location where we clicked. If we click on any other square, we will do nothing. If we are in the promotion
phase, we will only promote a piece if the relevant piece choice is clicked on.*/
void ChessBoard::OnClick(const Vei2& loc, Team t)
{

	//otherwise, we must be in the selection phase, so we handle piece selection and movement. The following handles movement.
	auto c = CellAt(loc);
	if (c->GetHighlight() == Cell::HighlightType::YELLOW || c->GetHighlight() == Cell::HighlightType::RED)
	{
		HandleMoveClick(loc, t);
		return;
	}

	//the following handles selection.
	HandleSelectionClick(loc, t);
}

void ChessBoard::HandlePromotionClick(Team t, MoveType type)
{
	if (type == MoveType::QueenPromotion)
	{
		moveMade = Move({ moveMade.src, moveMade.dest,MoveType::QueenPromotion });
		turnSwap = true;
		isPromoting = false;
		IsInCheck(t);
	}

	else if (type == MoveType::KnightPromotion)
	{
		moveMade = Move({ moveMade.src, moveMade.dest,MoveType::KnightPromotion });
		turnSwap = true;
		isPromoting = false;
		IsInCheck(t);
	}

	if (t == Team::WHITE)
	{
		whiteInCheck = false;
	}
	else
	{
		blackInCheck = false;
	}
}

void ChessBoard::HandleMoveClick(const Vei2& loc, Team t)
{
	_Move selectedMove;
	for (const auto& move : userPossibleMoves)
	{
		if (move.dest == loc)
		{
			selectedMove = move;
			moveMade = selectedMove;
			break;
		}
	}
	isPromoting = (selectedMove.type == MoveType::QueenPromotion || selectedMove.type == MoveType::KnightPromotion) ? true : false;
	if (!isPromoting)
	{
		moveMade = Move(selectedMove);
		//as we are not promoting a piece, we now test to see if we have put the other player in check.
		turnSwap = true;
		//if white successfully makes a move, then white necessarily cannot be in check.
		if (t == Team::WHITE)
		{
			whiteInCheck = false;
		}
		//similarly for black.
		else
		{
			blackInCheck = false;
		}
		IsInCheck(t);
	}
	cellPreviouslyHighlighted = loc;
	ReleaseHighlights();
}

void ChessBoard::HandleSelectionClick(const Vei2& loc, Team t)
{
	BitBoard pos = SquareToBitboard(CoordsToSquare(loc));
	auto c = CellAt(loc);
	ReleaseHighlights();
	auto piece = c->OnClick(t);
	if(pieceBBs[(int)t] & pos )
	{
		c->Highlight(Cell::HighlightType::BLUE);
		cellPreviouslyHighlighted = loc;
		auto moves = GetValidMoves(loc);
		for (const auto& move : moves)
		{
			if (CellAt(move.dest)->Empty())
				CellAt(move.dest)->Highlight(Cell::HighlightType::YELLOW);
			else
				CellAt(move.dest)->Highlight(Cell::HighlightType::RED);
			if (move.type == MoveType::EnPassant)
				CellAt(move.dest)->Highlight(Cell::HighlightType::RED);
		}
		userPossibleMoves = moves;
	}
}

