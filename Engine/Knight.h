#pragma once
#include "Piece.h"

class Knight : public Piece
{
public:
	Knight(Team t, const Vei2& loc);
	void Draw(Graphics& gfx, const Vei2& loc) const override;
	std::vector<Vei2> GetPossibleMoves(const ChessBoard& brd) const override;
	std::vector<Vei2> GetPossibleAttackMoves(const ChessBoard& brd) const override;
private:
	const Vei2 moveOffsets[8] = { Vei2{1,2}, Vei2{2,1}, Vei2{1,-2}, Vei2{2,-1}, Vei2{-1,2}, Vei2{-2,1}, Vei2{-1,-2}, Vei2{-2,-1} };
};