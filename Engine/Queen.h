#pragma once
#include "Piece.h"

class Queen : public Piece
{
public:
	Queen(Team t, const Vei2& loc);
	void Draw(Graphics& gfx, const Vei2& loc) const override;
	std::vector<Vei2> GetPossibleMoves(const ChessBoard& brd) const override;
	std::vector<Vei2> GetPossibleAttackMoves(const ChessBoard& brd) const override;
};