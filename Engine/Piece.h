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
	void Update(const Vei2& loc);
	const Vei2& GetPosition() const;
	void SetPosition(const Vei2& loc);
	bool HasMoved() const;
	Team GetTeam() const;
	int GetNumMoves();
	int GetNumAdjDefenders(const ChessBoard& brd) const;
protected:
	bool IsValidLoc(const Vei2& loc) const;
	inline static Surface s = "Images\\chess_pieces.bmp";
	Vei2 pos;
	Team team;
	bool hasMoved = false;
	int numMoves = 0;
};