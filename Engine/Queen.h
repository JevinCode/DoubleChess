#pragma once
#include "Piece.h"

class Queen : public Piece
{
public:
	Queen(Team t, const Surface& surf, const Vei2& loc);
	void Draw(Graphics& gfx, const Vei2& loc) const override;
};