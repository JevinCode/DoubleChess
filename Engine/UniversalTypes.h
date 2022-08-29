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

struct _Move
{
	enum class Flag
	{
		None,
		Capture = 1,
		QueenPromotion = 2,
		KnightPromotion = 4,
		KingsideCastle = 8,
		QueensideCastle = 16,
		PawnDoublePush = 32,
		EnPassant = 64
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
	_Move() = default;
	_Move(uint flags, uint source, uint target, uint srcPiece, uint capturedPiece)
	{
		move = (flags << 26) | source << 20 | target << 14 | srcPiece << 11 | capturedPiece << 8;
	}
	_Move(uint flags, uint source, uint target, uint srcPiece)
	{
		move = (flags << 26) | source << 20 | target << 14 | srcPiece << 11 | (uint)PieceType::Empty << 8;
	}
	_Move(uint flags, uint source, uint target, PieceType srcPiece, PieceType capturedPiece)
	{
		move = (flags << 26) | source << 20 | target << 14 | (uint)srcPiece << 11 | (uint)capturedPiece << 8;
	}
	_Move(uint flags, uint source, uint target, PieceType srcPiece)
	{
		move = (flags << 26) | source << 20 | target << 14 | (uint)srcPiece << 11 | (uint)PieceType::Empty << 8;
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
		return (move & sourceMask) >> 20;
	}
	uint GetTarget() const
	{
		return (move & targetMask) >> 14;
	}
	uint GetFlags() const
	{
		return (move & flagMask) >> 26;
	}
	PieceType GetSourcePiece() const
	{
		return (PieceType)((move & pieceMask) >> 11);
	}
	PieceType GetCapturedPiece() const
	{
		return (PieceType)((move & capturedMask) >> 8);
	}

	//move data is stored in a packed short. 6 most significant bits store flag data, next 6 store source square, next 6 store target square, next 3 store source piece type, last 3 store captured piece type
protected:
	uint move;
	static constexpr uint flagMask = 0xFC000000;
	static constexpr uint sourceMask = 0x03F00000;
	static constexpr uint targetMask = 0x000FC000;
	static constexpr uint pieceMask = 0x00003800;
	static constexpr uint capturedMask = 0x00000700;
};