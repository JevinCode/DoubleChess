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

bool ChessBoard::IsValidLoc(const Vei2& loc) const
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
	if (Piece::IsCroissant() && loc == Piece::GetEnCroissantSquare() && typeid(*destCell->GetPiece()) == typeid(Pawn))
	{
		CellAt(Piece::GetEnCroissantPawnLoc())->Clear();
	}
	destCell->GetPiece()->Update(loc);
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
	return ans;
}
void ChessBoard::OnClick(const Vei2& loc)
{
	auto c = CellAt(loc);
	if (c->GetHighlight() == Cell::HighlightType::YELLOW || c->GetHighlight() == Cell::HighlightType::RED)
	{
		Move(CellAt(cellPreviouslyHighlighted), loc);
		CellAt(cellPreviouslyHighlighted)->Clear();
		ReleaseHighlights();
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

