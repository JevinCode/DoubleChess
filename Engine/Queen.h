#pragma once
#include "Piece.h"

class Queen : public Piece
{
public:
	Queen(Team t, const Vei2& loc);
	void Draw(Graphics& gfx, const Vei2& loc) const override;
	static void DrawWhite(Graphics& gfx, const ChessBoard& brd);
	static void DrawBlack(Graphics& gfx, const ChessBoard& brd);
	std::vector<_Move> GetPossibleMoves(const ChessBoard& brd) const override;
	std::vector<_Move> GetPossibleAttackMoves(const ChessBoard& brd) const override;
};