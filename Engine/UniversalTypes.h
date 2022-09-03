#pragma once
#include "Vec2.h"
typedef unsigned short ushort;
typedef unsigned int uint;

enum class Team
{
	WHITE,
	BLACK
};
enum class Direction
{
	North,
	East,
	South,
	West,
	Northeast,
	Southeast,
	Southwest,
	Northwest
};
enum class MoveType
{
	Normal,
	EnPassant,
	KingsideCastle,
	QueensideCastle,
	QueenPromotion,
	KnightPromotion
};

enum class PieceType
{
	Empty,
	Pawn,
	Rook,
	Knight,
	Bishop,
	Queen,
	King
};

struct _Move
{
	enum class Flag
	{
		None,
		Capture,
		QueenPromotion,
		KnightPromotion,
		QueenPromotionCapture,
		KnightPromotionCapture,
		KingsideCastle,
		QueensideCastle,
		PawnDoublePush,
		EnPassant
	};
	_Move() = default;
	_Move(uint flags, uint source, uint target, uint srcPiece, uint capturedPiece)
	{
		move = (flags << 28) | source << 22 | target << 16 | srcPiece << 13 | capturedPiece << 10;
	}
	_Move(uint flags, uint source, uint target, uint srcPiece)
	{
		move = (flags << 28) | source << 22 | target << 16 | srcPiece << 13 | (uint)PieceType::Empty << 10;
	}
	_Move(uint flags, uint source, uint target, PieceType srcPiece, PieceType capturedPiece)
	{
		move = (flags << 28) | source << 22 | target << 16 | (uint)srcPiece << 13 | (uint)capturedPiece << 10;
	}
	_Move(uint flags, uint source, uint target, PieceType srcPiece)
	{
		move = (flags << 28) | source << 22 | target << 16 | (uint)srcPiece << 13 | (uint)PieceType::Empty << 10;
	}
	_Move(const _Move& other)
	{
		*this = other;
	}

	_Move(const _Move&& other) noexcept
		:
		move(other.move)
	{}
	bool operator==(const _Move& other)
	{
		return move == other.move;
	}
	_Move& operator=(const _Move& other)
	{
		if (this == &other)
			return *this;
		move = other.move;
		return *this;
	}

	uint GetSource() const
	{
		return (move & sourceMask) >> 22;
	}
	uint GetTarget() const
	{
		return (move & targetMask) >> 16;
	}
	Flag GetFlag() const
	{
		return (Flag)((move & flagMask) >> 28);
	}
	PieceType GetSourcePiece() const
	{
		return (PieceType)((move & pieceMask) >> 13);
	}
	PieceType GetCapturedPiece() const
	{
		return (PieceType)((move & capturedMask) >> 10);
	}

	//move data is stored in a packed short. 4 most significant bits store flag data, next 6 store source square, next 6 store target square, next 3 store source piece type, last 3 store captured piece type
protected:
	uint move;
	static constexpr uint flagMask = 0xF0000000;
	static constexpr uint sourceMask = 0x0FC00000;
	static constexpr uint targetMask = 0x003F0000;
	static constexpr uint pieceMask = 0x0000E000;
	static constexpr uint capturedMask = 0x00001C00;
};