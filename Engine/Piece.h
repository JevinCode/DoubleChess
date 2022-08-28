#pragma once
#include "Graphics.h"
#include "UniversalTypes.h"

class ChessBoard;
class Cell;
class Piece
{
public:
	enum class PieceType
	{
		Pawn,
		Rook,
		Knight,
		Bishop,
		Queen,
		King
	};

	Piece() = delete;
	static void Draw(Graphics& gfx, const Vei2& loc, PieceType p, Team t);
private:
	inline static Surface s = std::string("Images\\chess_pieces.bmp");
};