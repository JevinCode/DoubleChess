#pragma once
#include "Vec2.h"
#include <vector>
#include "Surface.h"
#include "Graphics.h";
#include <memory>

class ChessBoard
{
public:
	ChessBoard();
	enum class Team
	{
		WHITE,
		BLACK
	};
	void Draw(Graphics& gfx) const;
private:
	int LinearizeCoords(const Vei2& loc);
	Vei2 Dimensify(int loc);
	class Cell
	{
	public:
		enum class Shade
		{
			LIGHT,
			DARK
		};
		Cell(Cell::Shade s, const Vei2& loc, const Surface& surf);
		bool Empty();
		void DrawCell(Graphics& gfx, const Vei2& offset);
	private:
		Shade shade;
		static constexpr int dimension = 30;
		const Vei2 loc;
		const Surface& s;

	};
	//member data
	Surface pieces = "Images\\chess_pieces.bmp";
	Surface tiles = "Images\\tiles.bmp";
	Vei2 topLeft = { 30,30 };
	std::unique_ptr<Cell> cells[64];
	friend class Piece;
};

class Piece
{
public:
	Piece( ChessBoard::Team t, const Surface& surf);
	virtual void Move(const Vei2& loc) = 0;
	virtual void Draw(Graphics& gfx) const = 0;
private:
	ChessBoard::Team team;
	Surface s;
};

class Pawn : public Piece
{
public:
	Pawn(ChessBoard::Team t, const Surface& surf);
	void Draw(Graphics& gfx) const override;
	void Move(const Vei2& loc) override;
};