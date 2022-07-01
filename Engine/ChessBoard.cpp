#include "ChessBoard.h"
#include "Cell.h"

ChessBoard::ChessBoard()
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
		int i = LinearizeCoords({ x,1 });
		cells[i]->GivePiece(std::make_shared<Pawn>(Team::BLACK, Vei2{ x,1 }));
	}
	cells[LinearizeCoords({ 0,0 })]->GivePiece( std::make_shared<Rook>(Team::BLACK,  Vei2{ 0,0 }));
	cells[LinearizeCoords({ 1,0 })]->GivePiece( std::make_shared<Knight>(Team::BLACK, Vei2{ 1,0 }));
	cells[LinearizeCoords({ 2,0 })]->GivePiece( std::make_shared<Bishop>(Team::BLACK, Vei2{ 2,0 }));
	cells[LinearizeCoords({ 3,0 })]->GivePiece( std::make_shared<Queen>(Team::BLACK, Vei2{ 3,0 }));
	cells[LinearizeCoords({ 4,0 })]->GivePiece( std::make_shared<King>(Team::BLACK, Vei2{4,0 }));
	cells[LinearizeCoords({ 5,0 })]->GivePiece( std::make_shared<Bishop>(Team::BLACK, Vei2{ 5,0 }));
	cells[LinearizeCoords({ 6,0 })]->GivePiece( std::make_shared<Knight>(Team::BLACK, Vei2{ 6,0 }));
	cells[LinearizeCoords({ 7,0 })]->GivePiece( std::make_shared<Rook>(Team::BLACK, Vei2{ 7,0 }));

	for (int x = 0; x < 8; x++)
	{
		int i = LinearizeCoords({ x,6 });
		cells[i]->GivePiece(std::make_shared<Pawn>(Team::WHITE, Vei2{x,6}));
	}
	cells[LinearizeCoords({ 0,7 })]->GivePiece(std::make_shared<Rook>(Team::WHITE, Vei2{ 0,7 }));
	cells[LinearizeCoords({ 1,7 })]->GivePiece(std::make_shared<Knight>(Team::WHITE, Vei2{ 1,7 }));
	cells[LinearizeCoords({ 2,7 })]->GivePiece(std::make_shared<Bishop>(Team::WHITE, Vei2{ 2,7 }));
	cells[LinearizeCoords({ 3,7 })]->GivePiece(std::make_shared<Queen>(Team::WHITE, Vei2{ 3,7 }));
	cells[LinearizeCoords({ 4,7 })]->GivePiece(std::make_shared<King>(Team::WHITE, Vei2{ 4,7 }));
	cells[LinearizeCoords({ 5,7 })]->GivePiece(std::make_shared<Bishop>(Team::WHITE, Vei2{ 5,7 }));
	cells[LinearizeCoords({ 6,7 })]->GivePiece(std::make_shared<Knight>(Team::WHITE, Vei2{ 6,7 }));
	cells[LinearizeCoords({ 7,7 })]->GivePiece(std::make_shared<Rook>(Team::WHITE, Vei2{ 7,7 }));
	
}

