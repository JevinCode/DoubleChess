#pragma once
#include "Vec2.h"
#include <vector>
#include "Surface.h"
#include "Graphics.h"
#include <memory>
#include "Piece.h"

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
		WHITE_KING
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
		Cell(Cell::Shade s, const Vei2& loc, const Surface& surf);
		bool Empty() const;
		void DrawCell(Graphics& gfx, const Vei2& offset);
		void GivePiece(const Pieces p);
		void Highlight();
		void ReleaseHighlight();
	private:
		Shade shade;
		static constexpr int dimension = 30;
		const Vei2 loc;
		const Surface& s;
		std::shared_ptr<Piece> piece = nullptr;
		bool isHighlighted = false;
		friend class ChessBoard;
	};
	//member functions
	void ReleaseHighlights();
	//member data
	Surface sPieces = "Images\\chess_pieces.bmp";
	Surface tiles = "Images\\tiles.bmp";
	Vei2 topLeft = { 30,30 };
	std::unique_ptr<Cell> cells[64];
	inline static std::shared_ptr<Piece> pieces[12];
	friend class Piece;
public:
	static constexpr int boardSize = 8 * Cell::dimension;
	static constexpr int cellSize = Cell::dimension;
};