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

class ChessAI;

class ChessBoard
{
public:
	ChessBoard(const Vei2& topLeft);
	void Draw(Graphics& gfx) const;
	static int LinearizeCoords(const Vei2& loc);
	static Vei2 Dimensify(int loc);
	Vei2 GetOffset() const;
	void OnClick(const Vei2& loc, Team t);
	std::shared_ptr<Cell> CellAt(const Vei2& loc);
	std::shared_ptr<Cell> CellAt(const Vei2& loc) const;
	static bool IsValidLoc(const Vei2& loc);
	bool IsEnPassantable() const;
	Vei2 GetEnPassantSquare() const;
	Vei2 GetEnPassantPawnLoc() const;
	Team GetPassantTeam() const;
private:
	//member functions
	void ReleaseHighlights();
	std::vector<Vei2> GetPossibleMoves(const Vei2& loc) const;
	void Move(const Vei2& src, const Vei2& dest);
	bool IsWhiteInCheck() const;
	bool IsBlackInCheck() const;
	void IsInCheck(Team t);
	bool SimulateAndCheck(std::shared_ptr<Cell> cell, const Vei2& loc);
	std::vector<Vei2> GetValidMoves(const Vei2& loc);
	bool IsUnderAttack(Team t, const Vei2& loc) const;
	bool CanCastleKingside(Team t) const;
	bool CanCastleQueenside(Team t) const;
	void PostMoveUpdate(const std::shared_ptr<Piece> p, const Vei2& loc);
	bool IsCheckmate(Team t);

	//member data
	bool turnSwap = false;
	Vei2 cellPreviouslyHighlighted = { 0,6 };
	Surface sPieces = "Images\\chess_pieces.bmp";
	const Vei2 topLeft;
	std::shared_ptr<Cell> cells[64];
	bool hasCastledWhite = false;
	bool hasCastledBlack = false;
	bool isPromoting = false;
	bool isEnPassantable = false;
	bool whiteInCheck = false;
	bool blackInCheck = false;
	Vei2 enPassantSquare = { 0,0 };
	Vei2 enPassantPawnLoc = { 0,0 };
	Team passantTeam = Team::WHITE;
	friend class Game;
	friend class ChessAI;
public:
	static constexpr int cellSize = 30;
	static constexpr int boardSize = 9 * cellSize;
};