#pragma once
#include "Piece.h"

class Rook : public Piece
{
public:
	Rook(Team t, const Surface& surf, const Vei2& loc);
	void Draw(Graphics& gfx, const Vei2& loc) const override;
};