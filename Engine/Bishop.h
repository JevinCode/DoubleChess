#pragma once
#include "Piece.h"

class Bishop : public Piece
{
public:
	Bishop(Team t, const Surface& surf, const Vei2& loc);
	void Draw(Graphics& gfx, const Vei2& loc) const override;
};