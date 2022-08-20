#pragma once
#include "Graphics.h"
#include "Piece.h"
#include <memory>


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
	Cell(Cell::Shade s, const Vei2& loc);
	Cell(Cell::Shade s, const Vei2& loc, std::shared_ptr<Piece> p);
	bool Empty() const;
	void DrawCell(Graphics& gfx, const Vei2& offset);
	void GivePiece(std::shared_ptr<Piece> peace);
	Vei2 GetLoc() const;
	std::shared_ptr<Piece> OnClick(Team PlayerTurn);
	void ReleaseHighlight();
	void Highlight(const HighlightType h);
	HighlightType GetHighlight() const;
	void Clear();
	std::shared_ptr<Piece> GetPiece() const;
private:
	const Shade shade;
	static constexpr int dimension = 30;
	const Vei2 loc;
	inline static Surface s = std::string("Images\\tiles.bmp");
	std::shared_ptr<Piece> piece = nullptr;
	HighlightType highlight = HighlightType::NONE;
};