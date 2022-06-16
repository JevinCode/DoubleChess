#pragma once
#include "Vec2.h"
#include <vector>
#include "Surface.h"
#include "Graphics.h"
#include <memory>
#include "Piece.h"
#include <typeinfo>

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
	class Cell
	{
	public:
		enum class Shade
		{
			LIGHT,
			DARK
		};
		enum class HighlightType
		{
			BLUE,
			YELLOW,
			RED,
			NONE
		};
		Cell(Cell::Shade s, const Vei2& loc, const Surface& surf);
		bool Empty() const;
		void DrawCell(Graphics& gfx, const Vei2& offset);
		void GivePiece(std::unique_ptr<Piece> peace);
		ChessBoard::Pieces OnClick(Team PlayerTurn);
		void ReleaseHighlight();
		void Highlight(const HighlightType h);
		void Clear();
	private:
		const Shade shade;
		static constexpr int dimension = 30;
		const Vei2 loc;
		const Surface& s;
		std::unique_ptr<Piece> piece;
		ChessBoard::Pieces p = ChessBoard::Pieces::NONE;
		HighlightType highlight = HighlightType::NONE;
		friend class ChessBoard;
	};
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