#pragma once
#include "Piece.h"

class King : public Piece
{
public:
	King(Team t, const Surface& surf, const Vei2& loc);
	void Draw(Graphics& gfx, const Vei2& loc) const override;
};