#pragma once
#include "Piece.h"

class Bishop : public Piece
{
public:
	Bishop(Team t, const Vei2& loc);
	void Draw(Graphics& gfx, const Vei2& loc) const override;
	std::vector<_Move> GetPossibleMoves(const ChessBoard& brd) const override; 
	std::vector<_Move> GetPossibleAttackMoves(const ChessBoard& brd) const override;
};