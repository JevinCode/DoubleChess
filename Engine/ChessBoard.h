#pragma once
#include "Vec2.h"
#include <vector>
#include "Surface.h"
#include "Graphics.h"
#include <memory>

class Piece
{
public:
	enum class Team
	{
		WHITE,
		BLACK
	};
	Piece(Team t, const Surface& surf);
	virtual void Draw(Graphics& gfx, const Vei2& loc) const = 0;
	Team GetTeam() const;
private:
	Team team;
protected:
	const Surface& s;
};

class Pawn : public Piece
{
public:
	Pawn(Team t, const Surface& surf);
	void Draw(Graphics& gfx, const Vei2& loc) const override;
};

class Bishop : public Piece
{
public:
	Bishop(Team t, const Surface& surf);
	void Draw(Graphics& gfx, const Vei2& loc) const override;
};

class Knight : public Piece
{
public:
	Knight(Team t, const Surface& surf);
	void Draw(Graphics& gfx, const Vei2& loc) const override;
};

class Rook : public Piece
{
public:
	Rook(Team t, const Surface& surf);
	void Draw(Graphics& gfx, const Vei2& loc) const override;
};

class Queen : public Piece
{
public:
	Queen(Team t, const Surface& surf);
	void Draw(Graphics& gfx, const Vei2& loc) const override;
};

class King : public Piece
{
public:
	King(Team t, const Surface& surf);
	void Draw(Graphics& gfx, const Vei2& loc) const override;
};

class ChessBoard
{
public:
	ChessBoard();
	void Draw(Graphics& gfx) const;
	static int LinearizeCoords(const Vei2& loc);
	static Vei2 Dimensify(int loc);
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
	private:
		Shade shade;
		static constexpr int dimension = 30;
		const Vei2 loc;
		const Surface& s;
		std::shared_ptr<Piece> piece = nullptr;
	};
	//member data
	Surface sPieces = "Images\\chess_pieces.bmp";
	Surface tiles = "Images\\tiles.bmp";
	Vei2 topLeft = { 30,30 };
	std::unique_ptr<Cell> cells[64];
	inline static std::shared_ptr<Piece> pieces[12];
	friend class Piece;
};