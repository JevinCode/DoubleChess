#pragma once
#include "Vec2.h"
#include "Graphics.h"
#include "ChessBoard.h"


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