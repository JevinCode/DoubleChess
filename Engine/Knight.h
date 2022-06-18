#pragma once
#include "Piece.h"

class Knight : public Piece
{
public:
	Knight(Team t, const Surface& surf, const Vei2& loc);
	void Draw(Graphics& gfx, const Vei2& loc) const override;
	std::vector<Vei2> GetLegalMoves();
private:
	const Vei2 moveOffsets[8] = { Vei2{1,2}, Vei2{2,1}, Vei2{1,-2}, Vei2{2,-1}, Vei2{-1,2}, Vei2{-2,1}, Vei2{-1,-2}, Vei2{-2,-1} };
};