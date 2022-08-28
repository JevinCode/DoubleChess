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
#include <string>
#include "UniversalTypes.h"
#include "BitBoard.h"
class ChessAI;

class ChessBoard
{
public:
	//here we use little endian rank-file order (LERF) to enumerate the squares of the board
	enum class Square {
		a1, b1, c1, d1, e1, f1, g1, h1,
		a2, b2, c2, d2, e2, f2, g2, h2,
		a3, b3, c3, d3, e3, f3, g3, h3,
		a4, b4, c4, d4, e4, f4, g4, h4,
		a5, b5, c5, d5, e5, f5, g5, h5,
		a6, b6, c6, d6, e6, f6, g6, h6,
		a7, b7, c7, d7, e7, f7, g7, h7,
		a8, b8, c8, d8, e8, f8, g8, h8
	};
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
	bool IsCheckmate() const;
	bool CanCastleKingside(Team t) const;
	bool CanCastleQueenside(Team t) const;
	void HandlePromotionClick(Team t, MoveType type);
	Square CoordsToSquare(const Vei2& coords) const;
	BitBoard SquareToBitboard(const Square sq) const;

private:
	//member functions
	void ReleaseHighlights();
	std::vector<_Move> GetPossibleMoves(const Vei2& loc) const;
	_Move Move(_Move mv);
	bool IsWhiteInCheck() const;
	bool IsBlackInCheck() const;
	void IsInCheck(Team t);
	bool SimulateAndCheck(_Move move);
	std::vector<_Move> GetValidMoves(const Vei2& loc);
	bool IsUnderAttack(Team t, const Vei2& loc) const;
	void PostMoveUpdate(const std::shared_ptr<Piece> p, const Vei2& loc);
	void IsCheckmate(Team t);
	void HandleMoveClick(const Vei2& loc, Team t);
	void HandleSelectionClick(const Vei2& loc, Team t);

	//member dataclass

	enum class Pieces
	{
		White,
		Black,
		WhitePawn,
		BlackPawn,
		WhiteRook,
		BlackRook,
		WhiteKnight,
		BlackKnight,
		WhiteBishop,
		BlackBishop,
		WhiteQueen,
		BlackQueen,
		WhiteKing,
		BlackKing
	};
	BitBoard pieceBBs[14] = {
		0x000000000000FFFF, //White pieces (aka first two ranks)
		0xFFFF000000000000, //Black pieces (aka last two ranks)
		0x000000000000FF00, //White pawns
		0x00FF000000000000, //Black pawns
		0x0000000000000081, //White Rooks
		0x8100000000000000, //Black Rooks
		0x0000000000000042, //White Knights
		0x4200000000000000, //Black Knights
		0x0000000000000024, //White Bishops
		0x2400000000000000, //Black Bishops
		0x0000000000000008, //White Queen
		0x0800000000000000, //Black Queen
		0x0000000000000010, //White King
		0x1000000000000000, //Black King
	}; 


	bool turnSwap = false;
	Vei2 cellPreviouslyHighlighted = { 0,6 };
	Surface sPieces = std::string("Images\\chess_pieces.bmp");
	const Vei2 topLeft;
	std::shared_ptr<Cell> cells[64];
	bool hasCastledWhite = false;
	bool hasCastledBlack = false;
	bool isPromoting = false;
	bool isEnPassantable = false;
	bool whiteInCheck = false;
	bool blackInCheck = false;
	bool isCheckmate = false;
	Vei2 enPassantSquare = { 0,0 };
	Vei2 enPassantPawnLoc = { 0,0 };
	Team passantTeam = Team::WHITE;
	_Move moveMade;
	std::vector<_Move> userPossibleMoves;
	friend class Game;
	friend class ChessAI;
public:
	static constexpr int cellSize = 30;
	static constexpr int boardSize = 9 * cellSize;
};