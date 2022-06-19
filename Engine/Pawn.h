#pragma once
#include "Piece.h"

class Pawn : public Piece
{
public:
	Pawn(Team t, const Vei2& loc);
	void Draw(Graphics& gfx, const Vei2& loc) const override;
	void Update(const Vei2& loc) override;
	std::vector<Vei2> GetPossibleMoves(const ChessBoard& brd) const override;
	std::vector<Vei2> GetPossibleAttackMoves(const ChessBoard& brd) const override;
private:
	int numMoves = 0;
};