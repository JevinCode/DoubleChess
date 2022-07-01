#pragma once
#include "Graphics.h"
#include <memory>
#include "Piece.h"
#include <typeinfo>
#include "Pawn.h"
#include "Knight.h"
#include "Bishop.h"
#include "Rook.h"
#include "King.h"
#include "Queen.h"

class ChessBoard
{
public:
	ChessBoard();
	void Draw(Graphics& gfx) const;
	static int LinearizeCoords(const Vei2& loc);
	static Vei2 Dimensify(int loc);
	Vei2 GetOffset() const;
	void OnClick(const Vei2& loc, Team t);
	std::shared_ptr<Cell> CellAt(const Vei2& loc);
	std::shared_ptr<Cell> CellAt(const Vei2& loc) const;
private:
	//member functions
	void ReleaseHighlights();
	static bool IsValidLoc(const Vei2& loc);
	std::vector<Vei2> GetPossibleMoves(const Vei2& loc) const;
	void Move(std::shared_ptr<Cell> cell, const Vei2& loc);
	bool IsWhiteInCheck() const;
	bool IsBlackInCheck() const;
	bool SimulateAndCheck(std::shared_ptr<Cell> cell, const Vei2& loc);
	std::vector<Vei2> GetValidMoves(const Vei2& loc);
	bool IsUnderAttack(Team t, const Vei2& loc) const;
	bool CanCastleKingside(Team t) const;
	bool CanCastleQueenside(Team t) const;

	//member data
	bool turnSwap = false;
	Vei2 EnCroissantSquare = { 0,0 };
	Vei2 cellPreviouslyHighlighted = { 0,6 };
	Surface sPieces = "Images\\chess_pieces.bmp";
	Vei2 topLeft = { 30,30 };
	std::shared_ptr<Cell> cells[64];
	bool hasCastledWhite = false;
	bool hasCastledBlack = false;
	bool isPromoting = false;

	friend class Game;
public:
	static constexpr int cellSize = 30;
	static constexpr int boardSize = 9 * cellSize;
};