#include "Piece.h"

void Piece::Draw(Graphics& gfx, const Vei2& loc, Piece::PieceType p, Team t)
{
	switch (p)
	{
	case PieceType::Pawn:
		if (t == Team::BLACK)
			gfx.DrawSprite(loc.x, loc.y, { 147,177,3,33 }, s, SpriteEffect::Chroma{ Colors::Red });
		else
			gfx.DrawSprite(loc.x, loc.y, { 147,177,27,57 }, s, SpriteEffect::Chroma{ Colors::Red });
		break;

	case PieceType::Rook:
		if (t == Team::BLACK)
			gfx.DrawSprite(loc.x, loc.y, { 60,90,2,32 }, s, SpriteEffect::Chroma{ Colors::Red });
		else
			gfx.DrawSprite(loc.x, loc.y, { 60,90,27,57 }, s, SpriteEffect::Chroma{ Colors::Red });
		break;
	case PieceType::Knight:
		if (t == Team::BLACK)
			gfx.DrawSprite(loc.x, loc.y, { 120,150,2,32 }, s, SpriteEffect::Chroma{ Colors::Red });
		else
			gfx.DrawSprite(loc.x, loc.y, { 120,150,27,57 }, s, SpriteEffect::Chroma{ Colors::Red });
		break;

	case PieceType::Bishop:
		if (t == Team::BLACK)
			gfx.DrawSprite(loc.x, loc.y, { 89,119,2,32 }, s, SpriteEffect::Chroma{ Colors::Red });
		else
			gfx.DrawSprite(loc.x, loc.y, { 90,120,27,57 }, s, SpriteEffect::Chroma{ Colors::Red });
		break;

	case PieceType::Queen:
		if (t == Team::BLACK)
			gfx.DrawSprite(loc.x, loc.y, { 30,60,2,32 }, s, SpriteEffect::Chroma{ Colors::Red });
		else
			gfx.DrawSprite(loc.x, loc.y, { 30,60,27,57 }, s, SpriteEffect::Chroma{ Colors::Red });
		break;

	case PieceType::King:
		if (t == Team::BLACK)
			gfx.DrawSprite(loc.x, loc.y, { 0,30,2,32 }, s, SpriteEffect::Chroma{ Colors::Red });
		else
			gfx.DrawSprite(loc.x, loc.y, { 0,30,27,57 }, s, SpriteEffect::Chroma{ Colors::Red });
		break;
	}
}




//int Piece::GetNumAdjDefenders(const ChessBoard& brd) const
//{
//	int nPieces = 0;
//	Vei2 locs[8] = {
//	{ pos.x - 1, pos.y - 1 },
//	{ pos.x - 1, pos.y },
//	{ pos.x - 1, pos.y + 1 },
//	{ pos.x, pos.y - 1 },
//	{ pos.x, pos.y + 1 },
//	{ pos.x + 1, pos.y - 1 },
//	{ pos.x + 1, pos.y },
//	{ pos.x + 1, pos.y + 1 } };
//
//	for (const auto& loc : locs)
//		if (IsValidLoc(loc) && !brd.CellAt(loc)->Empty() && brd.CellAt(loc)->GetPiece()->GetTeam() == team)
//			nPieces++;
//
//	return nPieces;
//}


