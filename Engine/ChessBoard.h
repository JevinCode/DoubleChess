#pragma once
#include "Vec2.h"
#include <vector>
#include "Surface.h"
#include "Graphics.h"
#include <memory>
#include "Piece.h"
#include <typeinfo>
#include "Pawn.h"
#include "Knight.h"
#include "Bishop.h"
#include "Rook.h"
#include "King.h"
#include "Queen.h"

class ChessBoard
{
public:
	ChessBoard();
	void Draw(Graphics& gfx) const;
	static int LinearizeCoords(const Vei2& loc);
	static Vei2 Dimensify(int loc);
	Vei2 GetOffset() const;
	void OnClick(const Vei2& loc);
	enum class Pieces
	{
		BLACK_PAWN,
		WHITE_PAWN,
		BLACK_BISHOP,
		WHITE_BISHOP,
		BLACK_KNIGHT,
		WHITE_KNIGHT,
		BLACK_ROOK,
		WHITE_ROOK,
		BLACK_QUEEN,
		WHITE_QUEEN,
		BLACK_KING,
		WHITE_KING,
		NONE
	};
private:
	//member functions
	void ReleaseHighlights();
	bool IsValidLoc(const Vei2& loc) const;
	std::vector<std::pair<Vei2,Cell::HighlightType>> GetValidMoves(const Vei2& loc, const Pieces p) const;
	void Move(std::unique_ptr<Piece> piece, const Vei2& loc);

	//member data
	Team PlayerTurn = Team::WHITE;
	Vei2 EnCroissantSquare = { 0,0 };
	Vei2 cellPreviouslyHighlighted = { 0,6 };
	Surface sPieces = "Images\\chess_pieces.bmp";
	Surface tiles = "Images\\tiles.bmp";
	Vei2 topLeft = { 30,30 };
	std::unique_ptr<Cell> cells[64];
public:
	static constexpr int boardSize = 8 * Cell::dimension;
	static constexpr int cellSize = Cell::dimension;
};