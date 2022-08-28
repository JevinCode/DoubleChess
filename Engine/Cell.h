#pragma once
#include "Graphics.h"
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
	void Draw(Graphics& gfx, const Vei2& offset) const;
	Vei2 GetLoc() const;
	void ClearHighlight();
	void Highlight(const HighlightType h);
	HighlightType GetHighlight() const;
private:
	const Shade shade;
	static constexpr int dimension = 30;
	const Vei2 loc;
	inline static Surface s = std::string("Images\\tiles.bmp");
	HighlightType highlight = HighlightType::NONE;
};