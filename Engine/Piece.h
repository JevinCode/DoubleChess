#pragma once
#include "Graphics.h"

class ChessBoard;
class Cell;
enum class Team
{
	WHITE,
	BLACK
};
class Piece
{
public:
	Piece(Team t, const Vei2& pos);
	virtual void Draw(Graphics& gfx, const Vei2& loc) const = 0;
	virtual std::vector<Vei2> GetPossibleMoves(const ChessBoard& brd) const = 0;
	virtual std::vector<Vei2> GetPossibleAttackMoves(const ChessBoard& brd) const = 0;
	inline static bool IsCroissant()
	{
		return isEnCroissantable;
	}
	virtual void Update(const Vei2& loc);
	const Vei2& GetPosition() const;
	void SetPosition(const Vei2& loc);
	inline static Vei2& GetEnCroissantSquare()
	{
		return EnCroissantSquare;
	}
	inline static Vei2& GetEnCroissantPawnLoc()
	{
		return EnCroissantPawn;
	}
	bool HasMoved() const;
	Team GetTeam() const;
protected:
	bool IsValidLoc(const Vei2& loc) const;
	inline static Surface s = "Images\\chess_pieces.bmp";
	Vei2 pos;
	inline static Vei2 EnCroissantSquare;
	inline static Vei2 EnCroissantPawn;
	inline static bool isEnCroissantable;
	Team team;
	bool hasMoved = false;
};