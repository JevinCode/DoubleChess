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
#include <unordered_map>

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
		kMaxValue = Occupied
	};
	ChessBoard(const Vei2& topLeft);
	void ApplyMove(_Move m, Team t);
	void RevertMove();
	void Draw(Graphics& gfx) const;
	static int LinearizeCoords(const Vei2& loc);
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
	static Square BitBoardToSquare(BitBoard bb); //returns lsb of bb as square
	static Vei2 SquareToCoords(const Square sq);
	static std::vector<Vei2> SquaresToCoords(const std::vector<Square>& squares);
	Vei2 GetScreenCoords(const Square sq) const;
	std::vector<Vei2> GetScreenCoords(const std::vector<Square>& squares) const;
	PieceType ParseCapture(Square sq) const;
	const std::vector<std::vector<BitBoard>> GetRayAttacks() const;
	BitBoard GetKingDangerSquares(Team t) const;
	bool IsInCheck(Team t) const;
	std::unordered_map<Square, BitBoard> GetCorridors() const;
	BitBoard GetPins() const;
	bool IsDoubleCheck(Team t) const;
	BitBoard GetCheckCorridor() const;
	BitBoard GetEmptyBB() const;
	bool IsStalemate() const;
private:
	//member functions

	void GenerateRayAttackBBs();
	void GenerateKnightAttackBBs();
	void GenerateKingAttackBBs();
	void GenerateMoves(Team t);
	void DrawPieces(Graphics& gfx) const;
	void ClearHighlights();
	std::vector<_Move> GetPossibleMoves(const Square sq) const;
	std::vector<_Move> GetValidMoves(const Vei2& loc);
	void IsCheckmate(Team t);
	void HandleMoveClick(const Square sq, Team t);
	void HandleSelectionClick(const Vei2& loc, Team t);
	BBIndex PieceTypeMatcher(PieceType p) const;
	BitBoard CalculateKingDangerSquares(Team t);
	BitBoard CalculatePins(Team t);
	BitBoard GetKnightAttackBB(Team t);
	BitBoard GetKingAttackers(Team t);
	BitBoard GetCheckCorridor(Team t, Square sq, PieceType p);
	void SetCastleFlag(Team t);
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
	//BitBoard BishopAttacks[64];
	//BitBoard QueenAttacks[64];
	//0 = White, 1 = Black
	std::vector<BitBoard> kingDangerSquares = {0,0};
	BitBoard pins = 0;
	BitBoard kingAttackers = 0; //we update this only when the king gets put in check
	BitBoard checkCorridor = 0;

	std::unordered_map<Square, BitBoard> pinCorridors;
	std::stack<_Move> plies;
	std::stack<std::vector<BitBoard>> kingDangerStack;
	bool turnSwap = false;
	Square squarePreviouslyHighlighted = Square::a1;
	Surface sPieces = std::string("Images\\chess_pieces.bmp");
	const Vei2 topLeft;
	std::shared_ptr<Cell> cells[64];
	bool hasCastledWhite = false;
	bool hasCastledBlack = false;
	bool canCastleKingsideWhite = true;
	bool canCastleQueensideWhite = true;
	bool canCastleKingsideBlack = true;
	bool canCastleQueensideBlack = true;
	bool isPromoting = false;
	bool isEnPassantable = false;
	int kingsideWhitePlies = 0;
	int queensideWhitePlies = 0;
	int kingsideBlackPlies = 0;
	int queensideBlackPlies = 0;
	//bool whiteInCheck = false;
	//bool blackInCheck = false;
	bool isCheckmate = false;
	Vei2 enPassantSquare = { 0,0 };
	Vei2 enPassantPawnLoc = { 0,0 };
	Team passantTeam = Team::WHITE;
	std::vector<_Move> userPossibleMoves;
	friend class Game;
	friend class ChessAI;
	friend class PseudoLegalMoveGenerator;

public:
	EnumArray<Square, BitBoard> KnightAttacks;
	EnumArray<Square, BitBoard> KingAttacks;
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
	static constexpr BitBoard NotRank1 = Universe ^ Rank1;
	static constexpr BitBoard NotRank8 = Universe ^ Rank8;
};