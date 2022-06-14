#include "ChessBoard.h"

ChessBoard::ChessBoard()
{
	for (int row = 0; row < 8; row++)
	{
		Cell::Shade startShade = row % 2 ? Cell::Shade::DARK : Cell::Shade::LIGHT;
		for (int col = 0; col < 8; col++)
		{
			Vei2 loc = { row,col };
			auto sh = Cell::Shade(((int)startShade + col) % 2);
			cells[LinearizeCoords(loc)] = std::make_unique<Cell>(sh, loc, tiles);
		}
	}
	pieces[(int)Pieces::BLACK_PAWN] = std::make_unique<Pawn>(Piece::Team::BLACK, sPieces);
	pieces[(int)Pieces::WHITE_PAWN] = std::make_unique<Pawn>(Piece::Team::WHITE, sPieces);
	pieces[(int)Pieces::BLACK_BISHOP] = std::make_unique<Bishop>(Piece::Team::BLACK, sPieces);
	pieces[(int)Pieces::WHITE_BISHOP] = std::make_unique<Bishop>(Piece::Team::WHITE, sPieces);
	pieces[(int)Pieces::BLACK_KNIGHT] = std::make_unique<Knight>(Piece::Team::BLACK, sPieces);
	pieces[(int)Pieces::WHITE_KNIGHT] = std::make_unique<Knight>(Piece::Team::WHITE, sPieces);
	pieces[(int)Pieces::BLACK_ROOK] = std::make_unique<Rook>(Piece::Team::BLACK, sPieces);
	pieces[(int)Pieces::WHITE_ROOK] = std::make_unique<Rook>(Piece::Team::WHITE, sPieces);
	pieces[(int)Pieces::BLACK_QUEEN] = std::make_unique<Queen>(Piece::Team::BLACK, sPieces);
	pieces[(int)Pieces::WHITE_QUEEN] = std::make_unique<Queen>(Piece::Team::WHITE, sPieces);
	pieces[(int)Pieces::BLACK_KING] = std::make_unique<King>(Piece::Team::BLACK, sPieces);
	pieces[(int)Pieces::WHITE_KING] = std::make_unique<King>(Piece::Team::WHITE, sPieces);
	
	for (int i = 8; i < 16; i++)
	{
		cells[i]->GivePiece(Pieces::BLACK_PAWN);
	}
	for (int x = 0; x < 8; x++)
	{
		Vei2 loc = { x,6 };
		cells[LinearizeCoords(loc)]->GivePiece(Pieces::WHITE_PAWN);
	}
	cells[LinearizeCoords({ 2,0 })]->GivePiece(Pieces::BLACK_BISHOP);
	cells[LinearizeCoords({ 5,0 })]->GivePiece(Pieces::BLACK_BISHOP);
	cells[LinearizeCoords({ 2,7 })]->GivePiece(Pieces::WHITE_BISHOP);
	cells[LinearizeCoords({ 5,7 })]->GivePiece(Pieces::WHITE_BISHOP);

	cells[LinearizeCoords({ 1,0 })]->GivePiece(Pieces::BLACK_KNIGHT);
	cells[LinearizeCoords({ 6,0 })]->GivePiece(Pieces::BLACK_KNIGHT);
	cells[LinearizeCoords({ 1,7 })]->GivePiece(Pieces::WHITE_KNIGHT);
	cells[LinearizeCoords({ 6,7 })]->GivePiece(Pieces::WHITE_KNIGHT);

	cells[LinearizeCoords({ 0,0 })]->GivePiece(Pieces::BLACK_ROOK);
	cells[LinearizeCoords({ 7,0 })]->GivePiece(Pieces::BLACK_ROOK);
	cells[LinearizeCoords({ 0,7 })]->GivePiece(Pieces::WHITE_ROOK);
	cells[LinearizeCoords({ 7,7 })]->GivePiece(Pieces::WHITE_ROOK);

	cells[LinearizeCoords({ 3,0 })]->GivePiece(Pieces::BLACK_QUEEN);
	cells[LinearizeCoords({ 4,7 })]->GivePiece(Pieces::WHITE_QUEEN);

	cells[LinearizeCoords({ 4,0 })]->GivePiece(Pieces::BLACK_KING);
	cells[LinearizeCoords({ 3,7 })]->GivePiece(Pieces::WHITE_KING);
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


ChessBoard::Cell::Cell(Cell::Shade s, const Vei2& loc, const Surface& surf)
	:
	shade(s),
	loc(loc),
	s(surf)
{}


bool ChessBoard::Cell::Empty() const
{
	return piece == nullptr;
}


void ChessBoard::Cell::DrawCell(Graphics& gfx, const Vei2& offset)
{
	int xDest = loc.x * dimension + offset.x;
	int yDest = loc.y * dimension + offset.y;
	if (shade == Shade::DARK)
	{
		gfx.DrawSprite(xDest, yDest, { 0,30,0,30 }, s, SpriteEffect::Chroma{ Colors::Magenta });
	}
	else
	{
		gfx.DrawSprite(xDest, yDest, { 30,60,0,30 }, s, SpriteEffect::Chroma{ Colors::Magenta });
	}
	if (!Empty())
	{
		piece->Draw(gfx, { xDest, yDest });
	}
}

void ChessBoard::Cell::GivePiece(const Pieces p)
{
	piece = pieces[(int)p];
}

Piece::Piece(Team t, const Surface& surf)
	:
	team(t),
	s(surf)
{}

Piece::Team Piece::GetTeam() const
{
	return team;
}

Pawn::Pawn(Team t, const Surface& surf)
	:
	Piece(t,surf)
{}

void Pawn::Draw(Graphics& gfx, const Vei2& loc) const
{
	auto team = this->GetTeam();
	if (team == Team::BLACK)
	{
		gfx.DrawSprite(loc.x, loc.y, { 147,177,3,33 }, s, SpriteEffect::Chroma{ Colors::Red });
	}
	else
	{
		gfx.DrawSprite(loc.x, loc.y, { 147,177,27,57 }, s, SpriteEffect::Chroma{ Colors::Red });
	}
}

Bishop::Bishop(Team t, const Surface& surf)
	:
	Piece(t, surf)
{}

void Bishop::Draw(Graphics& gfx, const Vei2& loc) const
{
	auto team = this->GetTeam();
	if (team == Team::BLACK)
	{
		gfx.DrawSprite(loc.x, loc.y, { 89,119,2,32 }, s, SpriteEffect::Chroma{ Colors::Red });
	}
	else
	{
		gfx.DrawSprite(loc.x, loc.y, { 90,120,27,57 }, s, SpriteEffect::Chroma{ Colors::Red });
	}
}

Knight::Knight(Team t, const Surface& surf)
	:
	Piece(t, surf)
{}

void Knight::Draw(Graphics & gfx, const Vei2 & loc) const
{
	auto team = this->GetTeam();
	if (team == Team::BLACK)
	{
		gfx.DrawSprite(loc.x, loc.y, { 120,150,2,32 }, s, SpriteEffect::Chroma{ Colors::Red });
	}
	else
	{
		gfx.DrawSprite(loc.x, loc.y, { 120,150,27,57 }, s, SpriteEffect::Chroma{ Colors::Red });
	}
}

Rook::Rook(Team t, const Surface& surf)
	:
	Piece(t, surf)
{}

void Rook::Draw(Graphics& gfx, const Vei2& loc) const
{
	auto team = this->GetTeam();
	if (team == Team::BLACK)
	{
		gfx.DrawSprite(loc.x, loc.y, { 60,90,2,32 }, s, SpriteEffect::Chroma{ Colors::Red });
	}
	else
	{
		gfx.DrawSprite(loc.x, loc.y, { 60,90,27,57 }, s, SpriteEffect::Chroma{ Colors::Red });
	}
}
Queen::Queen(Team t, const Surface & surf)
	:
	Piece(t, surf)
{}

void Queen::Draw(Graphics& gfx, const Vei2& loc) const
{
	auto team = this->GetTeam();
	if (team == Team::BLACK)
	{
		gfx.DrawSprite(loc.x, loc.y, { 30,60,2,32 }, s, SpriteEffect::Chroma{ Colors::Red });
	}
	else
	{
		gfx.DrawSprite(loc.x, loc.y, { 30,60,27,57 }, s, SpriteEffect::Chroma{ Colors::Red });
	}
}

King::King(Team t, const Surface& surf)
	:
	Piece(t, surf)
{}

void King::Draw(Graphics& gfx, const Vei2& loc) const
{
	auto team = this->GetTeam();
	if (team == Team::BLACK)
	{
		gfx.DrawSprite(loc.x, loc.y, { 0,30,2,32 }, s, SpriteEffect::Chroma{ Colors::Red });
	}
	else
	{
		gfx.DrawSprite(loc.x, loc.y, { 0,30,27,57 }, s, SpriteEffect::Chroma{ Colors::Red });
	}
}
