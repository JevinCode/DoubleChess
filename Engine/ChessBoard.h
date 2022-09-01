#pragma once
#include <memory>
#include "Piece.h"
#include <typeinfo>
#include <string>
#include "BitBoard.h"
#include "BBTwiddler.h"
#include <stack>
#include "Bencher.h"
#include "EnumArray.h"

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
		a8, b8, c8, d8, e8, f8, g8, h8,
		kMaxValue = h8
	};
	enum class BBIndex
	{
		White,
		Black,
		Pawns,
		Rooks,
		Knights,
		Bishops,
		Queens,
		Kings,
		Occupied,
		Empty,
		kMaxValue = Empty
	};
	Bencher b;
	ChessBoard(const Vei2& topLeft);
	void ApplyMove(_Move m, Team t);
	void Draw(Graphics& gfx) const;
	static int LinearizeCoords(const Vei2& loc);
	static Vei2 Dimensify(int loc);
	Vei2 GetOffset() const;
	EnumArray<BBIndex, BitBoard> GetPieceBBs() const;
	void OnClick(const Vei2& loc, Team t);
	std::shared_ptr<Cell> CellAt(const Vei2& loc) const;
	std::shared_ptr<Cell> CellAt(const Square sq) const;
	static bool IsValidLoc(const Vei2& loc);
	bool IsEnPassantable() const;
	Vei2 GetEnPassantSquare() const;
	Vei2 GetEnPassantPawnLoc() const;
	Team GetPassantTeam() const;
	bool IsCheckmate() const;
	bool CanCastleKingside(Team t) const;
	bool CanCastleQueenside(Team t) const;
	void HandlePromotionClick(Team t, MoveType type);
	static Square CoordsToSquare(const Vei2& coords);
	static BitBoard SquareToBitBoard(const Square sq);
	static BitBoard SquaresToBitBoard(const std::vector<Square> squares);
	static std::vector<Square> BitBoardToSquares(BitBoard bb);
	static Vei2 SquareToCoords(const Square sq);
	static std::vector<Vei2> SquaresToCoords(const std::vector<Square>& squares);
	Vei2 GetScreenCoords(const Square sq) const;
	std::vector<Vei2> GetScreenCoords(const std::vector<Square>& squares) const;
	_Move::PieceType ParseCapture(Square sq) const;
	const std::vector<std::vector<BitBoard>> GetRayAttacks() const;
	BitBoard GetKingDangerSquares(Team t) const;


private:
	//member functions

	void GenerateRayAttackBBs();
	void GenerateKnightAttackBBs();
	void GenerateKingAttackBBs();
	void GenerateMoves(Team t);
	void DrawPieces(Graphics& gfx) const;
	void ClearHighlights();
	std::vector<_Move> GetPossibleMoves(const Square sq) const;
	_Move Move(_Move mv);
	bool IsWhiteInCheck() const;
	bool IsBlackInCheck() const;
	void IsInCheck(Team t);
	std::vector<_Move> GetValidMoves(const Vei2& loc);
	bool IsUnderAttack(Team t, const Vei2& loc) const;
	void IsCheckmate(Team t);
	void HandleMoveClick(const Square sq, Team t);
	void HandleSelectionClick(const Vei2& loc, Team t);
	BBIndex PieceTypeMatcher(_Move::PieceType p) const;
	BitBoard CalculateKingDangerSquares(Team t);
	BitBoard GetKnightAttackBB(Team t);
	//member dataclass

	enum class DirectionOffsets
	{
		North = 8,
		East = 1,
		South = -8,
		West = -1,
		NorthEast = 9,
		SouthEast = -7,
		SouthWest = -9,
		NorthWest = 7
	};

	EnumArray<BBIndex, BitBoard> pieceBBs;

	//second index is used as follows: 0 = North, 1 = East, 2 = South, 3 = West, 4 = Northeast, 5 = Southeast, 6 = Southwest, 7 = Northeast

	std::vector<std::vector<BitBoard>> RayAttacks;
	//static BitBoard RookAttacks[64];
	EnumArray<Square, BitBoard> KnightAttacks;
	BitBoard BishopAttacks[64];
	BitBoard QueenAttacks[64];
	EnumArray<Square, BitBoard> KingAttacks;
	//0 = White, 1 = Black
	BitBoard KingDangerSquares[2] = { 0,0 };


	std::stack<_Move> plies;
	bool turnSwap = false;
	Square squarePreviouslyHighlighted = Square::a1;
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
	friend class PseudoLegalMoveGenerator;

public:
	static constexpr int cellSize = 30;
	static constexpr int boardSize = 8 * cellSize;
	//constants
	static constexpr BitBoard Universe = 0xFFFFFFFFFFFFFFFF;
	static constexpr BitBoard Empty = 0;
	static constexpr BitBoard AFile = 0x0101010101010101;
	static constexpr BitBoard BFile = 0x0202020202020202;
	static constexpr BitBoard ABFile = AFile | BFile;
	static constexpr BitBoard GFile = 0x4040404040404040;
	static constexpr BitBoard HFile = 0x8080808080808080;
	static constexpr BitBoard GHFile = GFile | HFile;

	static constexpr BitBoard NotAFile = AFile ^ Universe;
	static constexpr BitBoard NotABFile = ABFile ^ Universe;
	static constexpr BitBoard NotHFile = HFile ^ Universe;
	static constexpr BitBoard NotGHFile = GHFile ^ Universe;
	static constexpr BitBoard Rank1 = 0x00000000000000FF;
	static constexpr BitBoard Rank2 = 0x000000000000FF00;
	static constexpr BitBoard Rank4 = 0x00000000FF000000;
	static constexpr BitBoard Rank5 = 0x000000FF00000000;
	static constexpr BitBoard Rank7 = 0x00FF000000000000;
	static constexpr BitBoard Rank8 = 0xFF00000000000000;
};