void ChessBoard::Draw(Graphics& gfx) const
{
	for (const auto& c : cells)
	{
		c->DrawCell(gfx, topLeft);
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

Vei2 ChessBoard::GetOffset() const
{
	return topLeft;
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
	return false;
}

std::vector<Vei2> ChessBoard::GetPossibleMoves(const Vei2& loc) const
{
	return CellAt(loc)->GetPiece()->GetPossibleMoves(*this);
}

void ChessBoard::Move(std::shared_ptr<Cell> src, const Vei2& loc)
{
	auto destCell = CellAt(loc);
	destCell->GivePiece(src->GetPiece());
	src->Clear();
	auto p = destCell->GetPiece();
	if (Piece::IsCroissant() && loc == Piece::GetEnCroissantSquare() && typeid(*p) == typeid(Pawn))
	{
		CellAt(Piece::GetEnCroissantPawnLoc())->Clear();
	}
	else if (typeid(*p) == typeid(King) && !p->HasMoved())
	{
		if (loc == Vei2{ 6,7 })
		{
			hasCastledWhite = true;
			Move(CellAt({ 7,7 }), { 5,7 });
		}
		else if (loc == Vei2{ 2,7 })
		{
			hasCastledWhite = true;
			Move(CellAt({ 0,7 }), { 3,7 });
		}
		else if (loc == Vei2{ 6,0 })
		{
			hasCastledBlack = true;
			Move(CellAt({ 7,0 }), { 5,0 });
		}
		else if (loc == Vei2{ 2,0 })
		{
			hasCastledBlack = true;
			Move(CellAt({ 0,0 }), { 3,0 });
		}
	}
	p->Update(loc);
	if (typeid(*p) == typeid(Pawn) && ((loc.y == 0 && p->GetTeam() == Team::WHITE) || (loc.y == 7 && p->GetTeam() == Team::BLACK)))
		isPromoting = true;
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
					auto c = CellAt(move);
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
					auto c = CellAt(move);
					if (!c->Empty() && c->GetPiece()->GetTeam() == Team::BLACK && typeid(*c->GetPiece()) == typeid(King))
						return true;
				}
			}
		}
	}
	return false;
}
bool ChessBoard::SimulateAndCheck(std::shared_ptr<Cell> src, const Vei2& loc)
{
	auto t = src->GetPiece()->GetTeam();
	auto dest = CellAt(loc);

	//make a copy of the piece at destCell
	auto destPiece = dest->GetPiece();

	dest->GivePiece(src->GetPiece());
	src->Clear();

	std::shared_ptr<Piece> croissant = nullptr;
	bool croissantified = false;

	if (Piece::IsCroissant() && loc == Piece::GetEnCroissantSquare() && typeid(*dest->GetPiece()) == typeid(Pawn))
	{
		croissant = CellAt(Piece::GetEnCroissantPawnLoc())->GetPiece();
		CellAt(Piece::GetEnCroissantPawnLoc())->Clear();
		croissantified = true;
	}

	//test if this move causes king to be in check
	bool badMove = t == Team::WHITE ? IsWhiteInCheck() : IsBlackInCheck();

	//restore pieces to their original position
	src->GivePiece(dest->GetPiece());
	dest->GivePiece(destPiece);
	if (croissantified)
		CellAt(Piece::GetEnCroissantPawnLoc())->GivePiece(croissant);

	return badMove;
}
std::vector<Vei2> ChessBoard::GetValidMoves(const Vei2& loc)
{
	std::vector<Vei2> ans;
	auto moves = CellAt(loc)->GetPiece()->GetPossibleMoves(*this);
	for (const auto& move : moves)
	{
		if (!SimulateAndCheck(CellAt(loc), move))
			ans.push_back(move);
	}
	if (loc == Vei2{4, 0})
	{
		if (CanCastleKingside(Team::BLACK))
			ans.push_back({ 6,0 });
		if (CanCastleQueenside(Team::BLACK))
			ans.push_back({ 2,0 });
	}
	if (loc == Vei2{ 4,7 })
	{
		if (CanCastleKingside(Team::WHITE))
			ans.push_back({ 6,7 });
		if (CanCastleQueenside(Team::WHITE))
			ans.push_back({ 2,7 });
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
					if (l == loc)
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
		if (!CellAt({ 4,7 })->Empty() && !CellAt({ 7,7 })->Empty())
		{
			auto k = CellAt({ 4,7 })->GetPiece();
			auto r = CellAt({ 7,7 })->GetPiece();
			if (typeid(*k) == typeid(King) && typeid(*r) == typeid(Rook) && !k->HasMoved() && !r->HasMoved() && CellAt({ 5,7 })->Empty() && CellAt({ 6,7 })->Empty() && !IsUnderAttack(t, { 5,7 }) && !IsUnderAttack(t, { 6,7 }) && !IsWhiteInCheck())
				return true;
		}
	}
	else
	{
		if (hasCastledBlack)
			return false;
		if (!CellAt({ 4,0 })->Empty() && !CellAt({ 7,0 })->Empty())
		{
			auto k = CellAt({ 4,0 })->GetPiece();
			auto r = CellAt({ 7,0 })->GetPiece();
			if (typeid(*k) == typeid(King) && typeid(*r) == typeid(Rook) && !k->HasMoved() && !r->HasMoved() && CellAt({ 5,0 })->Empty() && CellAt({ 6,0 })->Empty() && !IsUnderAttack(t, { 5,0 }) && !IsUnderAttack(t, { 6,0 }) && !IsBlackInCheck())
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
		if (!CellAt({ 4,7 })->Empty() && !CellAt({ 0,7 })->Empty())
		{
			auto k = CellAt({ 4,7 })->GetPiece();
			auto r = CellAt({ 0,7 })->GetPiece();
			if (typeid(*k) == typeid(King) && typeid(*r) == typeid(Rook) && !k->HasMoved() && !r->HasMoved() && CellAt({ 1,7 })->Empty() && CellAt({ 2,7 })->Empty() && CellAt({ 3,7 })->Empty() && !IsUnderAttack(t, { 2,7 }) && !IsUnderAttack(t, { 3,7 }))
				return true;
		}
	}
	else
	{
		if (hasCastledBlack)
			return false;
		if (!CellAt({ 4,0 })->Empty() && !CellAt({ 0,0 })->Empty())
		{
			auto k = CellAt({ 4,0 })->GetPiece();
			auto r = CellAt({ 0,0 })->GetPiece();
			if (typeid(*k) == typeid(King) && typeid(*r) == typeid(Rook) && !k->HasMoved() && !r->HasMoved() && CellAt({ 1,0 })->Empty() && CellAt({ 2,0 })->Empty() && CellAt({ 3,0 })->Empty() && !IsUnderAttack(t, { 2,0 }) && !IsUnderAttack(t, { 3,0 }))
				return true;
		}
	}
	return false;
}
void ChessBoard::OnClick(const Vei2& loc)
{
	if (loc == Vei2{ 8, 0 })
	{
		CellAt(cellPreviouslyHighlighted)->GivePiece(std::make_shared<Queen>(PlayerTurn, cellPreviouslyHighlighted));
		PlayerTurn = (Team)(((int)PlayerTurn + 1) % 2);
		isPromoting = false;
		return;
	}

	else if (loc == Vei2{ 8,1 })
	{
		CellAt(cellPreviouslyHighlighted)->GivePiece(std::make_shared<Knight>(PlayerTurn, cellPreviouslyHighlighted));
		PlayerTurn = (Team)(((int)PlayerTurn + 1) % 2);
		isPromoting = false;
		return;
	}

	auto c = CellAt(loc);
	if (c->GetHighlight() == Cell::HighlightType::YELLOW || c->GetHighlight() == Cell::HighlightType::RED)
	{
		Move(CellAt(cellPreviouslyHighlighted), loc);
		cellPreviouslyHighlighted = loc;
		ReleaseHighlights();
		if(!isPromoting)
			PlayerTurn = (Team)(((int)PlayerTurn + 1) % 2);
		return;
	}
	ReleaseHighlights();
	auto piece = c->OnClick(PlayerTurn);
	if (piece)
	{
		c->Highlight(Cell::HighlightType::BLUE);
		cellPreviouslyHighlighted = loc;
		auto moveTargets = GetValidMoves(loc);
		for (const auto& loc : moveTargets)
		{
			if (CellAt(loc)->Empty())
				CellAt(loc)->Highlight(Cell::HighlightType::YELLOW);
			else
				CellAt(loc)->Highlight(Cell::HighlightType::RED);
			if (Piece::IsCroissant() && loc == Piece::GetEnCroissantSquare() && piece->GetTeam() != CellAt(Piece::GetEnCroissantPawnLoc())->GetPiece()->GetTeam())
				CellAt(loc)->Highlight(Cell::HighlightType::RED);
		}
	}
}

