#pragma once
#include "Piece.h"

class Pawn : public Piece
{
public:
	Pawn(Team t, const Surface& surf, const Vei2& loc);
	void Draw(Graphics& gfx, const Vei2& loc) const override;
	void Update(const Vei2& loc) override;
private:
	int numMoves = 0;
};